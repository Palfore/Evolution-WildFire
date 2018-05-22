#include "Math/MyMath.h"

#include <random>
#include <time.h>
#include <thread>

double randf(const double lower, const double upper) { // thread safe
    std::hash<std::thread::id> hasher;
    static thread_local std::mt19937* generator = new std::mt19937(std::clock() + hasher(std::this_thread::get_id()));
    std::uniform_real_distribution<double> distribution(lower, upper);
    return distribution(*generator);
}


// Random float [0, number]
double randf(const double upper) {
    return randf(0, upper);
}

// Ramdom float [-number, +number]
double pmRandf(double bounds) {
    return 2 * (randf(bounds) - (0.5 * bounds));
}

int comb(int n) {
    return 0.5 * n * (n - 1);
}


double euc(Vec a, Vec b) {
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double z = (a.z - b.z);
    double square = (x * x) + (y * y) + (z * z);
    return sqrtf(square);
}

double euc2D(Vec a, Vec b) { // Excludes z
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double square = (x * x) + (y * y);
    return sqrtf(square);
}
