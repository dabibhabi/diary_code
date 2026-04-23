/**
 * @file pool.cpp
 * @brief Pool implementation — thin layer on top of Worker.
 */
#include "pool.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

Pool::Pool(int num_workers) {
    for (int i = 1; i <= num_workers; ++i) {
        m_workers.emplace_back(std::make_unique<Worker>(i));
    }
}

Pool::~Pool() {
    stop();
}

void Pool::start() {
    for (auto& w : m_workers) w->start();
}

void Pool::stop() {
    for (auto& w : m_workers) w->stop();
}

Worker* Pool::find_worker(int id) {
    for (auto& w : m_workers) {
        if (w->id() == id) return w.get();
    }
    return nullptr;
}

void Pool::assign(int worker_id, std::shared_ptr<Task> task) {
    Worker* w = find_worker(worker_id);
    if (!w) throw std::invalid_argument("unknown worker id");
    w->submit(std::move(task));
}

Pool::compound_builder Pool::build_compound(std::string name) {
    return compound_builder(*this, std::move(name));
}

Pool::compound_builder::compound_builder(Pool& pool, std::string name)
    : m_pool(pool), m_name(std::move(name)) {}

Pool::compound_builder& Pool::compound_builder::part_on(
        int worker_id, std::string name, Task::work_fn work, TaskPriority priority) {
    auto task = std::make_shared<Task>(std::move(name), std::move(work), priority);
    m_pool.assign(worker_id, task);
    m_parts.push_back(std::move(task));
    return *this;
}

std::shared_ptr<Task> Pool::compound_builder::join_on(
        int worker_id, std::string name, Task::work_fn work, TaskPriority priority) {
    auto join = std::make_shared<Task>(std::move(name), std::move(work), priority);
    for (auto& part : m_parts) join->add_dependency(part);
    m_pool.assign(worker_id, join);
    return join;
}

void Pool::render() const {
    std::cout << "--- pool state ---\n";
    for (const auto& w : m_workers) {
        auto s = w->take_snapshot();
        std::cout << s.label
                  << " [" << s.state << "]"
                  << " current=" << (s.current_task.empty() ? "-" : s.current_task)
                  << " backlog=" << s.backlog << "\n";
    }
    std::cout << std::flush;
}
