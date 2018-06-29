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
#include "Vec.h"
#include <iostream>
#include <vector>

// Constructor of the class VecD
Vec::Vec() : x(0.0), y(0.0), z(0.0) {

}

// Constructor with arguments
Vec::Vec(double newX, double newY, double newZ) : x(newX), y(newY), z(newZ) {

}

// Copy-constructor
Vec::Vec(const Vec& vector) : x(vector.x), y(vector.y), z(vector.z) {

}

// Destructor
Vec::~Vec() {

}

std::ostream &operator<<(std::ostream& stream, const Vec& obj) {
    stream << '(' << obj.x << ", " << obj.y << ", " << obj.z << ')';
    return stream;
}

// Return the corresponding unit vector
Vec Vec::getUnit() const {
    double lengthVector = length();

    if (lengthVector < MACHINE_EPSILON) {
        return *this;
    }

    // Compute and return the unit vector
    double lengthInv = double(1.0) / lengthVector;
    return Vec(x * lengthInv, y * lengthInv, z * lengthInv);
}

// Return one unit orthogonal vector of the current vector
Vec Vec::getOneUnitOrthogonalVector() const {

    assert(length() > MACHINE_EPSILON);

    // Get the minimum element of the vector
    Vec vectorAbs(fabs(x), fabs(y), fabs(z));
    int minElement = vectorAbs.getMinAxis();

    if (minElement == 0) {
        return Vec(0.0, -z, y) / sqrt(y*y + z*z);
    }
    else if (minElement == 1) {
        return Vec(-z, 0.0, x) / sqrt(x*x + z*z);
    }
    else {
        return Vec(-y, x, 0.0) / sqrt(x*x + y*y);
    }

}
