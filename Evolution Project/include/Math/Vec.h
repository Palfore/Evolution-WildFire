/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2016 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_Vec_H
#define REACTPHYSICS3D_Vec_H

// Libraries
#include "MyMath.h"
#include <cmath>
#include <cassert>
#include <algorithm>

// Class Vec
/**
 * This class represents a 3D vector.
 */
struct Vec {

    public:

        // -------------------- Attributes -------------------- //

        /// Component x
        double x;

        /// Component y
        double y;

        /// Component z
        double z;

        // -------------------- Methods -------------------- //

        /// Constructor of the class VecD
        Vec();

        /// Constructor with arguments
        Vec(double newX, double newY, double newZ);

        /// Copy-constructor
        Vec(const Vec& vector);

        /// Destructor
        virtual ~Vec();

        /// Set all the values of the vector
        void setAllValues(double newX, double newY, double newZ);

        /// Set the vector to zero
        void setToZero();

        /// Return the length of the vector
        double length() const;

        /// Return the square of the length of the vector
        double lengthSquare() const;

        /// Return the corresponding unit vector
        Vec getUnit() const;

        /// Return one unit orthogonal vector of the current vector
        Vec getOneUnitOrthogonalVector() const;

        /// Return true if the vector is unit and false otherwise
        bool isUnit() const;

        /// Return true if the current vector is the zero vector
        bool isZero() const;

        /// Dot product of two vectors
        double dot(const Vec& vector) const;

        /// Cross product of two vectors
        Vec cross(const Vec& vector) const;

        /// Normalize the vector
        void normalize();

        /// Return the corresponding absolute value vector
        Vec getAbsoluteVector() const;

        /// Return the axis with the minimal value
        int getMinAxis() const;

        /// Return the axis with the maximal value
        int getMaxAxis() const;

        /// Return the minimum value among the three components of a vector
        double getMinValue() const;

        /// Return the maximum value among the three components of a vector
        double getMaxValue() const;

        /// Overloaded operator for the equality condition
        bool operator== (const Vec& vector) const;

        /// Overloaded operator for the is different condition
        bool operator!= (const Vec& vector) const;

        /// Overloaded operator for addition with assignment
        Vec& operator+=(const Vec& vector);

        /// Overloaded operator for substraction with assignment
        Vec& operator-=(const Vec& vector);

        /// Overloaded operator for multiplication with a number with assignment
        Vec& operator*=(double number);

        /// Overloaded operator for division by a number with assignment
        Vec& operator/=(double number);

        /// Overloaded operator for value access
        double& operator[] (int index);

        /// Overloaded operator for value access
        const double& operator[] (int index) const;

        /// Overloaded operator
        Vec& operator=(const Vec& vector);

        /// Overloaded less than operator for ordering to be used inside std::set for instance
        bool operator<(const Vec& vector) const;

        /// Return a vector taking the minimum components of two vectors
        static Vec min(const Vec& vector1, const Vec& Vec2);

        /// Return a vector taking the maximum components of two vectors
        static Vec max(const Vec& vector1, const Vec& Vec2);

        /// Return the zero vector
        static Vec zero();

        // -------------------- Friends -------------------- //

        friend Vec operator+(const Vec& vector1, const Vec& Vec2);
        friend Vec operator-(const Vec& vector1, const Vec& Vec2);
        friend Vec operator-(const Vec& vector);
        friend Vec operator*(const Vec& vector, double number);
        friend Vec operator*(double number, const Vec& vector);
        friend Vec operator*(const Vec& vector1, const Vec& Vec2);
        friend Vec operator/(const Vec& vector, double number);
        friend Vec operator/(const Vec& vector1, const Vec& Vec2);
};

// Set the vector to zero
inline void Vec::setToZero() {
    x = 0;
    y = 0;
    z = 0;
}

// Set all the values of the vector
inline void Vec::setAllValues(double newX, double newY, double newZ) {
    x = newX;
    y = newY;
    z = newZ;
}

// Return the length of the vector
inline double Vec::length() const {
    return sqrt(x*x + y*y + z*z);
}

// Return the square of the length of the vector
inline double Vec::lengthSquare() const {
    return x*x + y*y + z*z;
}

// Scalar product of two vectors (inline)
inline double Vec::dot(const Vec& vector) const {
    return (x*vector.x + y*vector.y + z*vector.z);
}

// Cross product of two vectors (inline)
inline Vec Vec::cross(const Vec& vector) const {
    return Vec(y * vector.z - z * vector.y,
                   z * vector.x - x * vector.z,
                   x * vector.y - y * vector.x);
}

// Normalize the vector
inline void Vec::normalize() {
    double l = length();
    if (l < MACHINE_EPSILON) {
        return;
    }
    x /= l;
    y /= l;
    z /= l;
}

// Return the corresponding absolute value vector
inline Vec Vec::getAbsoluteVector() const {
    return Vec(std::abs(x), std::abs(y), std::abs(z));
}

// Return the axis with the minimal value
inline int Vec::getMinAxis() const {
    return (x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2));
}

// Return the axis with the maximal value
inline int Vec::getMaxAxis() const {
    return (x < y ? (y < z ? 2 : 1) : (x < z ? 2 : 0));
}

// Return true if the vector is unit and false otherwise
inline bool Vec::isUnit() const {
    return approxEqual(lengthSquare(), 1.0);
}

// Return true if the vector is the zero vector
inline bool Vec::isZero() const {
    return approxEqual(lengthSquare(), 0.0);
}

// Overloaded operator for the equality condition
inline bool Vec::operator== (const Vec& vector) const {
    return approxEqual(x, vector.x) && approxEqual(y, vector.y) && approxEqual(z, vector.z);
}

// Overloaded operator for the is different condition
inline bool Vec::operator!= (const Vec& vector) const {
    return !(*this == vector);
}

// Overloaded operator for addition with assignment
inline Vec& Vec::operator+=(const Vec& vector) {
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

// Overloaded operator for substraction with assignment
inline Vec& Vec::operator-=(const Vec& vector) {
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

// Overloaded operator for multiplication with a number with assignment
inline Vec& Vec::operator*=(double number) {
    x *= number;
    y *= number;
    z *= number;
    return *this;
}

// Overloaded operator for division by a number with assignment
inline Vec& Vec::operator/=(double number) {
    assert(number > std::numeric_limits<double>::epsilon());
    x /= number;
    y /= number;
    z /= number;
    return *this;
}

// Overloaded operator for value access
inline double& Vec::operator[] (int index) {
    return (&x)[index];
}

// Overloaded operator for value access
inline const double& Vec::operator[] (int index) const {
    return (&x)[index];
}

// Overloaded operator for addition
inline Vec operator+(const Vec& vector1, const Vec& Vec2) {
    return Vec(vector1.x + Vec2.x, vector1.y + Vec2.y, vector1.z + Vec2.z);
}

// Overloaded operator for substraction
inline Vec operator-(const Vec& vector1, const Vec& Vec2) {
    return Vec(vector1.x - Vec2.x, vector1.y - Vec2.y, vector1.z - Vec2.z);
}

// Overloaded operator for the negative of a vector
inline Vec operator-(const Vec& vector) {
    return Vec(-vector.x, -vector.y, -vector.z);
}

// Overloaded operator for multiplication with a number
inline Vec operator*(const Vec& vector, double number) {
    return Vec(number * vector.x, number * vector.y, number * vector.z);
}

// Overloaded operator for division by a number
inline Vec operator/(const Vec& vector, double number) {
    assert(number > MACHINE_EPSILON);
    return Vec(vector.x / number, vector.y / number, vector.z / number);
}

// Overload operator for division between two vectors
inline Vec operator/(const Vec& vector1, const Vec& Vec2) {
    assert(Vec2.x > MACHINE_EPSILON);
    assert(Vec2.y > MACHINE_EPSILON);
    assert(Vec2.z > MACHINE_EPSILON);
    return Vec(vector1.x / Vec2.x, vector1.y / Vec2.y, vector1.z / Vec2.z);
}

// Overloaded operator for multiplication with a number
inline Vec operator*(double number, const Vec& vector) {
    return vector * number;
}

// Overload operator for multiplication between two vectors
inline Vec operator*(const Vec& vector1, const Vec& Vec2) {
    return Vec(vector1.x * Vec2.x, vector1.y * Vec2.y, vector1.z * Vec2.z);
}

// Assignment operator
inline Vec& Vec::operator=(const Vec& vector) {
    if (&vector != this) {
        x = vector.x;
        y = vector.y;
        z = vector.z;
    }
    return *this;
}

// Overloaded less than operator for ordering to be used inside std::set for instance
inline bool Vec::operator<(const Vec& vector) const {
    return (approxEqual(x, vector.x) ? (approxEqual(y, vector.y) ? z < vector.z : y < vector.y) : x < vector.x);
}

// Return a vector taking the minimum components of two vectors
inline Vec Vec::min(const Vec& vector1, const Vec& vector2) {
    return Vec(std::min(vector1.x, vector2.x),
                   std::min(vector1.y, vector2.y),
                   std::min(vector1.z, vector2.z));
}

// Return a vector taking the maximum components of two vectors
inline Vec Vec::max(const Vec& vector1, const Vec& vector2) {
    return Vec(std::max(vector1.x, vector2.x),
                   std::max(vector1.y, vector2.y),
                   std::max(vector1.z, vector2.z));
}

// Return the minimum value among the three components of a vector
inline double Vec::getMinValue() const {
    return std::min(std::min(x, y), z);
}

// Return the maximum value among the three components of a vector
inline double Vec::getMaxValue() const {
    return std::max(std::max(x, y), z);
}

// Return the zero vector
inline Vec Vec::zero() {
    return Vec(0, 0, 0);
}


#endif
