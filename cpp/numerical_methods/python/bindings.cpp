// Pybind11 bindings (Adapter pattern, footnoted in docs/PATTERNS.md).
//
// This module is the only place where Python types touch C++ types. Tests
// in tests/python/ talk to nmpy as if it were native Python.

#include "linalg.h"
#include "numerical_methods.h"
#include "ode.h"
#include "optimization.h"
#include "patterns/convergence_observer.h"
#include "patterns/iterative_root_finder.h"
#include "patterns/mc_decorators.h"
#include "patterns/mc_engine_builder.h"
#include "patterns/solver_factory.h"
#include "patterns/solvers.h"
#include "quadrature.h"
#include "random_mc.h"
#include "todo.h"
#include "utils.h"

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace nm;
using namespace nm::patterns;

PYBIND11_MODULE(nmpy, m) {
    m.doc() = "Numerical methods C++ library — Python bindings";

    // Map nm::not_implemented to NotImplementedError.
    py::register_exception<not_implemented>(m, "NotImplementedYet", PyExc_NotImplementedError);

    // ---- Result types ----
    py::class_<ConvergenceResult>(m, "ConvergenceResult")
        .def_readonly("root", &ConvergenceResult::root)
        .def_readonly("iterations", &ConvergenceResult::iterations)
        .def_readonly("converged", &ConvergenceResult::converged)
        .def_readonly("errors", &ConvergenceResult::errors);
    m.def("estimate_order", &estimate_order, py::arg("errors"));

    // ---- Solver enum ----
    py::enum_<SolverKind>(m, "SolverKind")
        .value("BISECTION", SolverKind::BISECTION)
        .value("NEWTON", SolverKind::NEWTON)
        .value("SECANT", SolverKind::SECANT)
        .value("BRENT", SolverKind::BRENT)
        .value("HALLEY", SolverKind::HALLEY)
        .value("RIDDERS", SolverKind::RIDDERS)
        .value("MULLER", SolverKind::MULLER)
        .value("CHANDRUPATLA", SolverKind::CHANDRUPATLA);

    // ---- IRootFinder + Solver classes ----
    py::class_<IRootFinder, std::shared_ptr<IRootFinder>>(m, "IRootFinder")
        .def("solve", &IRootFinder::solve, py::arg("tol") = 1e-10, py::arg("max_iter") = 200)
        .def("attach", &IRootFinder::attach)
        .def("clear_observers", &IRootFinder::clear_observers);

    // ---- Observers ----
    py::class_<IConvergenceObserver, std::shared_ptr<IConvergenceObserver>>(m, "IConvergenceObserver");
    py::class_<LoggingObserver, IConvergenceObserver, std::shared_ptr<LoggingObserver>>(m, "LoggingObserver")
        .def(py::init<>());
    py::class_<CapturingObserver, IConvergenceObserver, std::shared_ptr<CapturingObserver>>(m, "CapturingObserver")
        .def(py::init<>())
        .def_property_readonly("events", [](const CapturingObserver& c) {
            std::vector<std::tuple<int, double, double, double>> out;
            for (const auto& e : c.events()) out.emplace_back(e.iter, e.x, e.abs_error, e.f_value);
            return out;
        });
    py::class_<EarlyStopObserver, IConvergenceObserver, std::shared_ptr<EarlyStopObserver>>(m, "EarlyStopObserver")
        .def(py::init<double>(), py::arg("threshold"));

    // ---- Factory ----
    py::class_<SolverConfig>(m, "SolverConfig")
        .def(py::init<>())
        .def_readwrite("f", &SolverConfig::f)
        .def_readwrite("df", &SolverConfig::df)
        .def_readwrite("d2f", &SolverConfig::d2f)
        .def_readwrite("a", &SolverConfig::a)
        .def_readwrite("b", &SolverConfig::b)
        .def_readwrite("x0", &SolverConfig::x0)
        .def_readwrite("x1", &SolverConfig::x1)
        .def_readwrite("x2", &SolverConfig::x2);
    m.def("create_solver", &SolverFactory::create, py::arg("kind"), py::arg("cfg"));

    // ---- MC builder + decorators ----
    py::class_<IMCEstimator, std::shared_ptr<IMCEstimator>>(m, "IMCEstimator")
        .def("estimate", &IMCEstimator::estimate, py::arg("n_paths"), py::arg("seed"));
    py::class_<MonteCarloEngineBuilder>(m, "MonteCarloEngineBuilder")
        .def(py::init<>())
        .def("with_payoff", &MonteCarloEngineBuilder::with_payoff, py::return_value_policy::reference)
        .def("with_paths", &MonteCarloEngineBuilder::with_paths, py::return_value_policy::reference)
        .def("with_seed", &MonteCarloEngineBuilder::with_seed, py::return_value_policy::reference)
        .def("with_antithetic", &MonteCarloEngineBuilder::with_antithetic, py::return_value_policy::reference)
        .def("with_control_variate", &MonteCarloEngineBuilder::with_control_variate, py::return_value_policy::reference)
        .def("build", &MonteCarloEngineBuilder::build);

    // ---- Utils ----
    m.def("derivative", &utils::derivative, py::arg("f"), py::arg("h") = 1e-6);
    m.def("integrate", &utils::integrate, py::arg("f"), py::arg("a"), py::arg("b"), py::arg("n") = 1000);

    // ---- Submodules for the rest of the library (still working as before) ----
    auto q = m.def_submodule("quadrature");
    q.def("romberg", &quadrature::romberg);
    q.def("adaptive_simpson", &quadrature::adaptive_simpson);
    q.def("gauss_legendre", &quadrature::gauss_legendre);
    q.def("gauss_hermite", &quadrature::gauss_hermite);
    q.def("tanh_sinh", &quadrature::tanh_sinh);

    auto ode_m = m.def_submodule("ode");
    py::class_<ode::Trajectory>(ode_m, "Trajectory")
        .def_readonly("t", &ode::Trajectory::t)
        .def_readonly("y", &ode::Trajectory::y);
    ode_m.def("euler", &ode::euler);
    ode_m.def("rk4", &ode::rk4);
    ode_m.def("rk45", &ode::rk45);

    auto la = m.def_submodule("linalg");
    py::class_<linalg::Matrix>(la, "Matrix")
        .def(py::init<int, int>())
        .def_readonly("rows", &linalg::Matrix::rows)
        .def_readonly("cols", &linalg::Matrix::cols)
        .def("__getitem__", [](const linalg::Matrix& A, std::pair<int, int> ij) { return A(ij.first, ij.second); })
        .def("__setitem__", [](linalg::Matrix& A, std::pair<int, int> ij, double v) { A(ij.first, ij.second) = v; });
    la.def("cholesky", &linalg::cholesky);
    la.def("conjugate_gradient", &linalg::conjugate_gradient);

    auto opt = m.def_submodule("optimization");
    py::class_<optimization::OptResult>(opt, "OptResult")
        .def_readonly("x", &optimization::OptResult::x)
        .def_readonly("f_value", &optimization::OptResult::f_value)
        .def_readonly("iterations", &optimization::OptResult::iterations)
        .def_readonly("converged", &optimization::OptResult::converged);
    opt.def("golden_section", &optimization::golden_section);
    opt.def("brent_min", &optimization::brent_min);
    opt.def("nelder_mead", &optimization::nelder_mead);
    opt.def("levenberg_marquardt", &optimization::levenberg_marquardt);

    auto rmc = m.def_submodule("random_mc");
    py::class_<random_mc::NormalPair>(rmc, "NormalPair")
        .def_readonly("z1", &random_mc::NormalPair::z1)
        .def_readonly("z2", &random_mc::NormalPair::z2);
    rmc.def("box_muller", &random_mc::box_muller);
    rmc.def("inv_normal_cdf", &random_mc::inv_normal_cdf);
    rmc.def("sobol_1d", &random_mc::sobol_1d);
}
