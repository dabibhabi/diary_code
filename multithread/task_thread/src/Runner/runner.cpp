/**
 * @file runner.cpp
 * @brief Demo of the task / worker / pool system.
 *
 * @section overview Overview
 * This project is a small, readable example of concurrent task dispatch with
 * per-worker queues and inter-task dependencies.
 *
 * @section files Files
 * - @ref task.hpp / @ref task.cpp
 *   Task = one unit of work. Holds a std::function (Command pattern), an
 *   optional list of dependency Tasks, and a list of completion listeners
 *   (Observer pattern). Transitions PENDING -> RUNNING -> DONE.
 *
 * - @ref worker.hpp / @ref worker.cpp
 *   Worker = one OS thread with its own queue. Each worker is named w1, w2,
 *   ... wN. Instead of running tasks strictly in FIFO order, the worker
 *   picks the highest-priority task whose deps are all DONE. It sleeps on a
 *   condition variable between jobs and wakes up when (a) a task is
 *   submitted, (b) a dependency of one of its queued tasks finishes, or (c)
 *   the Pool asks it to stop.
 *
 * - @ref pool.hpp / @ref pool.cpp
 *   Pool = the registry of workers. Exposes assign(id, task) for direct
 *   dispatch and a fluent Builder (build_compound().part_on(...).join_on(...))
 *   for the "many parts across many workers, one join at the end" pattern.
 *
 * - runner.hpp / runner.cpp
 *   Exposes the demo as a single runner() entry point that the outer
 *   main.cpp calls.
 *
 * - ../main.cpp
 *   Thin outer entry point. Just calls runner().
 *
 * @section scenario Scenario demonstrated
 *   - taskB runs on w3 by itself (no dependencies).
 *   - taskA decomposes into:
 *       part1 on w1,  part2 on w2,  and join on w3.
 *   - The join will not start until both parts are DONE.
 *   - Meanwhile w3 is NOT blocked: it runs taskB first, because taskB is
 *     ready and the join is still waiting on its deps.
 *
 * @section concurrency How the concurrency works
 *   - Each Worker has its own std::mutex + std::condition_variable guarding
 *     its queue. The only shared mutable state is each Task's state field
 *     (std::atomic<TaskState>) and its listener list.
 *   - When a Task finishes, it fires every registered listener. Workers
 *     that had that Task as a dep registered a notify_one() listener at
 *     submit-time, so the listener call wakes up those workers. No polling.
 *   - Listeners are fired OUTSIDE the Task's lock to avoid self-deadlock.
 */
#include "runner.hpp"
#include "pool.hpp"

#include <chrono>
#include <iostream>
#include <thread>

void runner() {
    // Three workers: w1, w2, w3. start() spins up one thread per worker.
    Pool pool(3);
    pool.start();

    // --- Standalone task ---
    // No dependencies, so w3 can run this the moment it pops off the queue.
    auto standalone = std::make_shared<Task>("taskB_on_w3", [] {
        std::cout << "  [w3] running taskB\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    });
    pool.assign(3, standalone);

    // --- Compound task (parts + join) ---
    // part_on(N, ...) creates a task and ships it to wN immediately.
    // join_on(N, ...) creates a task that depends on every prior part and
    // ships it to wN. That join will stay in wN's queue as "not ready" until
    // every part has reached DONE on its own worker.
    auto joined = pool.build_compound("taskA")
        .part_on(1, "taskA_part1_on_w1", [] {
            std::cout << "  [w1] running taskA_part1\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        })
        .part_on(2, "taskA_part2_on_w2", [] {
            std::cout << "  [w2] running taskA_part2\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        })
        .join_on(3, "taskA_join_on_w3", [] {
            std::cout << "  [w3] running taskA_join\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });

    // Small pause, then print a snapshot so you can see what's in flight.
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    pool.render();

    // Block until the whole graph has finished.
    joined->wait();
    standalone->wait();

    std::cout << "\nafter completion:\n";
    pool.render();

    pool.stop();  // also happens in Pool's destructor; explicit for clarity.
}
