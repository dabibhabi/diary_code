#include "differentiation.h"
#include "todo.h"

namespace nm::differentiation {

// TODO: Complex-step derivative.
//   Return Im(f(x + i*h)) / h. With h ~ 1e-20, no subtractive cancellation.
// LANGUAGE CHOICE: C++ — needs std::complex<double>. Same idea works in Python
//   (just `complex`) but C++ wins when the underlying pricer is templated and
//   one rebuild gives you Greeks "for free".
// References: Squire & Trapp 1998.
double complex_step_derivative(std::function<std::complex<double>(std::complex<double>)> /*f*/, double /*x*/,
                               double /*h*/) {
    NM_TODO("differentiation::complex_step_derivative");
}

}  // namespace nm::differentiation
