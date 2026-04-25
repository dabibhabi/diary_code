# `src/differentiation/` — numerical differentiation

Beyond plain central differences. Two methods, two very different ideas.

## Richardson extrapolation (`richardson.cpp`)

Central difference is $O(h^2)$:

$$
D_1(h) \;=\; \frac{f(x+h) - f(x-h)}{2h} \;=\; f'(x) + a_2 h^2 + a_4 h^4 + \cdots
$$

Richardson eliminates the leading $h^2$ term:

$$
D_2(h) \;=\; \frac{4\,D_1(h/2) - D_1(h)}{3} \;=\; f'(x) + b_4 h^4 + \cdots
$$

After $L$ levels, accuracy is $O(h^{2L})$. Same idea Romberg uses on
integrals — the diagonal of the table converges fastest.

### Modern C++17 sketch

```cpp
func richardson_derivative(func f, double h0, int levels) {
    return [f = std::move(f), h0, levels](double x) {
        std::vector<std::vector<double>> T(levels);
        for (int i = 0; i < levels; ++i) {
            T[i].resize(i + 1);
            const double h = h0 / (1 << i);
            T[i][0] = (f(x + h) - f(x - h)) / (2.0 * h);          // O(h^2)
            for (int j = 1; j <= i; ++j) {
                const double pow4 = static_cast<double>(1 << (2 * j));
                T[i][j] = (pow4 * T[i][j - 1] - T[i - 1][j - 1]) / (pow4 - 1.0);
            }
        }
        return T[levels - 1][levels - 1];
    };
}
```

Notes:
- **Lambda captures `f` by move** — avoids copying `std::function` into every call.
- **`1 << i`** for $2^i$, **`1 << (2*j)`** for $4^j$.
- Returns a callable, matching the rest of the library where derivatives are functions.

## Complex-step (`complex_step.cpp`)

Taylor expand $f$ around $x$ with imaginary step $ih$:

$$
f(x + ih) \;=\; f(x) + ih\,f'(x) - \tfrac{h^2}{2}f''(x) - i\tfrac{h^3}{6}f'''(x) + \cdots
$$

Take the imaginary part and divide by $h$:

$$
\frac{\Im\big(f(x+ih)\big)}{h} \;=\; f'(x) - \tfrac{h^2}{6}f'''(x) + \cdots
$$

The leading error is $O(h^2)$ but **there's no subtractive cancellation**.
Set $h \approx 10^{-20}$ and you get derivative accuracy near machine epsilon.

### Modern C++ sketch

```cpp
double complex_step_derivative(
    std::function<std::complex<double>(std::complex<double>)> f,
    double x, double h) {
    using namespace std::complex_literals;            // C++14 i suffix
    const auto z = std::complex<double>{x, h};
    return f(z).imag() / h;
}
```

The trick: **`f` must be holomorphic**, i.e. expressible without `abs`,
`max`, conjugation. If your pricer is templated, it works automatically. If
it isn't, you're stuck with finite differences.

**Quant**: clean Greeks. Set up a Black-Scholes pricer with templated value
type; one rebuild and `complex_step_derivative` gives you delta to 14
digits, no finite-difference noise.
