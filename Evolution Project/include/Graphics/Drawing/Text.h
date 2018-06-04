#ifndef TEXT_H
#define TEXT_H

#include "Drawing/Draw.h"
#include "GFramework.h"

template <Appearance A>
struct DrawString : DrawItem<A> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(2);

    DrawString(std::string s, float x, float y, bool centerX=true, bool centerY=true) : DrawItem<A>(dimension) {
        if (!Drawing::isColor(A)) {
            LOG("The Appearance of strings should be a color.", LogDegree::WARNING, LogType::GRAPHICS);
        }
        double w = GFramework::get->windowSize.x;
        double h = GFramework::get->windowSize.y;
        draw(s, w*x, h*y, centerX, centerY);
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
