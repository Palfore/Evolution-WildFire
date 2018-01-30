#ifndef OBJECTS_H
#define OBJECTS_H

#include "Drawing/Draw.h"
#include "Graphics.h"

template <Appearance A>
struct DrawMySpecificObject : DrawItem<A> {
    static Dimension constexpr dimension = static_cast<Dimension>(3);

    DrawMySpecificObject(double x, double y, double X, double Y) : DrawItem<A>(dimension) {
        double w = Graphics::get().windowSize.x;
        double h = Graphics::get().windowSize.y;
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
