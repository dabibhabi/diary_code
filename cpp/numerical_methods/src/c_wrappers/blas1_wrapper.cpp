// Currently no C++-side wrapper functions are required for BLAS-1 — callers
// can use c_api/blas1.h directly. This TU exists so CMake's GLOB picks up a
// non-empty c_wrappers/ and so future C++-side helpers (e.g. a vector-aware
// overload) have a home.
//
// Suggested expansion: a small `Vector saxpy(double a, const Vector& x, Vector y)`
// helper that calls c_api::nm_saxpy on the underlying buffers.

namespace nm::linalg {

// (intentionally empty — see comment above)

}  // namespace nm::linalg
