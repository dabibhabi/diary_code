/**
 * @file task.hpp
 * @brief A unit of work with optional dependencies and completion listeners.
 *
 * Task uses the Command pattern (it wraps a callable) and the Observer pattern
 * (anyone can subscribe to be notified when the task finishes).
 */
#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

/** @brief Scheduling priority. Higher values run first among ready tasks. */
enum class TaskPriority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2
};

/** @brief Lifecycle of a Task. Transitions: PENDING -> RUNNING -> DONE. */
enum class TaskState {
    PENDING,
    RUNNING,
    DONE
};

/**
 * @class Task
 * @brief One unit of work. Optionally waits on other tasks before running.
 *
 * A Task is "ready" when every dependency is DONE. A Worker only picks up
 * ready tasks from its queue. On completion the Task atomically transitions
 * to DONE and fires every registered listener — this is how downstream
 * workers get woken up.
 */
class Task {
public:
    using work_fn = std::function<void()>;       ///< The callable the task runs.
    using listener_fn = std::function<void()>;   ///< Fired once when the task reaches DONE.

    /**
     * @brief Construct a task with a name, work function, and priority.
     * @param name     Human-readable label (shown in snapshots).
     * @param work     The callable to execute when run() fires.
     * @param priority Used by the worker to pick among ready tasks.
     */
    Task(std::string name, work_fn work, TaskPriority priority = TaskPriority::MEDIUM);

    /** @brief Register another task that must be DONE before this one is ready. */
    void add_dependency(std::shared_ptr<Task> dep);

    /**
     * @brief Subscribe a callback to fire when this task reaches DONE.
     * @note If the task is already DONE, the callback fires synchronously.
     */
    void add_completion_listener(listener_fn cb);

    /** @brief Execute the work, mark DONE, and notify listeners. Called by a Worker. */
    void run();

    /** @brief Block the calling thread until this task reaches DONE. */
    void wait();

    /** @brief True when every dependency is DONE (i.e. this task may run now). */
    bool is_ready() const;

    /** @brief True when state() == DONE. */
    bool is_done() const;

    const std::string& name() const { return m_name; }
    TaskPriority priority() const { return m_priority; }
    TaskState state() const { return m_state.load(); }
    const std::vector<std::shared_ptr<Task>>& dependencies() const { return m_deps; }

private:
    std::string m_name;
    work_fn m_work;
    TaskPriority m_priority;
    std::atomic<TaskState> m_state;

    std::vector<std::shared_ptr<Task>> m_deps;

    mutable std::mutex m_mutex;
    std::condition_variable m_done_cv;
    std::vector<listener_fn> m_listeners;
};
