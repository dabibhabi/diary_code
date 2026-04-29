#pragma once

namespace stats::math {

double normal_pdf(double x);
double normal_cdf(double x);
double normal_quantile(double p);

double t_pdf(double t, double df);
double t_cdf(double t, double df);
double t_quantile(double p, double df);

double log_beta(double a, double b);
double regularized_incomplete_beta(double a, double b, double x);

}  // namespace stats::math
