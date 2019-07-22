#ifndef TERRAIN_H
#define TERRAIN_H

#include "Vec.h"
class Terrain {
 public:
    Terrain(double maxHeight=100);
    virtual ~Terrain();

    double getHeight(Vec r) const;
    double getHeight(double x, double y) const;
    Vec getNormal(Vec r) const;
    Vec getNormal(double x, double y) const;
    void draw() const;

 private:
 	static constexpr int len = 200;
    static constexpr double h = 1; // precision

    const double maxHeight;
};

#endif // TERRAIN_H
