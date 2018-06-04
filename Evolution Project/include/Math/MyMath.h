#ifndef MY_MATH_H
#define MY_MATH_H

#include <math.h>
#include "reactphysics3d.h"

#define PI 3.1415926535897932384626

typedef reactphysics3d::Matrix3x3 Matrix;
typedef reactphysics3d::Matrix2x2 Matrix2;
typedef reactphysics3d::Vector3 Vec;
typedef reactphysics3d::Vector2 Vec2;

double randf(const double upper);
double randf(const double lower, const double upper);
int randi(const int lower, const int upper);
int randi(const int upper);
int pmRandi(const int bounds);
double pmRandf(const double number);
int comb(const int n);

double euc(Vec a, Vec b);
double euc2D(Vec a, Vec b);

#endif // MY_MATH_H
