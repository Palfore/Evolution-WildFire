#include "Bisection.h"

#include <iostream>
double findRoot(std::function<double(double)> f, double a, double b, double eps) {
 	auto f_a = f(a);
    while (a + eps < b) {
        auto const mid = 0.5 * a + 0.5 * b;
        auto const f_mid = f(mid);

        if ((f_a < 0) == (f_mid < 0)) {
            a = mid;
            f_a = f_mid;
        } else {
            b = mid;
        }
    }
    return a;
}
