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


#ifndef REACTPHYSICS3D_Matrix_H
#define REACTPHYSICS3D_Matrix_H

// Libraries
#include <cassert>
#include "Vec.h"
#include "MyMath.h"

/**
 * This class represents a 3x3 matrix.
 */
class Matrix {

    private :

        // -------------------- Attributes -------------------- //

        /// Rows of the matrix;
        Vec mRows[3];

    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        Matrix();

        /// Constructor
        Matrix(double value);

        /// Constructor
        Matrix(double a1, double a2, double a3, double b1, double b2, double b3,
                  double c1, double c2, double c3);

        /// Destructor
        virtual ~Matrix();

        /// Copy-constructor
        Matrix(const Matrix& matrix);

        /// Assignment operator
        Matrix& operator=(const Matrix& matrix);

        /// Set all the values in the matrix
        void setAllValues(double a1, double a2, double a3, double b1, double b2, double b3,
                  double c1, double c2, double c3);

        /// Set the matrix to zero
        void setToZero();

        /// Return a column
        Vec getColumn(int i) const;

        /// Return a row
        Vec getRow(int i) const;

        /// Return the transpose matrix
        Matrix getTranspose() const;

        /// Return the determinant of the matrix
        double getDeterminant() const;

        /// Return the trace of the matrix
        double getTrace() const;

        /// Return the inverse matrix
        Matrix getInverse() const;

        /// Return the matrix with absolute values
        Matrix getAbsoluteMatrix() const;

        /// Set the matrix to the identity matrix
        void setToIdentity();

        /// Return the 3x3 identity matrix
        static Matrix identity();

        /// Return the 3x3 zero matrix
        static Matrix zero();

        /// Return a skew-symmetric matrix using a given vector that can be used
        /// to compute cross product with another vector using matrix multiplication
        static Matrix computeSkewSymmetricMatrixForCrossProduct(const Vec& vector);

        /// Overloaded operator for addition
        friend Matrix operator+(const Matrix& matrix1, const Matrix& matrix2);

        /// Overloaded operator for substraction
        friend Matrix operator-(const Matrix& matrix1, const Matrix& matrix2);

        /// Overloaded operator for the negative of the matrix
        friend Matrix operator-(const Matrix& matrix);

        /// Overloaded operator for multiplication with a number
        friend Matrix operator*(double nb, const Matrix& matrix);

        /// Overloaded operator for multiplication with a matrix
        friend Matrix operator*(const Matrix& matrix, double nb);

        /// Overloaded operator for matrix multiplication
        friend Matrix operator*(const Matrix& matrix1, const Matrix& matrix2);

        /// Overloaded operator for multiplication with a vector
        friend Vec operator*(const Matrix& matrix, const Vec& vector);

        /// Overloaded operator for equality condition
        bool operator==(const Matrix& matrix) const;

        /// Overloaded operator for the is different condition
        bool operator!= (const Matrix& matrix) const;

        /// Overloaded operator for addition with assignment
        Matrix& operator+=(const Matrix& matrix);

        /// Overloaded operator for substraction with assignment
        Matrix& operator-=(const Matrix& matrix);

        /// Overloaded operator for multiplication with a number with assignment
        Matrix& operator*=(double nb);

        /// Overloaded operator to read element of the matrix.
        const Vec& operator[](int row) const;

        /// Overloaded operator to read/write element of the matrix.
        Vec& operator[](int row);
};

// Method to set all the values in the matrix
inline void Matrix::setAllValues(double a1, double a2, double a3,
                                    double b1, double b2, double b3,
                                    double c1, double c2, double c3) {
    mRows[0][0] = a1; mRows[0][1] = a2; mRows[0][2] = a3;
    mRows[1][0] = b1; mRows[1][1] = b2; mRows[1][2] = b3;
    mRows[2][0] = c1; mRows[2][1] = c2; mRows[2][2] = c3;
}

// Set the matrix to zero
inline void Matrix::setToZero() {
    mRows[0].setToZero();
    mRows[1].setToZero();
    mRows[2].setToZero();
}

// Return a column
inline Vec Matrix::getColumn(int i) const {
    assert(i>= 0 && i<3);
    return Vec(mRows[0][i], mRows[1][i], mRows[2][i]);
}

// Return a row
inline Vec Matrix::getRow(int i) const {
    assert(i>= 0 && i<3);
    return mRows[i];
}

// Return the transpose matrix
inline Matrix Matrix::getTranspose() const {

    // Return the transpose matrix
    return Matrix(mRows[0][0], mRows[1][0], mRows[2][0],
                     mRows[0][1], mRows[1][1], mRows[2][1],
                     mRows[0][2], mRows[1][2], mRows[2][2]);
}

// Return the determinant of the matrix
inline double Matrix::getDeterminant() const {

    // Compute and return the determinant of the matrix
    return (mRows[0][0]*(mRows[1][1]*mRows[2][2]-mRows[2][1]*mRows[1][2]) -
            mRows[0][1]*(mRows[1][0]*mRows[2][2]-mRows[2][0]*mRows[1][2]) +
            mRows[0][2]*(mRows[1][0]*mRows[2][1]-mRows[2][0]*mRows[1][1]));
}

// Return the trace of the matrix
inline double Matrix::getTrace() const {

    // Compute and return the trace
    return (mRows[0][0] + mRows[1][1] + mRows[2][2]);
}

// Set the matrix to the identity matrix
inline void Matrix::setToIdentity() {
    mRows[0][0] = 1.0; mRows[0][1] = 0.0; mRows[0][2] = 0.0;
    mRows[1][0] = 0.0; mRows[1][1] = 1.0; mRows[1][2] = 0.0;
    mRows[2][0] = 0.0; mRows[2][1] = 0.0; mRows[2][2] = 1.0;
}

// Return the 3x3 identity matrix
inline Matrix Matrix::identity() {
    return Matrix(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

// Return the 3x3 zero matrix
inline Matrix Matrix::zero() {
    return Matrix(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

// Return a skew-symmetric matrix using a given vector that can be used
// to compute cross product with another vector using matrix multiplication
inline Matrix Matrix::computeSkewSymmetricMatrixForCrossProduct(const Vec& vector) {
    return Matrix(0, -vector.z, vector.y, vector.z, 0, -vector.x, -vector.y, vector.x, 0);
}

// Return the matrix with absolute values
inline Matrix Matrix::getAbsoluteMatrix() const {
    return Matrix(fabs(mRows[0][0]), fabs(mRows[0][1]), fabs(mRows[0][2]),
                     fabs(mRows[1][0]), fabs(mRows[1][1]), fabs(mRows[1][2]),
                     fabs(mRows[2][0]), fabs(mRows[2][1]), fabs(mRows[2][2]));
}

// Overloaded operator for addition
inline Matrix operator+(const Matrix& matrix1, const Matrix& matrix2) {
    return Matrix(matrix1.mRows[0][0] + matrix2.mRows[0][0], matrix1.mRows[0][1] +
                     matrix2.mRows[0][1], matrix1.mRows[0][2] + matrix2.mRows[0][2],
                     matrix1.mRows[1][0] + matrix2.mRows[1][0], matrix1.mRows[1][1] +
                     matrix2.mRows[1][1], matrix1.mRows[1][2] + matrix2.mRows[1][2],
                     matrix1.mRows[2][0] + matrix2.mRows[2][0], matrix1.mRows[2][1] +
                     matrix2.mRows[2][1], matrix1.mRows[2][2] + matrix2.mRows[2][2]);
}

// Overloaded operator for substraction
inline Matrix operator-(const Matrix& matrix1, const Matrix& matrix2) {
    return Matrix(matrix1.mRows[0][0] - matrix2.mRows[0][0], matrix1.mRows[0][1] -
                     matrix2.mRows[0][1], matrix1.mRows[0][2] - matrix2.mRows[0][2],
                     matrix1.mRows[1][0] - matrix2.mRows[1][0], matrix1.mRows[1][1] -
                     matrix2.mRows[1][1], matrix1.mRows[1][2] - matrix2.mRows[1][2],
                     matrix1.mRows[2][0] - matrix2.mRows[2][0], matrix1.mRows[2][1] -
                     matrix2.mRows[2][1], matrix1.mRows[2][2] - matrix2.mRows[2][2]);
}

// Overloaded operator for the negative of the matrix
inline Matrix operator-(const Matrix& matrix) {
    return Matrix(-matrix.mRows[0][0], -matrix.mRows[0][1], -matrix.mRows[0][2],
                     -matrix.mRows[1][0], -matrix.mRows[1][1], -matrix.mRows[1][2],
                     -matrix.mRows[2][0], -matrix.mRows[2][1], -matrix.mRows[2][2]);
}

// Overloaded operator for multiplication with a number
inline Matrix operator*(double nb, const Matrix& matrix) {
    return Matrix(matrix.mRows[0][0] * nb, matrix.mRows[0][1] * nb, matrix.mRows[0][2] * nb,
                     matrix.mRows[1][0] * nb, matrix.mRows[1][1] * nb, matrix.mRows[1][2] * nb,
                     matrix.mRows[2][0] * nb, matrix.mRows[2][1] * nb, matrix.mRows[2][2] * nb);
}

// Overloaded operator for multiplication with a matrix
inline Matrix operator*(const Matrix& matrix, double nb) {
    return nb * matrix;
}

// Overloaded operator for matrix multiplication
inline Matrix operator*(const Matrix& matrix1, const Matrix& matrix2) {
    return Matrix(matrix1.mRows[0][0]*matrix2.mRows[0][0] + matrix1.mRows[0][1] *
                     matrix2.mRows[1][0] + matrix1.mRows[0][2]*matrix2.mRows[2][0],
                     matrix1.mRows[0][0]*matrix2.mRows[0][1] + matrix1.mRows[0][1] *
                     matrix2.mRows[1][1] + matrix1.mRows[0][2]*matrix2.mRows[2][1],
                     matrix1.mRows[0][0]*matrix2.mRows[0][2] + matrix1.mRows[0][1] *
                     matrix2.mRows[1][2] + matrix1.mRows[0][2]*matrix2.mRows[2][2],
                     matrix1.mRows[1][0]*matrix2.mRows[0][0] + matrix1.mRows[1][1] *
                     matrix2.mRows[1][0] + matrix1.mRows[1][2]*matrix2.mRows[2][0],
                     matrix1.mRows[1][0]*matrix2.mRows[0][1] + matrix1.mRows[1][1] *
                     matrix2.mRows[1][1] + matrix1.mRows[1][2]*matrix2.mRows[2][1],
                     matrix1.mRows[1][0]*matrix2.mRows[0][2] + matrix1.mRows[1][1] *
                     matrix2.mRows[1][2] + matrix1.mRows[1][2]*matrix2.mRows[2][2],
                     matrix1.mRows[2][0]*matrix2.mRows[0][0] + matrix1.mRows[2][1] *
                     matrix2.mRows[1][0] + matrix1.mRows[2][2]*matrix2.mRows[2][0],
                     matrix1.mRows[2][0]*matrix2.mRows[0][1] + matrix1.mRows[2][1] *
                     matrix2.mRows[1][1] + matrix1.mRows[2][2]*matrix2.mRows[2][1],
                     matrix1.mRows[2][0]*matrix2.mRows[0][2] + matrix1.mRows[2][1] *
                     matrix2.mRows[1][2] + matrix1.mRows[2][2]*matrix2.mRows[2][2]);
}

// Overloaded operator for multiplication with a vector
inline Vec operator*(const Matrix& matrix, const Vec& vector) {
    return Vec(matrix.mRows[0][0]*vector.x + matrix.mRows[0][1]*vector.y +
                   matrix.mRows[0][2]*vector.z,
                   matrix.mRows[1][0]*vector.x + matrix.mRows[1][1]*vector.y +
                   matrix.mRows[1][2]*vector.z,
                   matrix.mRows[2][0]*vector.x + matrix.mRows[2][1]*vector.y +
                   matrix.mRows[2][2]*vector.z);
}

// Overloaded operator for equality condition
inline bool Matrix::operator==(const Matrix& matrix) const {
    return (approxEqual(mRows[0][0], matrix.mRows[0][0]) && approxEqual(mRows[0][1], matrix.mRows[0][1]) &&
            approxEqual(mRows[0][2], matrix.mRows[0][2]) &&
            approxEqual(mRows[1][0], matrix.mRows[1][0]) && approxEqual(mRows[1][1], matrix.mRows[1][1]) &&
            approxEqual(mRows[1][2], matrix.mRows[1][2]) &&
            approxEqual(mRows[2][0], matrix.mRows[2][0]) && approxEqual(mRows[2][1], matrix.mRows[2][1]) &&
            approxEqual(mRows[2][2], matrix.mRows[2][2]));
}

// Overloaded operator for the is different condition
inline bool Matrix::operator!= (const Matrix& matrix) const {
    return !(*this == matrix);
}

// Overloaded operator for addition with assignment
inline Matrix& Matrix::operator+=(const Matrix& matrix) {
   mRows[0][0] += matrix.mRows[0][0]; mRows[0][1] += matrix.mRows[0][1];
   mRows[0][2] += matrix.mRows[0][2]; mRows[1][0] += matrix.mRows[1][0];
   mRows[1][1] += matrix.mRows[1][1]; mRows[1][2] += matrix.mRows[1][2];
   mRows[2][0] += matrix.mRows[2][0]; mRows[2][1] += matrix.mRows[2][1];
   mRows[2][2] += matrix.mRows[2][2];
   return *this;
}

// Overloaded operator for substraction with assignment
inline Matrix& Matrix::operator-=(const Matrix& matrix) {
   mRows[0][0] -= matrix.mRows[0][0]; mRows[0][1] -= matrix.mRows[0][1];
   mRows[0][2] -= matrix.mRows[0][2]; mRows[1][0] -= matrix.mRows[1][0];
   mRows[1][1] -= matrix.mRows[1][1]; mRows[1][2] -= matrix.mRows[1][2];
   mRows[2][0] -= matrix.mRows[2][0]; mRows[2][1] -= matrix.mRows[2][1];
   mRows[2][2] -= matrix.mRows[2][2];
   return *this;
}

// Overloaded operator for multiplication with a number with assignment
inline Matrix& Matrix::operator*=(double nb) {
   mRows[0][0] *= nb; mRows[0][1] *= nb; mRows[0][2] *= nb;
   mRows[1][0] *= nb; mRows[1][1] *= nb; mRows[1][2] *= nb;
   mRows[2][0] *= nb; mRows[2][1] *= nb; mRows[2][2] *= nb;
   return *this;
}

// Overloaded operator to return a row of the matrix.
/// This operator is also used to access a matrix value using the syntax
/// matrix[row][col].
inline const Vec& Matrix::operator[](int row) const {
    return mRows[row];
}

// Overloaded operator to return a row of the matrix.
/// This operator is also used to access a matrix value using the syntax
/// matrix[row][col].
inline Vec& Matrix::operator[](int row) {
    return mRows[row];
}


#endif
