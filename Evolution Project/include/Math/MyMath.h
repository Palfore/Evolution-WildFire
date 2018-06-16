#ifndef MY_MATH_H
#define MY_MATH_H

#include <cmath>
#include <limits>

constexpr double PI = 3.1415926535897932384626;
constexpr double MACHINE_EPSILON = std::numeric_limits<double>::epsilon();

class Vec;

/** @brief Returns a thread-safe double in the range [lower, upper).
 * @param lower The lower-bound of numbers generated.
 * @param upper The upper-boundof numbers generated.
 * @return double A random number [lower, upper).
 */
double randf(const double lower, const double upper);

/** @brief Returns a thread-safe double in the range [0, upper).
 * @param upper The upper-boundof numbers generated.
 * @return double A random number [0, upper).
 */
double randf(const double upper);

/** @brief Returns a thread-safe int in the range [lower, upper).
 * @param lower The lower-bound of numbers generated.
 * @param upper The upper-boundof numbers generated.
 * @warning Unknown if range is inclusive.
 * @todo Determine if range is inclusive.
 * @return int A random number [lower, upper).
 */
int randi(const int lower, const int upper);

/** @brief Returns a thread-safe int in the range [0, upper).
 * @param upper The upper-boundof numbers generated.
 * @warning Unknown if range is inclusive.
 * @todo Determine if range is inclusive.
 * @return int A random number [0, upper).
 */
int randi(const int upper);

/** @brief Returns a thread-safe double in the range [-bounds, bounds).
 * @param bounds The bounds of numbers generated.
 * @warning Unknown if range is inclusive.
 * @todo Determine if range is inclusive.
 * @return double A random number [-bounds, bounds).
 */
double pmRandf(const double bounds);

/** @brief Returns a thread-safe int in the range [-bounds, bounds).
 * @param bounds The bounds of numbers generated.
 * @warning Unknown if range is inclusive.
 * @todo Determine if range is inclusive.
 * @return int A random number [-bounds, bounds).
 */
int pmRandi(const int bounds);

/** @brief Returns the number of connections that can be made with n nodes.
 * @details This permutation calculation simplfies and is returned as \f$ \frac{1}{2}n(n - 1) \f$ .
 * @param n The number of nodes.
 * @return int The number of combinations.
 *
 */
int comb(const int n);

/** @brief Returns the Euclidean distance between two vectors.
 * @details This is computed as \f$ d = \sqrt{ (a.x - b.x)^2 + (a.y - b.y)^2 + (a.z - b.z)^2 } \f$ .
 * @param a One of the vectors.
 * @param b Another vector.
 * @return double The Euclidean distance.
 *
 */
double euc(Vec a, Vec b);

/** @brief Returns the Euclidean distance - ignoring the z componenet - between two vectors.
 * @details This is computed as \f$ d = \sqrt{ (a.x - b.x)^2 + (a.y - b.y)^2} \f$ .
 * @param a One of the vectors.
 * @param b Another vector.
 * @return double The planar-Euclidean distance.
 *
 */
double euc2D(Vec a, Vec b);

bool approxEqual(double a, double b, double epsilon = MACHINE_EPSILON);

#endif // MY_MATH_H
