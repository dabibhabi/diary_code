/**
 * @file task.cpp
 * @brief Task implementation — completion signalling is the interesting part.
 */
#include "task.hpp"

#include <utility>

Task::Task(std::string name, work_fn work, TaskPriority priority)
    : m_name(std::move(name))
    , m_work(std::move(work))
    , m_priority(priority)
    , m_state(TaskState::PENDING) {}

void Task::add_dependency(std::shared_ptr<Task> dep) {
    m_deps.push_back(std::move(dep));
}

/**
 * @brief Register @p cb to fire on completion. Safe against a race with run().
 *
 * If we see DONE under the lock, fire immediately (outside the lock so the
 * callback can do whatever it wants). Otherwise stash the callback; run()
 * will pick it up when it flips the state.
 */
void Task::add_completion_listener(listener_fn cb) {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_state.load() == TaskState::DONE) {
        lock.unlock();
        cb();
        return;
    }
    m_listeners.push_back(std::move(cb));
}

bool Task::is_ready() const {
    for (const auto& dep : m_deps) {
        if (!dep->is_done()) return false;
    }
    return true;
}

bool Task::is_done() const {
    return m_state.load() == TaskState::DONE;
}

/**
 * @brief Execute the work, flip state to DONE, then notify everyone waiting.
 *
 * We swap the listener vector out under the lock so we can fire callbacks
 * without holding the Task mutex — a callback might turn around and try to
 * take this same lock (e.g. via add_completion_listener) and self-deadlock.
 */
void Task::run() {
    m_state.store(TaskState::RUNNING);
    if (m_work) m_work();

    std::vector<listener_fn> to_fire;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_state.store(TaskState::DONE);
        to_fire.swap(m_listeners);
    }
    m_done_cv.notify_all();
    for (auto& cb : to_fire) cb();
}

void Task::wait() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_done_cv.wait(lock, [&] { return m_state.load() == TaskState::DONE; });
}
