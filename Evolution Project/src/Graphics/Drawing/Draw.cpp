#include "Drawing/Draw.h"
#include "MyGlut.h" // Glut
#include "Graphics.h" // Graphics::get().drawingState, Dimension
#include "Logger.h"

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
        Drawing::changeColor(Graphics::get().colorMap.at(C));
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
        Drawing::changeTexture(Graphics::get().textureMap.at(T));
    } catch (std::out_of_range) {
        LOG(UNKNOWN_APPROVED_TEXTURE_MESSAGE, LogDegree::WARNING, LogType::GRAPHICS);
    }
}

void Drawing::enable2D() {
    if (Graphics::get().drawingState == Dimension::TWO) return;
    Graphics::get().drawingState = Dimension::TWO;

    glMatrixMode(GL_MODELVIEW);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, Graphics::get().windowSize.x, Graphics::get().windowSize.y, 0);
    glViewport(0, 0, Graphics::get().windowSize.x, Graphics::get().windowSize.y);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}

void Drawing::enable3D() {
    if (Graphics::get().drawingState == Dimension::TWO) LOG(INVALID_DRAWING_ORDER_MESSAGE, LogDegree::FATAL, LogType::GRAPHICS);
    if (Graphics::get().drawingState == Dimension::THREE) return;
    Graphics::get().drawingState = Dimension::THREE;

    glPopMatrix(); //From 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double) Graphics::get().windowSize.x / (double) Graphics::get().windowSize.y, 0.1f, 1000);
    glViewport(0, 0, Graphics::get().windowSize.x, Graphics::get().windowSize.y);
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
