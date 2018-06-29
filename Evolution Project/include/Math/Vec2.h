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

#ifndef REACTPHYSICS3D_Vec2_H
#define REACTPHYSICS3D_Vec2_H

// Libraries
#include "MyMath.h"
#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

/**
 * This class represents a 2D vector.
 */
struct Vec2 {

    public:

        // -------------------- Attributes -------------------- //

        /// Component x
        double x;

        /// Component y
        double y;

        // -------------------- Methods -------------------- //

        /// Constructor of the class VecD
        Vec2();

        /// Constructor with arguments
        Vec2(double newX, double newY);

        /// Copy-constructor
        Vec2(const Vec2& vector);

        /// Destructor
        virtual ~Vec2();

        /// Set all the values of the vector
        void setAllValues(double newX, double newY);

        /// Set the vector to zero
        void setToZero();

        /// Return the length of the vector
        double length() const;

        /// Return the square of the length of the vector
        double lengthSquare() const;

        /// Return the corresponding unit vector
        Vec2 getUnit() const;

        /// Return one unit orthogonal vector of the current vector
        Vec2 getOneUnitOrthogonalVector() const;

        /// Return true if the vector is unit and false otherwise
        bool isUnit() const;

        /// Return true if the current vector is the zero vector
        bool isZero() const;

        /// Dot product of two vectors
        double dot(const Vec2& vector) const;

        /// Normalize the vector
        void normalize();

        /// Return the corresponding absolute value vector
        Vec2 getAbsoluteVector() const;

        /// Return the axis with the minimal value
        int getMinAxis() const;

        /// Return the axis with the maximal value
        int getMaxAxis() const;

        /// Overloaded operator for the equality condition
        bool operator== (const Vec2& vector) const;

        /// Overloaded operator for the is different condition
        bool operator!= (const Vec2& vector) const;

        /// Overloaded operator for addition with assignment
        Vec2& operator+=(const Vec2& vector);

        /// Overloaded operator for substraction with assignment
        Vec2& operator-=(const Vec2& vector);

        /// Overloaded operator for multiplication with a number with assignment
        Vec2& operator*=(double number);

        /// Overloaded operator for division by a number with assignment
        Vec2& operator/=(double number);

        /// Overloaded operator for value access
        double& operator[] (int index);

        /// Overloaded operator for value access
        const double& operator[] (int index) const;

        /// Overloaded operator
        Vec2& operator=(const Vec2& vector);

        /// Overloaded less than operator for ordering to be used inside std::set for instance
        bool operator<(const Vec2& vector) const;

        /// Return a vector taking the minimum components of two vectors
        static Vec2 min(const Vec2& vector1, const Vec2& vector2);

        /// Return a vector taking the maximum components of two vectors
        static Vec2 max(const Vec2& vector1, const Vec2& vector2);

        /// Return the zero vector
        static Vec2 zero();

        // -------------------- Friends -------------------- //

        friend Vec2 operator+(const Vec2& vector1, const Vec2& Vec2);
        friend Vec2 operator-(const Vec2& vector1, const Vec2& Vec2);
        friend Vec2 operator-(const Vec2& vector);
        friend Vec2 operator*(const Vec2& vector, double number);
        friend Vec2 operator*(double number, const Vec2& vector);
        friend Vec2 operator*(const Vec2& vector1, const Vec2& Vec2);
        friend Vec2 operator/(const Vec2& vector, double number);
        friend Vec2 operator/(const Vec2& vector1, const Vec2& Vec2);
        friend std::ostream &operator<<(std::ostream&, const Vec2&);
};

// Set the vector to zero
inline void Vec2::setToZero() {
    x = 0;
    y = 0;
}

// Set all the values of the vector
inline void Vec2::setAllValues(double newX, double newY) {
    x = newX;
    y = newY;
}

// Return the length of the vector
inline double Vec2::length() const {
    return sqrt(x*x + y*y);
}

// Return the square of the length of the vector
inline double Vec2::lengthSquare() const {
    return x*x + y*y;
}

// Scalar product of two vectors (inline)
inline double Vec2::dot(const Vec2& vector) const {
    return (x*vector.x + y*vector.y);
}

// Normalize the vector
inline void Vec2::normalize() {
    double l = length();
    if (l < MACHINE_EPSILON) {
        return;
    }
    x /= l;
    y /= l;
}

// Return the corresponding absolute value vector
inline Vec2 Vec2::getAbsoluteVector() const {
    return Vec2(std::abs(x), std::abs(y));
}

// Return the axis with the minimal value
inline int Vec2::getMinAxis() const {
    return (x < y ? 0 : 1);
}

// Return the axis with the maximal value
inline int Vec2::getMaxAxis() const {
    return (x < y ? 1 : 0);
}

// Return true if the vector is unit and false otherwise
inline bool Vec2::isUnit() const {
    return approxEqual(lengthSquare(), 1.0);
}

// Return true if the vector is the zero vector
inline bool Vec2::isZero() const {
    return approxEqual(lengthSquare(), 0.0);
}

// Overloaded operator for the equality condition
inline bool Vec2::operator== (const Vec2& vector) const {
    return approxEqual(x, vector.x) && approxEqual(y, vector.y);
}

// Overloaded operator for the is different condition
inline bool Vec2::operator!= (const Vec2& vector) const {
    return !(*this == vector);
}

// Overloaded operator for addition with assignment
inline Vec2& Vec2::operator+=(const Vec2& vector) {
    x += vector.x;
    y += vector.y;
    return *this;
}

// Overloaded operator for substraction with assignment
inline Vec2& Vec2::operator-=(const Vec2& vector) {
    x -= vector.x;
    y -= vector.y;
    return *this;
}

// Overloaded operator for multiplication with a number with assignment
inline Vec2& Vec2::operator*=(double number) {
    x *= number;
    y *= number;
    return *this;
}

// Overloaded operator for division by a number with assignment
inline Vec2& Vec2::operator/=(double number) {
    assert(number > std::numeric_limits<double>::epsilon());
    x /= number;
    y /= number;
    return *this;
}

// Overloaded operator for value access
inline double& Vec2::operator[] (int index) {
    return (&x)[index];
}

// Overloaded operator for value access
inline const double& Vec2::operator[] (int index) const {
    return (&x)[index];
}

// Overloaded operator for addition
inline Vec2 operator+(const Vec2& vector1, const Vec2& vector2) {
    return Vec2(vector1.x + vector2.x, vector1.y + vector2.y);
}

// Overloaded operator for substraction
inline Vec2 operator-(const Vec2& vector1, const Vec2& vector2) {
    return Vec2(vector1.x - vector2.x, vector1.y - vector2.y);
}

// Overloaded operator for the negative of a vector
inline Vec2 operator-(const Vec2& vector) {
    return Vec2(-vector.x, -vector.y);
}

// Overloaded operator for multiplication with a number
inline Vec2 operator*(const Vec2& vector, double number) {
    return Vec2(number * vector.x, number * vector.y);
}

// Overloaded operator for multiplication of two vectors
inline Vec2 operator*(const Vec2& vector1, const Vec2& vector2) {
    return Vec2(vector1.x * vector2.x, vector1.y * vector2.y);
}

// Overloaded operator for division by a number
inline Vec2 operator/(const Vec2& vector, double number) {
    assert(number > MACHINE_EPSILON);
    return Vec2(vector.x / number, vector.y / number);
}

// Overload operator for division between two vectors
inline Vec2 operator/(const Vec2& vector1, const Vec2& vector2) {
    assert(vector2.x > MACHINE_EPSILON);
    assert(vector2.y > MACHINE_EPSILON);
    return Vec2(vector1.x / vector2.x, vector1.y / vector2.y);
}

// Overloaded operator for multiplication with a number
inline Vec2 operator*(double number, const Vec2& vector) {
    return vector * number;
}

// Assignment operator
inline Vec2& Vec2::operator=(const Vec2& vector) {
    if (&vector != this) {
        x = vector.x;
        y = vector.y;
    }
    return *this;
}

// Overloaded less than operator for ordering to be used inside std::set for instance
inline bool Vec2::operator<(const Vec2& vector) const {
    return approxEqual(x, vector.x) ? y < vector.y : x < vector.x;
}

// Return a vector taking the minimum components of two vectors
inline Vec2 Vec2::min(const Vec2& vector1, const Vec2& vector2) {
    return Vec2(std::min(vector1.x, vector2.x),
                   std::min(vector1.y, vector2.y));
}

// Return a vector taking the maximum components of two vectors
inline Vec2 Vec2::max(const Vec2& vector1, const Vec2& vector2) {
    return Vec2(std::max(vector1.x, vector2.x),
                   std::max(vector1.y, vector2.y));
}

// Return the zero vector
inline Vec2 Vec2::zero() {
    return Vec2(0, 0);
}

#endif
