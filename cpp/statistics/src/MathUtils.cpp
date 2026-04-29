#include "MathUtils.hpp"

#include <cmath>
#include <stdexcept>

namespace stats::math {

namespace {

constexpr double kPi      = 3.14159265358979323846;
constexpr double kSqrt2   = 1.41421356237309504880;
constexpr double kSqrt2Pi = 2.50662827463100050241;

// Acklam's rational approximation to the inverse standard normal CDF.
double acklam_inverse_normal(double p) {
    constexpr double a[6]   = {-3.969683028665376e+01, 2.209460984245205e+02,  -2.759285104469687e+02,
                               1.383577518672690e+02,  -3.066479806614716e+01, 2.506628277459239e+00};
    constexpr double b[5]   = {-5.447609879822406e+01, 1.615858368580409e+02, -1.556989798598866e+02,
                               6.680131188771972e+01, -1.328068155288572e+01};
    constexpr double c[6]   = {-7.784894002430293e-03, -3.223964580411365e-01, -2.400758277161838e+00,
                               -2.549732539343734e+00, 4.374664141464968e+00,  2.938163982698783e+00};
    constexpr double d[4]   = {7.784695709041462e-03, 3.224671290700398e-01, 2.445134137142996e+00,
                               3.754408661907416e+00};
    constexpr double p_low  = 0.02425;
    constexpr double p_high = 1.0 - p_low;

    if (p < p_low) {
        const double q = std::sqrt(-2.0 * std::log(p));
        return (((((c[0] * q + c[1]) * q + c[2]) * q + c[3]) * q + c[4]) * q + c[5]) /
               ((((d[0] * q + d[1]) * q + d[2]) * q + d[3]) * q + 1.0);
    }
    if (p <= p_high) {
        const double q = p - 0.5;
        const double r = q * q;
        return (((((a[0] * r + a[1]) * r + a[2]) * r + a[3]) * r + a[4]) * r + a[5]) * q /
               (((((b[0] * r + b[1]) * r + b[2]) * r + b[3]) * r + b[4]) * r + 1.0);
    }
    const double q = std::sqrt(-2.0 * std::log(1.0 - p));
    return -(((((c[0] * q + c[1]) * q + c[2]) * q + c[3]) * q + c[4]) * q + c[5]) /
           ((((d[0] * q + d[1]) * q + d[2]) * q + d[3]) * q + 1.0);
}

// Lentz's continued-fraction evaluation for the regularized incomplete beta.
double beta_continued_fraction(double a, double b, double x) {
    constexpr int    kMaxIter = 200;
    constexpr double kEps     = 3.0e-12;
    constexpr double kTiny    = 1.0e-300;

    const double qab = a + b;
    const double qap = a + 1.0;
    const double qam = a - 1.0;
    double       c   = 1.0;
    double       d   = 1.0 - qab * x / qap;
    if (std::fabs(d) < kTiny) d = kTiny;
    d        = 1.0 / d;
    double h = d;

    for (int m = 1; m <= kMaxIter; ++m) {
        const int m2 = 2 * m;
        double    aa = m * (b - m) * x / ((qam + m2) * (a + m2));
        d            = 1.0 + aa * d;
        if (std::fabs(d) < kTiny) d = kTiny;
        c = 1.0 + aa / c;
        if (std::fabs(c) < kTiny) c = kTiny;
        d = 1.0 / d;
        h *= d * c;

        aa = -(a + m) * (qab + m) * x / ((a + m2) * (qap + m2));
        d  = 1.0 + aa * d;
        if (std::fabs(d) < kTiny) d = kTiny;
        c = 1.0 + aa / c;
        if (std::fabs(c) < kTiny) c = kTiny;
        d                  = 1.0 / d;
        const double delta = d * c;
        h *= delta;
        if (std::fabs(delta - 1.0) < kEps) { return h; }
    }
    throw std::runtime_error("regularized_incomplete_beta: failed to converge");
}

}  // namespace

double normal_pdf(double x) { return std::exp(-0.5 * x * x) / kSqrt2Pi; }
double normal_cdf(double x) { return 0.5 * std::erfc(-x / kSqrt2); }

double normal_quantile(double p) {
    if (p <= 0.0 || p >= 1.0) throw std::invalid_argument("normal_quantile: p must be in (0, 1)");
    return acklam_inverse_normal(p);
}

double log_beta(double a, double b) { return std::lgamma(a) + std::lgamma(b) - std::lgamma(a + b); }

double regularized_incomplete_beta(double a, double b, double x) {
    if (x < 0.0 || x > 1.0) throw std::invalid_argument("regularized_incomplete_beta: x out of range");
    if (x == 0.0) return 0.0;
    if (x == 1.0) return 1.0;

    const double bt =
        std::exp(std::lgamma(a + b) - std::lgamma(a) - std::lgamma(b) + a * std::log(x) + b * std::log(1.0 - x));

    if (x < (a + 1.0) / (a + b + 2.0)) return bt * beta_continued_fraction(a, b, x) / a;
    return 1.0 - bt * beta_continued_fraction(b, a, 1.0 - x) / b;
}
double t_pdf(double t, double df) {
    const double c = std::exp(std::lgamma((df + 1.0) / 2.0) - std::lgamma(df / 2.0)) / std::sqrt(df * kPi);
    return c * std::pow(1.0 + t * t / df, -(df + 1.0) / 2.0);
}

double t_cdf(double t, double df) {
    if (df <= 0.0) throw std::invalid_argument("t_cdf: df must be positive");
    const double x    = df / (df + t * t);
    const double tail = 0.5 * regularized_incomplete_beta(df / 2.0, 0.5, x);
    return t >= 0.0 ? 1.0 - tail : tail;
}

double t_quantile(double p, double df) {
    if (p <= 0.0 || p >= 1.0) throw std::invalid_argument("t_quantile: p must be in (0, 1)");
    // Bisection on t_cdf. Bracket symmetrically around 0.
    double lo = -1.0e6;
    double hi = 1.0e6;
    for (int i = 0; i < 200; ++i) {
        const double mid = 0.5 * (lo + hi);
        if (t_cdf(mid, df) < p)
            lo = mid;
        else
            hi = mid;
        if (hi - lo < 1.0e-10) break;
    }
    return 0.5 * (lo + hi);
}

}  // namespace stats::math
