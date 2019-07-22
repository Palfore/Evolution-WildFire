#ifndef BISECTION_H
#define BISECTION_H

#include <functional>

double findRoot(std::function<double(double)> f, double a, double b, double eps=1e-4);

#endif // BISECTION_H
