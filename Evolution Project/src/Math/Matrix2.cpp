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

// Libraries
#include "Matrix2.h"

// Constructor of the class Matrix2
Matrix2::Matrix2() {

    // Initialize all values in the matrix to zero
    setAllValues(0.0, 0.0, 0.0, 0.0);
}

// Constructor
Matrix2::Matrix2(double value) {
    setAllValues(value, value, value, value);
}

// Constructor with arguments
Matrix2::Matrix2(double a1, double a2, double b1, double b2) {

    // Initialize the matrix with the values
    setAllValues(a1, a2, b1, b2);
}

// Destructor
Matrix2::~Matrix2() {

}

// Copy-constructor
Matrix2::Matrix2(const Matrix2& matrix) {
    setAllValues(matrix.mRows[0][0], matrix.mRows[0][1],
                 matrix.mRows[1][0], matrix.mRows[1][1]);
}

std::ostream &operator<<(std::ostream& stream, const Matrix2& matrix) {
    stream << '[' << matrix.mRows[0].x << ", " << matrix.mRows[0].y << '\n'
           << ' ' << matrix.mRows[1].x << ", " << matrix.mRows[1].y << ']';
    return stream;
}

// Assignment operator
Matrix2& Matrix2::operator=(const Matrix2& matrix) {

    // Check for self-assignment
    if (&matrix != this) {
        setAllValues(matrix.mRows[0][0], matrix.mRows[0][1],
                     matrix.mRows[1][0], matrix.mRows[1][1]);
    }
    return *this;
}

// Return the inverse matrix
Matrix2 Matrix2::getInverse() const {

    // Compute the determinant of the matrix
    double determinant = getDeterminant();

    // Check if the determinant is equal to zero
    assert(std::abs(determinant) > MACHINE_EPSILON);

    double invDeterminant = double(1.0) / determinant;

    Matrix2 tempMatrix(mRows[1][1], -mRows[0][1], -mRows[1][0], mRows[0][0]);

    // Return the inverse matrix
    return (invDeterminant * tempMatrix);
}
