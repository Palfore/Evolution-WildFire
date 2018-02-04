#ifdef __APPLE__
#include <GLUT/freeglut.h>
#else
#include <GL/freeglut.h>
#endif

#ifndef MYGLUT_H_INCLUDED
#define MYGLUT_H_INCLUDED

#include "MyMath.h"

typedef GLuint Tex;

Tex glLoadTexture(std::string fileName);
void saveScreenShot();

void glTexVert2f(double tx, double ty, double vx, double vy);
void glTexVert2f(Vec2 t, double vx, double vy);
void glTexVert2f(double tx, double ty, Vec2 v);
void glTexVert2f(Vec2 t, Vec2 v);

void glTexVert3f(double tx, double ty, double vx, double vy, double vz);
void glTexVert3f(Vec2 t, double vx, double vy, double vz);
void glTexVert3f(double tx, double ty, Vec v);
void glTexVert3f(Vec2, Vec v);

#endif // MYGLUT_H_INCLUDED
