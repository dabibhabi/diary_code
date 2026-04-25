#include "dsa/tridiag.h"
#include "todo_helper.h"

using nm::testing::run_or_todo;
using namespace nm::dsa;

int main() {
    run_or_todo("Tridiag solve on small SPD-tridiagonal", []() {
        Tridiag T;
        T.sub                   = {0.0, -1.0, -1.0};
        T.diag                  = {2.0, 2.0, 2.0};
        T.super                 = {-1.0, -1.0, 0.0};
        std::vector<double> rhs = {1.0, 0.0, 1.0};
        auto                x   = solve(T, rhs);
        NM_CHECK(x.size() == 3);
        NM_CHECK_NEAR(x[0], 1.0, 1e-10);
        NM_CHECK_NEAR(x[1], 1.0, 1e-10);
        NM_CHECK_NEAR(x[2], 1.0, 1e-10);
    });

    return nm::testing::exit_code();
}
