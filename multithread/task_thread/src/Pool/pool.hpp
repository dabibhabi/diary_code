/**
 * @file pool.hpp
 * @brief A registry of named Workers plus a Builder for multi-worker tasks.
 *
 * The Pool owns every Worker, exposes direct assignment (`assign(id, task)`),
 * and provides a fluent Builder for the common "parts on many workers, join
 * on one" pattern.
 */
#pragma once

#include "task.hpp"
#include "worker.hpp"

#include <memory>
#include <string>
#include <vector>

/**
 * @class Pool
 * @brief Owns N Workers identified by 1..N and routes tasks to them.
 */
class Pool {
public:
    /** @brief Create @p num_workers workers with ids 1..num_workers. */
    explicit Pool(int num_workers);

    /** @brief Stops every worker in destruction order. */
    ~Pool();

    Pool(const Pool&) = delete;
    Pool& operator=(const Pool&) = delete;

    /** @brief Start every worker's thread. */
    void start();

    /** @brief Signal every worker to stop and join their threads. */
    void stop();

    /**
     * @brief Submit @p task directly to worker @p worker_id.
     * @throws std::invalid_argument if the worker id is unknown.
     */
    void assign(int worker_id, std::shared_ptr<Task> task);

    /**
     * @class compound_builder
     * @brief Fluent Builder for "parts + join" task graphs.
     *
     * Each call to part_on() creates a sub-task and ships it to the named
     * worker. The final join_on() call creates a task that depends on every
     * previously added part and ships it to its worker. The join will only
     * become ready after all parts finish, no matter which order they end in.
     */
    class compound_builder {
    public:
        /** @brief Internal. Use Pool::build_compound() to create one. */
        compound_builder(Pool& pool, std::string name);

        /**
         * @brief Add a part task that runs on @p worker_id.
         * @return *this, for chaining.
         */
        compound_builder& part_on(int worker_id,
                                  std::string name,
                                  Task::work_fn work,
                                  TaskPriority priority = TaskPriority::MEDIUM);

        /**
         * @brief Add the join task that waits on every previous part.
         * @return shared_ptr to the join task so callers can wait() on it.
         */
        std::shared_ptr<Task> join_on(int worker_id,
                                      std::string name,
                                      Task::work_fn work,
                                      TaskPriority priority = TaskPriority::MEDIUM);

    private:
        Pool& m_pool;
        std::string m_name;
        std::vector<std::shared_ptr<Task>> m_parts;
    };

    /** @brief Begin assembling a compound (multi-worker) task. */
    compound_builder build_compound(std::string name);

    /** @brief Print a one-line snapshot per worker to std::cout. */
    void render() const;

private:
    Worker* find_worker(int id);

    std::vector<std::unique_ptr<Worker>> m_workers;
};
