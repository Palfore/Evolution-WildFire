#include "Drawing/Draw.h"
#include "MyGlut.h" // Glut
#include "GFramework.h" // GFramework::get().drawingState, Dimension
#include "Logger.h"

void Drawing::changeCustomColor(double r, double g, double b) {
    GLOBAL->colorMap[Appearance::CUSTOM] = Vec(r, g, b);
    Drawing::changeColor(Appearance::CUSTOM);
}

void Drawing::changeCustomColor(Vec color) {
    Drawing::changeCustomColor(color.x, color.y, color.z);
}

bool Drawing::isColor(Appearance A) {
    if (A < 0) return false;
    if (A >= Appearance::LAST_COLOR__) return false;
    return true;
}

bool Drawing::isTexture(Appearance A) {
    if (A < Appearance::LAST_COLOR__) return false;
    if (A >= Appearance::LAST_TEXTURE__) return false;
    return true;
}

void Drawing::changeColor(Vec c) {
    glColor3f(c.x, c.y, c.z);
}

void Drawing::changeColor(Appearance C) {
    if (!Drawing::isColor(C)) LOG(INVALID_COLOR_MESSAGE, LogDegree::WARNING, LogType::GRAPHICS);
    try {
        Drawing::changeColor(GFramework::get->colorMap.at(C));
    } catch (std::out_of_range) {
        LOG(UNKNOWN_APPROVED_COLOR_MESSAGE, LogDegree::WARNING, LogType::GRAPHICS);
    }
}

void Drawing::changeTexture(Tex textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Drawing::changeTexture(Appearance T) {
    if (!Drawing::isTexture(T)) LOG(INVALID_TEXTURE_MESSAGE, LogDegree::WARNING, LogType::GRAPHICS);
    try {
        Drawing::changeTexture(GFramework::get->textureMap.at(T));
    } catch (std::out_of_range) {
        LOG(UNKNOWN_APPROVED_TEXTURE_MESSAGE, LogDegree::WARNING, LogType::GRAPHICS);
    }
}

void Drawing::enable2D() {
    if (GFramework::get->drawingState == Dimension::TWO) return;
    GFramework::get->drawingState = Dimension::TWO;

    glMatrixMode(GL_MODELVIEW);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, GFramework::get->windowSize.x, GFramework::get->windowSize.y, 0);
    glViewport(0, 0, GFramework::get->windowSize.x, GFramework::get->windowSize.y);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}

void Drawing::enable3D(bool force) {
    if ((GFramework::get->drawingState == Dimension::TWO) && !force) LOG(INVALID_DRAWING_ORDER_MESSAGE, LogDegree::FATAL, LogType::GRAPHICS);
    if (GFramework::get->drawingState == Dimension::THREE) return;
    GFramework::get->drawingState = Dimension::THREE;

    glPopMatrix(); //From 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double) GFramework::get->windowSize.x / (double) GFramework::get->windowSize.y, 0.1f, GFramework::RENDERING_DISTANCE);
    glViewport(0, 0, GFramework::get->windowSize.x, GFramework::get->windowSize.y);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void Drawing::enableND(Dimension d) {
    switch(d) {
        case Dimension::NONE:
        case Dimension::TWO:   Drawing::enable2D(); break;
        case Dimension::THREE: Drawing::enable3D(); break;
        default: LOG(UNKNOWN_DIMENSION_MESSAGE, LogDegree::WARNING, LogType::GRAPHICS); Drawing::enable2D(); break;
    }
}
