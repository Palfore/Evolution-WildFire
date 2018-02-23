#ifndef SHAPES_CPP
#define SHAPES_CPP

#include "Drawing/Draw.h"
#include "Logger.h"
#include "GFramework.h"

template <Appearance A>
struct DrawRectangle : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(2);

    DrawRectangle(double x, double y, double X, double Y) : DrawItem<A>(dimension) {
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

template <Appearance A>
struct DrawCircle : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(2);

    DrawCircle(double x, double y, double r) : DrawCircle(x, y, r, 20) {}
    DrawCircle(double x, double y, double r, int numSegments) : DrawItem<A>(dimension) {
        double w = GFramework::get->windowSize.x;
        double h = GFramework::get->windowSize.y;
        draw(x*w, y*h, 0.5*(w+h)*r, numSegments); // how should r be scaled? this seems wrong because asymetic scaling of window is possible.
    }
    private:void draw(double x, double y, double r, int numSegments) {
        if (!Drawing::isColor(A)) {
            LOG("Cicles can't be draw textured.", LogDegree::WARNING, LogType::GRAPHICS);
        }
        glBegin(GL_POLYGON);
        for(int i = 0; i < numSegments; i++) {
            float theta = 2.0 * PI * double(i) / double(numSegments);
            float xNext = r * cosf(theta);
            float yNext = r * sinf(theta);

            glVertex2f(x + xNext, y + yNext);
        }
        glEnd();
    }
};

template <Appearance A>
struct DrawPlane : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(3);

    DrawPlane(double length) : DrawPlane(0,0,0, length, 0) {}
    DrawPlane(double length, double xAngle) : DrawPlane(0,0,0, length, xAngle) {}
    DrawPlane(Vec pos, double length) : DrawPlane(pos, length, 0) {}
    DrawPlane(double x, double y, double z, double length) : DrawPlane(Vec(x,y,z), length, 0) {}
    DrawPlane(double x, double y, double z, double length, double xAngle) : DrawPlane(Vec(x,y,z), length, xAngle) {}
    DrawPlane(Vec pos, double length, double xAngle) : DrawItem<A>(dimension) {
        draw(pos, length, xAngle);
    }
    private:void draw(Vec pos, double length, double xAngle) {
        glBegin(GL_QUADS);
            glTexVert3f(0, 0, pos.x + -length * cos(xAngle), pos.y + -length, pos.z +  length * sin(xAngle));
            glTexVert3f(0, 1, pos.x + -length * cos(xAngle), pos.y +  length, pos.z +  length * sin(xAngle));
            glTexVert3f(1, 1, pos.x +  length * cos(xAngle), pos.y +  length, pos.z + -length * sin(xAngle));
            glTexVert3f(1, 0, pos.x +  length * cos(xAngle), pos.y + -length, pos.z + -length * sin(xAngle));
        glEnd();
    }
};


#endif // SHAPES_CPP
