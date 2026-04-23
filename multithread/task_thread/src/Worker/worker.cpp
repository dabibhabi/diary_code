/**
 * @file worker.cpp
 * @brief Worker implementation — run_loop() is the core of the concurrency model.
 */
#include "worker.hpp"
#include "task.hpp"

#include <sstream>
#include <utility>

Worker::Worker(int id) : m_id(id), m_stop(false), m_running(false) {}

Worker::~Worker() {
    stop();
}

std::string Worker::label() const {
    std::ostringstream os;
    os << "w" << m_id;
    return os.str();
}

void Worker::start() {
    if (m_running.exchange(true)) return;
    m_stop.store(false);
    m_thread = std::thread([this] { run_loop(); });
}

void Worker::stop() {
    if (!m_running.exchange(false)) return;
    m_stop.store(true);
    m_cv.notify_all();
    if (m_thread.joinable()) m_thread.join();
}

/**
 * @brief Enqueue @p task and subscribe to each of its dependencies.
 *
 * The listener registered on each dep calls notify_one() on this worker's
 * CV. That is how this worker's thread wakes up when a dep finishes on a
 * *different* thread — no polling needed.
 */
void Worker::submit(std::shared_ptr<Task> task) {
    for (const auto& dep : task->dependencies()) {
        dep->add_completion_listener([this] { m_cv.notify_one(); });
    }
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_back(std::move(task));
    }
    m_cv.notify_one();
}

bool Worker::has_ready_task_unlocked() const {
    for (const auto& t : m_queue) {
        if (t->is_ready()) return true;
    }
    return false;
}

std::shared_ptr<Task> Worker::pick_ready_task_unlocked() {
    // Highest priority among ready tasks wins; queue order breaks ties.
    auto best = m_queue.end();
    for (auto it = m_queue.begin(); it != m_queue.end(); ++it) {
        if (!(*it)->is_ready()) continue;
        if (best == m_queue.end() || (*it)->priority() > (*best)->priority()) {
            best = it;
        }
    }
    if (best == m_queue.end()) return nullptr;
    auto task = *best;
    m_queue.erase(best);
    return task;
}

/**
 * @brief Main worker loop.
 *
 * Steps per iteration:
 *   1. Sleep on the CV until either stop is requested or a ready task exists.
 *   2. If stopping and no ready work remains, exit.
 *   3. Pick the highest-priority ready task, record it as current, release lock.
 *   4. Run the task (outside the lock, so long work doesn't block submit()).
 *   5. Clear the "current task" field and loop.
 */
void Worker::run_loop() {
    while (true) {
        std::shared_ptr<Task> task;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [&] {
                return m_stop.load() || has_ready_task_unlocked();
            });

            if (m_stop.load() && !has_ready_task_unlocked()) return;

            task = pick_ready_task_unlocked();
            if (!task) continue;
            m_current_task = task->name();
        }

        task->run();

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_current_task.clear();
        }
    }
}

Worker::Snapshot Worker::take_snapshot() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    Snapshot s;
    s.id = m_id;
    std::ostringstream os;
    os << "w" << m_id;
    s.label = os.str();
    s.state = m_current_task.empty() ? "IDLE" : "RUNNING";
    s.current_task = m_current_task;
    s.backlog = m_queue.size();
    return s;
}
