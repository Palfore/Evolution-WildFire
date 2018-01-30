#ifndef TEXT_H
#define TEXT_H

#include "Drawing/Draw.h"
#include "Graphics.h"

template <Appearance A>
struct DrawString : DrawItem<A> {
    static Dimension constexpr dimension = static_cast<Dimension>(2);

    DrawString(std::string s, float x, float y, bool centerX=true, bool centerY=true) : DrawItem<A>(dimension) {
        double w = Graphics::get().windowSize.x;
        double h = Graphics::get().windowSize.y;
        draw(s, w*x, h*y, centerX, centerY);
    }
    private: draw(std::string s, float x, float y, bool centerX=true, bool centerY=true) {
        int width = 0;
        int fontHeight = 18;
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
