#include "Drawing/Draw.h"
#include "MyGlut.h" // Glut
#include "Graphics.h" // Graphics::get().drawingState, Dimension
#include "Logger.h"

bool Drawing::isColor(Appearance A) {
    if (A < 0) return false;
    if (A >= Appearance::LAST__) return false;
    return true;
}

bool Drawing::isTexture(Appearance A) {
    if (A < Appearance::LAST__) return false;
    if (A >= Appearance::ENDING__) return false;
    return true;
}

void Drawing::changeColor(Vec c) {
    glColor3f(c.x, c.y, c.z);
}

void Drawing::changeColor(Appearance C) {
    if (!Drawing::isColor(C)) LOG(LogDegree::WARNING, LogType::GRAPHICS, "Color is not valid.");
    Drawing::changeColor(Graphics::get().colorMap.at(C)); // this is not safe if C not in colormap
}

void Drawing::changeTexture(Tex textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Drawing::changeTexture(Appearance T) {
    if (!Drawing::isTexture(T)) LOG(LogDegree::WARNING, LogType::GRAPHICS, "Texture is not valid.");
    Drawing::changeTexture(Graphics::get().textureMap.at(T)); // this is not safe if C is no in textures
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
    if (Graphics::get().drawingState == Dimension::TWO) LOG(LogDegree::FATAL, LogType::GRAPHICS, "Cannot render 2D graphics before 3D.");
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
        default: LOG(LogDegree::WARNING, LogType::GRAPHICS, "Invalid number of dimensions, defaulting to 2D"); Drawing::enable2D(); break;
    }
}
