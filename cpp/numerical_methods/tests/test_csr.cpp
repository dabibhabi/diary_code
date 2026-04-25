#include "dsa/csr.h"
#include "todo_helper.h"

using nm::testing::run_or_todo;
using namespace nm::dsa;

int main() {
    run_or_todo("CSR Laplacian 1-D shape", []() {
        auto L = laplacian_1d(4);
        NM_CHECK(L.rows == 4 && L.cols == 4);
        // 4 rows, each interior with 3 nnz, endpoints with 2 → 2 + 3*2 + 2 = 10 nnz
        NM_CHECK(static_cast<int>(L.nnz()) == 10);
    });

    run_or_todo("CSR spmv on Laplacian", []() {
        auto                L = laplacian_1d(4);
        std::vector<double> x = {1.0, 1.0, 1.0, 1.0};
        auto                y = spmv(L, x);
        // Lx for constant x: row 0: 2-1=1; rows 1,2: 0; row 3: 2-1=1.
        NM_CHECK_NEAR(y[0], 1.0, 1e-12);
        NM_CHECK_NEAR(y[1], 0.0, 1e-12);
        NM_CHECK_NEAR(y[3], 1.0, 1e-12);
    });

    return nm::testing::exit_code();
}
