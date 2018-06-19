#include "Math/MyMath.h"

#include <random>
#include <time.h>
#include <thread>

#include "Vec.h"



double randf(const double lower, const double upper) { // thread safe
    std::hash<std::thread::id> hasher;
    static thread_local std::mt19937* generator = new std::mt19937(std::clock() + hasher(std::this_thread::get_id()));
    std::uniform_real_distribution<double> distribution(lower, upper);
    return distribution(*generator);
}

int randi(const int lower, const int upper) { // thread safe
    std::hash<std::thread::id> hasher;
    static thread_local std::mt19937* generator = new std::mt19937(std::clock() + hasher(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(*generator);
}

// Random int [0, number]
int randi(const int upper) {
    return randi(0, upper);
}

// Random int [-number, +number]
int pmRandi(const int bounds) {
    return 2 * (randi(bounds) - (0.5 * bounds));
}

// Random float [0, number]
double randf(const double upper) {
    return randf(0, upper);
}

// Random float [-number, +number]
double pmRandf(const double bounds) {
    return 2 * (randf(bounds) - (0.5 * bounds));
}

double pmRandf(const double minimal, const double maximal) {
    return 0; ///< @todo implement pmRandf(min, max)
}

int pmRandi(const int minimal, const int maximal) {
    return 0;///< @todo implement pmRandi(min, max)
}

int comb(const int n) {
    return 0.5 * n * (n - 1);
}

bool approxEqual(double a, double b, double epsilon) {
    return (std::fabs(a - b) < epsilon);
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
