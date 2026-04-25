// §13 stretch — CRTP vs virtual dispatch.
//
// Until BisectionSolver::solve() (the virtual version) and CRTPBisection::solve()
// (already implemented in the header) both produce results, this test reports
// TODO. Once they do, it validates correctness AND prints a microbenchmark.

#include "patterns/crtp_root_finder.h"
#include "patterns/solvers.h"
#include "todo_helper.h"

#include <chrono>
#include <cmath>
#include <cstdio>

using nm::testing::run_or_todo;
using namespace nm::patterns;

namespace {
auto poly_f() {
    return [](double x) { return x * x - 2.0; };
}
}  // namespace

int main() {
    run_or_todo("CRTP and virtual produce the same root", []() {
        BisectionSolver virt(poly_f(), 0.0, 2.0);
        CRTPBisection   crtp(poly_f(), 0.0, 2.0);

        auto rv = virt.solve(1e-12, 200);
        auto rc = crtp.solve(1e-12, 200);

        NM_CHECK(rv.converged && rc.converged);
        NM_CHECK_NEAR(rv.root, rc.root, 1e-12);
    });

    run_or_todo("CRTP benchmark (informational)", []() {
        constexpr int N = 1000;
        const auto    f = poly_f();

        auto t0 = std::chrono::steady_clock::now();
        for (int i = 0; i < N; ++i) {
            BisectionSolver s(f, 0.0, 2.0);
            (void)s.solve(1e-10, 100);
        }
        auto t1 = std::chrono::steady_clock::now();

        for (int i = 0; i < N; ++i) {
            CRTPBisection s(f, 0.0, 2.0);
            (void)s.solve(1e-10, 100);
        }
        auto t2 = std::chrono::steady_clock::now();

        const auto v_us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
        const auto c_us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        std::printf("    virtual: %lld us  |  CRTP: %lld us  |  speedup: %.2fx\n", static_cast<long long>(v_us),
                    static_cast<long long>(c_us),
                    v_us > 0 ? static_cast<double>(v_us) / static_cast<double>(c_us) : 0.0);
        // We don't assert on the ratio — it depends on -O level and CPU.
    });

    return nm::testing::exit_code();
}
