#ifndef OBJECTS_H
#define OBJECTS_H

#include "Drawing/Draw.h"
#include "GFramework.h"

template <Appearance A>
struct DrawMySpecificObject : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(3);

    DrawMySpecificObject(double x, double y, double X, double Y) : DrawItem<A>(dimension) {
        double w = GFramework::get->windowSize.x;
        double h = GFramework::get->windowSize.y;
        draw(x*w, y*h, X*w, Y*h);
    }
    private:void draw(double x, double y, double X, double Y) {
        glBegin(GL_QUADS);
            glTexVert2f(0, 0, x, y);
            glTexVert2f(0, 1, x, Y);
            glTexVert2f(1, 1, X, Y);
            glTexVert2f(1, 0, X, y);
        glEnd();
    }
};

#endif // OBJECTS_H
