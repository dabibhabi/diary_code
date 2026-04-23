/**
 * @file worker.hpp
 * @brief A single thread with its own queue of Tasks. Named w1, w2, ..., wn.
 *
 * Each Worker owns one std::thread and a personal queue. It sleeps on a
 * condition variable until either (a) a new task arrives, or (b) a
 * dependency it was waiting on has completed on some other worker.
 */
#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Task;

/**
 * @class Worker
 * @brief One thread drains one private task queue; skips not-yet-ready tasks.
 */
class Worker {
public:
    /** @brief Point-in-time view of a worker, safe to print. */
    struct Snapshot {
        int id;                  ///< Numeric id (1, 2, ...).
        std::string label;       ///< "w1", "w2", ...
        std::string state;       ///< "IDLE" or "RUNNING".
        std::string current_task;///< Name of the in-flight task, if any.
        std::size_t backlog;     ///< Number of tasks still queued.
    };

    /** @brief Construct an idle worker. Call start() to spin up the thread. */
    explicit Worker(int id);

    /** @brief Stops and joins the worker's thread. */
    ~Worker();

    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;

    /** @brief Spin up the worker thread. Idempotent. */
    void start();

    /** @brief Signal the worker to exit, wake it, and join its thread. */
    void stop();

    /**
     * @brief Hand a task to this worker. Also subscribes to the task's deps.
     *
     * The subscription is what lets this worker wake up when a dep finishes
     * on some other worker's thread.
     */
    void submit(std::shared_ptr<Task> task);

    int id() const { return m_id; }        ///< Numeric worker id.
    std::string label() const;             ///< Returns "w<id>".
    Snapshot take_snapshot() const;        ///< Copy out a state snapshot (locks briefly).

private:
    /** @brief The thread entry point. Loops until stopped and drained. */
    void run_loop();

    /** @brief Pick the highest-priority ready task. Caller must hold m_mutex. */
    std::shared_ptr<Task> pick_ready_task_unlocked();

    /** @brief True if any queued task has all deps DONE. Caller holds m_mutex. */
    bool has_ready_task_unlocked() const;

    int m_id;
    std::thread m_thread;
    std::atomic<bool> m_stop;
    std::atomic<bool> m_running;

    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
    std::vector<std::shared_ptr<Task>> m_queue;
    std::string m_current_task;
};
