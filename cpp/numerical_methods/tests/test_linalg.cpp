#include "linalg.h"
#include "todo_helper.h"

#include <cmath>

using nm::testing::run_or_todo;
using namespace nm::linalg;

static Matrix make_spd_3x3() {
    Matrix A(3, 3);
    A(0, 0) = 4;
    A(0, 1) = 2;
    A(0, 2) = 1;
    A(1, 0) = 2;
    A(1, 1) = 5;
    A(1, 2) = 3;
    A(2, 0) = 1;
    A(2, 1) = 3;
    A(2, 2) = 6;
    return A;
}

int main() {
    run_or_todo("LU solve 3x3", []() {
        Matrix A   = make_spd_3x3();
        Vector b   = {7, 10, 10};
        auto   fac = lu_decompose(A);
        Vector x   = lu_solve(fac, b);
        // verify A*x ~= b
        for (int i = 0; i < 3; ++i) {
            double s = 0;
            for (int j = 0; j < 3; ++j) s += A(i, j) * x[j];
            NM_CHECK_NEAR(s, b[i], 1e-10);
        }
    });

    run_or_todo("Cholesky 3x3 SPD: L*L^T = A", []() {
        Matrix A = make_spd_3x3();
        Matrix L = cholesky(A);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                double s = 0;
                for (int k = 0; k < 3; ++k) s += L(i, k) * L(j, k);
                NM_CHECK_NEAR(s, A(i, j), 1e-10);
            }
    });

    run_or_todo("QR 3x3: Q^T Q = I, Q*R = A", []() {
        Matrix A  = make_spd_3x3();
        auto   qr = qr_decompose(A);
        // Q^T Q = I
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                double s = 0;
                for (int k = 0; k < 3; ++k) s += qr.Q(k, i) * qr.Q(k, j);
                NM_CHECK_NEAR(s, (i == j) ? 1.0 : 0.0, 1e-10);
            }
        // Q*R = A
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                double s = 0;
                for (int k = 0; k < 3; ++k) s += qr.Q(i, k) * qr.R(k, j);
                NM_CHECK_NEAR(s, A(i, j), 1e-10);
            }
    });

    run_or_todo("Conjugate gradient SPD 3x3", []() {
        Matrix A = make_spd_3x3();
        Vector b = {7, 10, 10};
        Vector x = conjugate_gradient(A, b, 1e-12, 100);
        for (int i = 0; i < 3; ++i) {
            double s = 0;
            for (int j = 0; j < 3; ++j) s += A(i, j) * x[j];
            NM_CHECK_NEAR(s, b[i], 1e-8);
        }
    });

    return nm::testing::exit_code();
}
