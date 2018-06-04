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
            glTexVert2f(0, 1, x, y);
            glTexVert2f(0, 0, x, Y);
            glTexVert2f(1, 0, X, Y);
            glTexVert2f(1, 1, X, y);
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


template <Appearance A>
struct DrawSphere : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(3);

    DrawSphere(double r) : DrawSphere(Vec(0,0,0), r) {}
    DrawSphere(Vec pos, double r) : DrawItem<A>(dimension) {
        draw(pos, r);
    }
    private:void draw(Vec pos, double r) {
        GLUquadric *qobj = gluNewQuadric();

        gluQuadricTexture(qobj,GL_TRUE);

        glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            gluSphere(qobj,r,20,20);
        glPopMatrix();

        gluDeleteQuadric(qobj);
        gluQuadricTexture(qobj,GL_FALSE);

//        glPushMatrix();
//            glTranslated(pos.x, pos.y, pos.z);
//            glutSolidSphere(r, 20, 20);
//        glPopMatrix();
    }
};

template <Appearance A>
struct DrawCylinder : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(3);

//    DrawCylinder(double l, double r) : DrawCylinder(Vec(0,0,0), l, r) {} /// not yet implemented
//    DrawCylinder(Vec pos, double l, double r) : DrawItem<A>(dimension) {
//        draw(pos, l, r);
//    }
    DrawCylinder(Vec pos1, Vec pos2, double radius) : DrawItem<A>(dimension) {
        draw(pos1, pos2, radius);
    }
    private:void draw(Vec pos1, Vec pos2, float radius) {
          //http://lifeofaprogrammergeek.blogspot.ca/2008/07/rendering-cylinder-between-two-points.html
        GLUquadricObj *quadric = gluNewQuadric();
        gluQuadricTexture(quadric,GL_TRUE);
        gluQuadricNormals(quadric, GLU_SMOOTH);
        int subdivisions = 10;
        float vx = pos2.x-pos1.x;
        float vy = pos2.y-pos1.y;
        float vz = pos2.z-pos1.z;

        //handle the degenerate case of pos1.z == z2 with an approximation
        if(fabs(vz) < 0.0001)
           vz = 0.0001;

        float v = sqrt( vx*vx + vy*vy + vz*vz );
        float ax = 57.2957795*acos( vz/v );
        if ( vz < 0.0 )
            ax = -ax;
        float rx = -vy*vz;
        float ry = vx*vz;
        glPushMatrix();

        //draw the cylinder body
        glTranslatef( pos1.x,pos1.y,pos1.z );
        glRotatef(ax, rx, ry, 0.0);
        gluQuadricOrientation(quadric,GLU_OUTSIDE);
        gluCylinder(quadric, radius, radius, v, subdivisions, 1);




        //draw the first cap
        gluQuadricOrientation(quadric,GLU_INSIDE);
        gluDisk( quadric, 0.0, radius, subdivisions, 1);
        glTranslatef( 0,0,v );

        //draw the second cap
        gluQuadricOrientation(quadric,GLU_OUTSIDE);
        gluDisk( quadric, 0.0, radius, subdivisions, 1);
        glPopMatrix();
          gluDeleteQuadric(quadric);

        gluQuadricTexture(quadric,GL_FALSE);

    }
};


#endif // SHAPES_CPP
