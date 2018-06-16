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

#ifndef REACTPHYSICS3D_Matrix2_H
#define REACTPHYSICS3D_Matrix2_H

// Libraries
#include <cassert>
#include "Vec2.h"

/**
 * This class represents a 2x2 matrix.
 */
class Matrix2 {

    private :

        // -------------------- Attributes -------------------- //

        /// Rows of the matrix;
        Vec2 mRows[2];

    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        Matrix2();

        /// Constructor
        Matrix2(double value);

        /// Constructor
        Matrix2(double a1, double a2, double b1, double b2);

        /// Destructor
        ~Matrix2();

        /// Copy-constructor
        Matrix2(const Matrix2& matrix);

        /// Assignment operator
        Matrix2& operator=(const Matrix2& matrix);

        /// Set all the values in the matrix
        void setAllValues(double a1, double a2, double b1, double b2);

        /// Set the matrix to zero
        void setToZero();

        /// Return a column
        Vec2 getColumn(int i) const;

        /// Return a row
        Vec2 getRow(int i) const;

        /// Return the transpose matrix
        Matrix2 getTranspose() const;

        /// Return the determinant of the matrix
        double getDeterminant() const;

        /// Return the trace of the matrix
        double getTrace() const;

        /// Return the inverse matrix
        Matrix2 getInverse() const;

        /// Return the matrix with absolute values
        Matrix2 getAbsoluteMatrix() const;

        /// Set the matrix to the identity matrix
        void setToIdentity();

        /// Return the 2x2 identity matrix
        static Matrix2 identity();

        /// Return the 2x2 zero matrix
        static Matrix2 zero();

        /// Overloaded operator for addition
        friend Matrix2 operator+(const Matrix2& matrix1, const Matrix2& matrix2);

        /// Overloaded operator for substraction
        friend Matrix2 operator-(const Matrix2& matrix1, const Matrix2& matrix2);

        /// Overloaded operator for the negative of the matrix
        friend Matrix2 operator-(const Matrix2& matrix);

        /// Overloaded operator for multiplication with a number
        friend Matrix2 operator*(double nb, const Matrix2& matrix);

        /// Overloaded operator for multiplication with a matrix
        friend Matrix2 operator*(const Matrix2& matrix, double nb);

        /// Overloaded operator for matrix multiplication
        friend Matrix2 operator*(const Matrix2& matrix1, const Matrix2& matrix2);

        /// Overloaded operator for multiplication with a vector
        friend Vec2 operator*(const Matrix2& matrix, const Vec2& vector);

        /// Overloaded operator for equality condition
        bool operator==(const Matrix2& matrix) const;

        /// Overloaded operator for the is different condition
        bool operator!= (const Matrix2& matrix) const;

        /// Overloaded operator for addition with assignment
        Matrix2& operator+=(const Matrix2& matrix);

        /// Overloaded operator for substraction with assignment
        Matrix2& operator-=(const Matrix2& matrix);

        /// Overloaded operator for multiplication with a number with assignment
        Matrix2& operator*=(double nb);

        /// Overloaded operator to read element of the matrix.
        const Vec2& operator[](int row) const;

        /// Overloaded operator to read/write element of the matrix.
        Vec2& operator[](int row);
};

// Method to set all the values in the matrix
inline void Matrix2::setAllValues(double a1, double a2,
                                    double b1, double b2) {
    mRows[0][0] = a1; mRows[0][1] = a2;
    mRows[1][0] = b1; mRows[1][1] = b2;
}

// Set the matrix to zero
inline void Matrix2::setToZero() {
    mRows[0].setToZero();
    mRows[1].setToZero();
}

// Return a column
inline Vec2 Matrix2::getColumn(int i) const {
    assert(i>= 0 && i<2);
    return Vec2(mRows[0][i], mRows[1][i]);
}

// Return a row
inline Vec2 Matrix2::getRow(int i) const {
    assert(i>= 0 && i<2);
    return mRows[i];
}

// Return the transpose matrix
inline Matrix2 Matrix2::getTranspose() const {

    // Return the transpose matrix
    return Matrix2(mRows[0][0], mRows[1][0],
                     mRows[0][1], mRows[1][1]);
}

// Return the determinant of the matrix
inline double Matrix2::getDeterminant() const {

    // Compute and return the determinant of the matrix
    return mRows[0][0] * mRows[1][1] - mRows[1][0] * mRows[0][1];
}

// Return the trace of the matrix
inline double Matrix2::getTrace() const {

    // Compute and return the trace
    return (mRows[0][0] + mRows[1][1]);
}

// Set the matrix to the identity matrix
inline void Matrix2::setToIdentity() {
    mRows[0][0] = 1.0; mRows[0][1] = 0.0;
    mRows[1][0] = 0.0; mRows[1][1] = 1.0;
}

// Return the 2x2 identity matrix
inline Matrix2 Matrix2::identity() {

    // Return the isdentity matrix
    return Matrix2(1.0, 0.0, 0.0, 1.0);
}

// Return the 2x2 zero matrix
inline Matrix2 Matrix2::zero() {
    return Matrix2(0.0, 0.0, 0.0, 0.0);
}

// Return the matrix with absolute values
inline Matrix2 Matrix2::getAbsoluteMatrix() const {
    return Matrix2(fabs(mRows[0][0]), fabs(mRows[0][1]),
                     fabs(mRows[1][0]), fabs(mRows[1][1]));
}

// Overloaded operator for addition
inline Matrix2 operator+(const Matrix2& matrix1, const Matrix2& matrix2) {
    return Matrix2(matrix1.mRows[0][0] + matrix2.mRows[0][0],
                     matrix1.mRows[0][1] + matrix2.mRows[0][1],
                     matrix1.mRows[1][0] + matrix2.mRows[1][0],
                     matrix1.mRows[1][1] + matrix2.mRows[1][1]);
}

// Overloaded operator for substraction
inline Matrix2 operator-(const Matrix2& matrix1, const Matrix2& matrix2) {
    return Matrix2(matrix1.mRows[0][0] - matrix2.mRows[0][0],
                     matrix1.mRows[0][1] - matrix2.mRows[0][1],
                     matrix1.mRows[1][0] - matrix2.mRows[1][0],
                     matrix1.mRows[1][1] - matrix2.mRows[1][1]);
}

// Overloaded operator for the negative of the matrix
inline Matrix2 operator-(const Matrix2& matrix) {
    return Matrix2(-matrix.mRows[0][0], -matrix.mRows[0][1],
                     -matrix.mRows[1][0], -matrix.mRows[1][1]);
}

// Overloaded operator for multiplication with a number
inline Matrix2 operator*(double nb, const Matrix2& matrix) {
    return Matrix2(matrix.mRows[0][0] * nb, matrix.mRows[0][1] * nb,
                     matrix.mRows[1][0] * nb, matrix.mRows[1][1] * nb);
}

// Overloaded operator for multiplication with a matrix
inline Matrix2 operator*(const Matrix2& matrix, double nb) {
    return nb * matrix;
}

// Overloaded operator for matrix multiplication
inline Matrix2 operator*(const Matrix2& matrix1, const Matrix2& matrix2) {
    return Matrix2(matrix1.mRows[0][0] * matrix2.mRows[0][0] + matrix1.mRows[0][1] *
                     matrix2.mRows[1][0],
                     matrix1.mRows[0][0] * matrix2.mRows[0][1] + matrix1.mRows[0][1] *
                     matrix2.mRows[1][1],
                     matrix1.mRows[1][0] * matrix2.mRows[0][0] + matrix1.mRows[1][1] *
                     matrix2.mRows[1][0],
                     matrix1.mRows[1][0] * matrix2.mRows[0][1] + matrix1.mRows[1][1] *
                     matrix2.mRows[1][1]);
}

// Overloaded operator for multiplication with a vector
inline Vec2 operator*(const Matrix2& matrix, const Vec2& vector) {
    return Vec2(matrix.mRows[0][0]*vector.x + matrix.mRows[0][1]*vector.y,
                   matrix.mRows[1][0]*vector.x + matrix.mRows[1][1]*vector.y);
}

// Overloaded operator for equality condition
inline bool Matrix2::operator==(const Matrix2& matrix) const {
    return (approxEqual(mRows[0][0], matrix.mRows[0][0]) && approxEqual(mRows[0][1], matrix.mRows[0][1]) &&
            approxEqual(mRows[1][0], matrix.mRows[1][0]) && approxEqual(mRows[1][1], matrix.mRows[1][1]));
}

// Overloaded operator for the is different condition
inline bool Matrix2::operator!= (const Matrix2& matrix) const {
    return !(*this == matrix);
}

// Overloaded operator for addition with assignment
inline Matrix2& Matrix2::operator+=(const Matrix2& matrix) {
   mRows[0][0] += matrix.mRows[0][0]; mRows[0][1] += matrix.mRows[0][1];
   mRows[1][0] += matrix.mRows[1][0]; mRows[1][1] += matrix.mRows[1][1];
   return *this;
}

// Overloaded operator for substraction with assignment
inline Matrix2& Matrix2::operator-=(const Matrix2& matrix) {
   mRows[0][0] -= matrix.mRows[0][0]; mRows[0][1] -= matrix.mRows[0][1];
   mRows[1][0] -= matrix.mRows[1][0]; mRows[1][1] -= matrix.mRows[1][1];
   return *this;
}

// Overloaded operator for multiplication with a number with assignment
inline Matrix2& Matrix2::operator*=(double nb) {
   mRows[0][0] *= nb; mRows[0][1] *= nb;
   mRows[1][0] *= nb; mRows[1][1] *= nb;
   return *this;
}

// Overloaded operator to return a row of the matrix.
/// This operator is also used to access a matrix value using the syntax
/// matrix[row][col].
inline const Vec2& Matrix2::operator[](int row) const {
    return mRows[row];
}

// Overloaded operator to return a row of the matrix.
/// This operator is also used to access a matrix value using the syntax
/// matrix[row][col].
inline Vec2& Matrix2::operator[](int row) {
    return mRows[row];
}


#endif
