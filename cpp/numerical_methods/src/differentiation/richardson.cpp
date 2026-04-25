#include "differentiation.h"
#include "todo.h"

namespace nm::differentiation {

// TODO: Richardson extrapolation on central differences.
//   D(h) = (f(x+h) - f(x-h)) / (2h)        // O(h^2)
//   Build a Romberg-style triangle eliminating O(h^2), O(h^4), ... terms.
// LANGUAGE CHOICE: pure C++ — small, scalar work, std::function is fine.
// References: Wikipedia "Richardson_extrapolation"; NR3 §5.7.
func richardson_derivative(func /*f*/, double /*h0*/, int /*levels*/) {
    NM_TODO("differentiation::richardson_derivative");
}

}  // namespace nm::differentiation
