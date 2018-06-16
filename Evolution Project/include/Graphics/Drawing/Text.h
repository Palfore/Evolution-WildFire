#ifndef TEXT_H
#define TEXT_H

#include "Drawing/Draw.h"
#include "GFramework.h"

struct by_pixel{};
struct by_percentage{};

template <Appearance A>
struct DrawString : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(2);

    DrawString(std::string s, float x, float y, by_percentage, bool centerX=true, bool centerY=true) : DrawItem<A>(dimension) {
        if (!Drawing::isColor(A)) {
            LOG("The Appearance of strings should be a color.", LogDegree::WARNING, LogType::GRAPHICS);
        }
        double w = GFramework::get->windowSize.x;
        double h = GFramework::get->windowSize.y;
        draw(s, w*x, h*y, centerX, centerY);
    }
    DrawString(std::string s, float x, float y, by_pixel, bool centerX=true, bool centerY=true) : DrawItem<A>(dimension) {
        if (!Drawing::isColor(A)) {
            LOG("The Appearance of strings should be a color.", LogDegree::WARNING, LogType::GRAPHICS);
        }
        draw(s, x, y, centerX, centerY);
    }
    DrawString(std::string s, Vec worldCoords, bool centerX=true, bool centerY=true) : DrawItem<A>(dimension) {
        if (!Drawing::isColor(A)) {
            LOG("The Appearance of strings should be a color.", LogDegree::WARNING, LogType::GRAPHICS);
        }

        GLdouble modelview[16];
        GLdouble projection[16];
        GLint viewport[4];

        Drawing::enable3D(true);
            glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
            glGetDoublev(GL_PROJECTION_MATRIX, projection);
            glGetIntegerv(GL_VIEWPORT, viewport);
        Drawing::enable2D();

        GLdouble x;
        GLdouble y;
        GLdouble z;
        gluProject(worldCoords.x, worldCoords.y, worldCoords.z, modelview, projection, viewport, &x, &y, &z);
        double h = GLOBAL->windowSize.y;
        draw(s, x, h-y, centerX, centerY);
    }
    private: draw(std::string s, float x, float y, bool centerX=true, bool centerY=true) {
        int width = 0;
        const int fontHeight = 18;
        if (centerX) {
            for (const char &c: s) {
                width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, c);
            }
            x -= width * 0.5;
        }
        if (centerY) y += fontHeight * 0.25;

        glRasterPos2f(x, y);
        for (const char &c: s) {
            x += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, c);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
        return x;
    }
};

#endif // TEXT_H
