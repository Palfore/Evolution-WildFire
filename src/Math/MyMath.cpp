#include "Math/MyMath.h"

#include <random>
#include <time.h>
#include <thread>

#include "Vec.h"
#include "Vec2.h"

double der(std::function<double(Vec)> f, Vec a, int i) { // f(x + h, y, z) - f(x - h, y, z) / h
    double h = 0.0001;
    Vec lp = Vec(a.x + (i==0)*h, a.y + (i==1)*h, a.z + (i==2)*h); // add/sub h to proper component
    Vec lm = Vec(a.x - (i==0)*h, a.y - (i==1)*h, a.z - (i==2)*h);
    return (f(lp) - f(lm)) / (2*h);
}

Vec normal(std::function<double(Vec)> surface, Vec r) {
    return Vec(-der(surface, Vec(r.x, r.y, r.z), 0), -der(surface, Vec(r.x, r.y, r.z), 1), 1).getUnit();
}

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

Vec RandfR2(const double rmin, const double rmax) {
    const double r = randf(rmin, rmax);
    const double theta = randf(0, 2*PI);
    return Vec(r*cos(theta), r*sin(theta), 0);
}

Vec RandfR3(const double rmin, const double rmax) {
    const double r = randf(rmin, rmax);
    const double theta = randf(0, 2*PI);
    const double phi = randf(0, 2*PI);
    const double sinPhi = sin(phi);
    return Vec(r*cos(theta)*sinPhi, r*sin(theta)*sinPhi, r*cos(phi));
}

double pmRandf(const double minimal, const double maximal) {
    const double bound = maximal - minimal;
    double x = pmRandf(bound);
    return x + sgn(x)*minimal;
}

int pmRandi(const int minimal, const int maximal) {
    const int bound = maximal - minimal;
    int x = pmRandi(bound);
    return x + sgn(x)*minimal;
}

bool randb(const double trueChance) {
    return randf(100) < trueChance;
}

int comb(int n) {
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

double euc(Vec2 a, Vec2 b) {
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double square = (x * x) + (y * y);
    return sqrtf(square);
}

double euc2D(Vec a, Vec b) { // Excludes z
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double square = (x * x) + (y * y);
    return sqrtf(square);
}
