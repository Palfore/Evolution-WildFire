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
double pmRandf(double number);
int comb(int n);

double euc(Vec a, Vec b);
double euc2D(Vec a, Vec b);

#endif // MY_MATH_H
