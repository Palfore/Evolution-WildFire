#ifndef SHAPES_CPP
#define SHAPES_CPP

#include "Drawing/Draw.h"
#include "Logger.h"
#include "GFramework.h"


/** @brief Draw a 2D rectangle. (T/C)
 * @tparam A (Textured/Colored) The appearance.
 * @todo Create a constructor which specifies the center, width and height.
 */
template <Appearance A>
struct DrawRectangle : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(2);

    /** @brief Draws a 2D rectangle given corner values.
     * @param upperLeft The position of the top left corner of the rectangle.
     * @param lowerRight The position of the bottom right corner of the rectangle.
     */
    DrawRectangle(Vec2 upperLeft, Vec2 lowerRight) : DrawRectangle(upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y) {}

    /** @brief Draws a 2D rectangle given corner values.
     * @param x The left-most x.
     * @param y The up-most y.
     * @param X The right-most x.
     * @param Y The down-most y.
     */
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

/** @brief Draw a 2D circle. (-/C)
 * @tparam A (Colored) The appearance.
 * @warning No texture support.
 */
template <Appearance A>
struct DrawCircle : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(2);

    /** @brief Draws a 2D circle centered at the given position.
     * @details The number of triangluar segments defaults to 20.
     * @param x The x coordinate of the center.
     * @param y The y coordinate of the center.
     * @param r The radius.
     */
    DrawCircle(double x, double y, double r) : DrawCircle(x, y, r, 20) {}

    /** @brief Draws a 2D circle centered at the given position.
     * @param x The x coordinate of the center.
     * @param y The y coordinate of the center.
     * @param r The radius.
     * @param numSegments The number of triangles that make up the circle.
     */
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


/** @brief Draw a plane. (T/C)
 * @tparam A (Textured/Colored) The appearance of the plane.
 */
template <Appearance A>
struct DrawPlane : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(3);

    /** @brief Draws a flat plane centered at the origin.
     * @param length The length of the plane.
     */
    DrawPlane(double length) : DrawPlane(0,0,0, length, 0) {}

    /** @brief Draws a tilted plane centered at the origin, with tilt of xAngle degrees about the x-axis.
     * @param length The length of the plane.
     * @param xAngle The angle in degrees of rotation about the x axis.
     * @warning This method will be replaced by specifying a normal vector.
     */
    DrawPlane(double length, double xAngle) : DrawPlane(0,0,0, length, xAngle) {}

    /** @brief Draws a flat plane centered at the given position.
     * @param pos The center of the plane.
     * @param length The length of the plane.
     */
    DrawPlane(Vec pos, double length) : DrawPlane(pos, length, 0) {}

    /** @brief Draws a flat plane centered at the given position.
     * @param x The x coordinate of the center.
     * @param y The y coordinate of the center.
     * @param z The z coordinate of the center.
     * @param length The length of the plane.
     */
    DrawPlane(double x, double y, double z, double length) : DrawPlane(Vec(x,y,z), length, 0) {}

    /** @brief Draws a flat plane centered at the given position.
     * @param x The x coordinate of the center.
     * @param y The y coordinate of the center.
     * @param z The z coordinate of the center.
     * @param length The length of the plane.
     * @param xAngle The angle in degrees of rotation about the x axis.
     * @warning This method will be replaced by specifying a normal vector.
     */
    DrawPlane(double x, double y, double z, double length, double xAngle) : DrawPlane(Vec(x,y,z), length, xAngle) {}

    /** @brief Draws a flat plane centered at the given position.
     * @param pos The center of the plane.
     * @param length The length of the plane.
     * @param xAngle The angle in degrees of rotation about the x axis.
     * @warning This method will be replaced by specifying a normal vector.
     */
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

/** @brief Draw a 3D sphere. (T/C)
 * @tparam A (Textured/Colored) The appearance of the sphere.
 */
template <Appearance A>
struct DrawSphere : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(3);

    /** @brief Draws the sphere at the origin.
     * @param r The radius of the sphere.
     * @return
     *
     */
    DrawSphere(double r) : DrawSphere(Vec(0,0,0), r) {}

    /** @brief Draws the sphere at a given position.
     * @param pos The center of the sphere.
     * @param r The radius of the sphere.
     * @return
     *
     */
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

/** @brief Draw a 3D cylinder. (T/C)
 * @tparam A (Textured/Colored) The appearance of the cylinder.
 */
template <Appearance A>
struct DrawCylinder : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(3);

//    DrawCylinder(double l, double r) : DrawCylinder(Vec(0,0,0), l, r) {} /// not yet implemented
//    DrawCylinder(Vec pos, double l, double r) : DrawItem<A>(dimension) {
//        draw(pos, l, r);
//    }

    /** @brief Draws the cylinder.
     * @param pos1 The starting position.
     * @param pos2 The ending position.
     * @param radius The radius of the cylinder.
     * @return
     *
     */
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
