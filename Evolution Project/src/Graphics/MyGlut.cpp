#include "myGlut.h"
#include "SOIL.h"

Tex glLoadTexture(std::string fileName) {
    return SOIL_load_OGL_texture (
                 fileName.c_str(),
                 SOIL_LOAD_AUTO,
                 SOIL_CREATE_NEW_ID,
                 SOIL_FLAG_INVERT_Y);
}
void glTexVert2f(double tx, double ty, double vx, double vy) {glTexVert2f(Vec2(tx, ty), Vec2(vx, vy));}
void glTexVert2f(Vec2 t, double vx, double vy) {glTexVert2f(t, Vec2(vx, vy));}
void glTexVert2f(double tx, double ty, Vec2 v) {glTexVert2f(Vec2(tx, ty), v);}
void glTexVert2f(Vec2 t, Vec2 v) {
    glTexCoord2f(t.x, t.y);
    glVertex2f(v.x, v.y);
}


void glTexVert3f(double tx, double ty, double vx, double vy, double vz) {glTexVert3f(Vec2(tx, ty), Vec(vx, vy, vz));}
void glTexVert3f(Vec2 t, double vx, double vy, double vz) {glTexVert3f(t, Vec(vx, vy, vz));}
void glTexVert3f(double tx, double ty, Vec v) {glTexVert3f(Vec2(tx, ty), v);}
void glTexVert3f(Vec2 t, Vec v) {
    glTexCoord2f(t.x, t.y);
    glVertex3f(v.x, v.y, v.z);
}
