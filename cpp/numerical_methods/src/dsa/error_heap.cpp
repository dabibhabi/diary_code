#include "dsa/error_heap.h"

#include "todo.h"

#include <stdexcept>

namespace nm::dsa {

// TODO — standard binary-heap implementation:
//   data_.push_back(s); sift_up(data_.size() - 1);
void ErrorHeap::push(const Subinterval& /*s*/) { NM_TODO("ErrorHeap::push"); }

// TODO — swap top with last, pop_back, sift_down(0).
Subinterval ErrorHeap::pop_max() { NM_TODO("ErrorHeap::pop_max"); }

const Subinterval& ErrorHeap::peek_max() const {
    if (data_.empty()) throw std::out_of_range("peek_max on empty heap");
    return data_.front();
}

// TODO — bubble up: while parent.error_est < data_[i].error_est, swap.
void ErrorHeap::sift_up(std::size_t /*i*/) { NM_TODO("ErrorHeap::sift_up"); }

// TODO — bubble down: pick larger child, swap if heap property violated.
void ErrorHeap::sift_down(std::size_t /*i*/) { NM_TODO("ErrorHeap::sift_down"); }

}  // namespace nm::dsa
