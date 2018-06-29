#include "Trail.h"

#include "Shapes.h"

Trail::Trail() : trail({}) {
    //ctor
}

Trail::~Trail() {
    //dtor
}

void Trail::addPoint(Vec point) {
    const unsigned int len = trail.size();
    if (len > MAX_TRAIL_SIZE) {
        switch (deleteType) {
            default: [[fallthrough]]
            case DeletionType::CIRCULAR:
                trail.pop_front();
                break;
            case DeletionType::HALVING:
                for (unsigned int i = 1; i < len; i+=2) {
                    trail.erase(trail.begin() + i);
                }
                break;
            case DeletionType::BLOCKING:
                return;
        }
    }
    trail.push_back(point);
}

void Trail::draw() const {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricTexture(quadric,GL_FALSE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    Drawing::enableND(Drawing::Dimension::THREE);
    glDisable(GL_TEXTURE_2D);
    for (unsigned int i = 0; i < trail.size() - 1; i++) {
        Vec start = trail[i];
        Vec ending = trail[i+1];

        double x = 1 - i / static_cast<double>(trail.size());
        glColor3f(x,x,x);

        Vec v = (ending-start) * 1.25;
        if (fabs(v.z) < 0.0001) v.z = 0.0001;
        const double mag = v.length();

        double ax = 57.2957795*acos( v.z/mag );
        if ( v.z < 0.0 ) ax = -ax;
        const Vec2 r = Vec2(-v.y, v.x)*v.z;
        glPushMatrix();
            glTranslatef(start.x, start.y, start.z);
            glRotatef(ax, r.x, r.y, 0.0);
            gluQuadricOrientation(quadric,GLU_OUTSIDE);
            gluCylinder(quadric, RADIUS, RADIUS, mag, SUB_DIVISIONS, 1);

            //draw the first cap
            gluQuadricOrientation(quadric,GLU_INSIDE);
            gluDisk( quadric, 0.0, RADIUS, SUB_DIVISIONS, 1);
            glTranslatef( 0,0,mag );

            //draw the second cap
            gluQuadricOrientation(quadric,GLU_OUTSIDE);
            gluDisk( quadric, 0.0, RADIUS, SUB_DIVISIONS, 1);
        glPopMatrix();

    }
    gluDeleteQuadric(quadric);
}
