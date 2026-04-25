#include "dsa/error_heap.h"
#include "todo_helper.h"

using nm::testing::run_or_todo;
using namespace nm::dsa;

int main() {
    run_or_todo("ErrorHeap pops largest error first", []() {
        ErrorHeap h;
        h.push({0, 1, 1.0, 0.5});
        h.push({1, 2, 1.0, 0.1});
        h.push({2, 3, 1.0, 0.9});
        NM_CHECK(h.size() == 3);
        auto top = h.pop_max();
        NM_CHECK_NEAR(top.error_est, 0.9, 1e-12);
        top = h.pop_max();
        NM_CHECK_NEAR(top.error_est, 0.5, 1e-12);
    });

    return nm::testing::exit_code();
}
