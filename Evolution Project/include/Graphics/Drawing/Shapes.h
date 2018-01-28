#ifndef SHAPES_CPP
#define SHAPES_CPP

#include "Drawing/Draw.h"
#include "Logger.h"

template <Appearance A> class DrawShape { protected:
    DrawShape(Dimension dim) {
        Drawing::enableND(dim);

        /* Apply Appearance */
        if (Drawing::isTexture(A)) {
            Drawing::changeColor(Appearance::WHITE);
            glEnable(GL_TEXTURE_2D);
            Drawing::changeTexture(A);
        }
        else if (Drawing::isColor(A)) {
            glDisable(GL_TEXTURE_2D);
            Drawing::changeColor(A);
        } else {
            LOG(LogDegree::FATAL, LogType::GRAPHICS, "Appearance is not valid.");
        }
    }
};

template <Appearance A> class DrawRectangle : DrawShape<A> { public:
    static Dimension constexpr dimension = static_cast<Dimension>(2);

    DrawRectangle(double x, double y, double X, double Y) : DrawShape<A>(dimension) {
        draw(x, y, X, Y);
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

template <Appearance A> class DrawCircle : DrawShape<A> { public:
    static Dimension constexpr dimension = static_cast<Dimension>(2);

    DrawCircle(double x, double y, double r) : DrawCircle(x, y, r, 20) {}
    DrawCircle(double x, double y, double r, int numSegments) : DrawShape<A>(dimension) {
        draw(x, y, r, numSegments);
    }
    private:void draw(double x, double y, double r, int numSegments) {
        glBegin(GL_POLYGON);
        for(int i = 0; i < numSegments; i++) {
            float theta = 2.0 * PI * double(i) / double(numSegments); // Get the current angle
            float xNext = r * cosf(theta);
            float yNext = r * sinf(theta);

            glVertex2f(x + xNext, y + yNext); // Output Vertex
        }
        glEnd();
    }
};

template <Appearance A> class DrawPlane : DrawShape<A> { public:
    static Dimension constexpr dimension = static_cast<Dimension>(3);

    DrawPlane(double length) : DrawPlane(0,0,0, length, 0) {}
    DrawPlane(double length, double xAngle) : DrawPlane(0,0,0, length, xAngle) {}
    DrawPlane(Vec pos, double length) : DrawPlane(pos, length, 0) {}
    DrawPlane(double x, double y, double z, double length) : DrawPlane(Vec(x,y,z), length, 0) {}
    DrawPlane(double x, double y, double z, double length, double xAngle) : DrawPlane(Vec(x,y,z), length, xAngle) {}
    DrawPlane(Vec pos, double length, double xAngle) : DrawShape<A>(dimension) {
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
