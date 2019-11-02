#include "Trail.h"
#include "Shapes.h"

Trail::Trail(unsigned int max_size, unsigned int sampling_frequency):
    trail({}), maxSize(max_size), samplingFrequency(sampling_frequency) {
    //ctor
}

Trail::~Trail() {
    //dtor
}

Vec Trail::operator[](int i) const {
    return trail[i];
}

Vec Trail::operator[](int i) {
    return trail[i];
}

void Trail::clear() {
    trail.clear();
}

bool Trail::isFull() const {
    return trail.size() == maxSize;
}

void Trail::addPoint(Vec point) {
    const unsigned int len = trail.size();
    if (len > maxSize - 1) {
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

const std::deque<Vec>& Trail::getPoints() const {
    return trail;
}

void Trail::draw() const {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricTexture(quadric,GL_FALSE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    Drawing::enableND(Drawing::Dimension::THREE);
    glDisable(GL_TEXTURE_2D);
    for (unsigned int i = 1; i < trail.size(); i++) {
        Vec start = trail[i-1];
        Vec ending = trail[i];

        double x = 1 - (i - 1) / static_cast<double>(trail.size());
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

long long int Trail::size() const {
    return trail.size();
}