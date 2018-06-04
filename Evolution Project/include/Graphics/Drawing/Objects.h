#ifndef OBJECTS_H
#define OBJECTS_H

#include "Drawing/Draw.h"
#include "GFramework.h"


struct DrawSkybox : DrawItem<Appearance::WHITE> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(3);

    DrawSkybox(double distance) : DrawItem<Appearance::WHITE>(dimension) {
        draw(distance);
    }
    private:void draw(double l) {
        Vec c = GLOBAL->camera->pos;
        c.z -= 25;
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);

        double e = 0.001;
        double f = 1.0-e;
        glBindTexture(GL_TEXTURE_2D, GLOBAL->textureMap[Appearance::SKYBOX_PX]);
        glBegin(GL_QUADS);
            glTexCoord2f(  e,   e); glVertex3f(-l+c.x,  l+c.y, -l+c.z);
            glTexCoord2f(  e,   f); glVertex3f(-l+c.x,  l+c.y,  l+c.z);
            glTexCoord2f(  f,   f); glVertex3f( l+c.x,  l+c.y,  l+c.z);
            glTexCoord2f(  f,   e); glVertex3f( l+c.x,  l+c.y, -l+c.z);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, GLOBAL->textureMap[Appearance::SKYBOX_PZ]);
        glBegin(GL_QUADS);
            glTexCoord2f(  e,   e); glVertex3f( -l+c.x, -l+c.y, -l+c.z);
            glTexCoord2f(  e,   f); glVertex3f( -l+c.x, -l+c.y,  l+c.z);
            glTexCoord2f(  f,   f); glVertex3f( -l+c.x,  l+c.y,  l+c.z);
            glTexCoord2f(  f,   e); glVertex3f( -l+c.x,  l+c.y, -l+c.z);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, GLOBAL->textureMap[Appearance::SKYBOX_PY]);
        glBegin(GL_QUADS);
            glTexCoord2f(  e,   e); glVertex3f( -l+c.x,  -l+c.y, l+c.z);
            glTexCoord2f(  f,   e); glVertex3f( -l+c.x,   l+c.y, l+c.z);
            glTexCoord2f(  f,   f); glVertex3f( l+c.x,    l+c.y, l+c.z);
            glTexCoord2f(  e,   f); glVertex3f( l+c.x,   -l+c.y, l+c.z);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, GLOBAL->textureMap[Appearance::SKYBOX_MX]);
        glBegin(GL_QUADS);
            glTexCoord2f(  f,   e); glVertex3f(-l+c.x,   -l+c.y, -l+c.z);
            glTexCoord2f(  f,   f); glVertex3f(-l+c.x,   -l+c.y,  l+c.z);
            glTexCoord2f(  e,   f); glVertex3f( l+c.x,   -l+c.y,  l+c.z);
            glTexCoord2f(  e,   e); glVertex3f( l+c.x,   -l+c.y, -l+c.z);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, GLOBAL->textureMap[Appearance::SKYBOX_MZ]);
        glBegin(GL_QUADS);
            glTexCoord2f(  f,   e); glVertex3f( l+c.x,  -l+c.y, -l+c.z);
            glTexCoord2f(  f,   f); glVertex3f( l+c.x,  -l+c.y,  l+c.z);
            glTexCoord2f(  e,   f); glVertex3f( l+c.x,   l+c.y,  l+c.z);
            glTexCoord2f(  e,   e); glVertex3f( l+c.x,   l+c.y, -l+c.z);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, GLOBAL->textureMap[Appearance::SKYBOX_MY]);
        glBegin(GL_QUADS);
            glTexCoord2f(  e,   f); glVertex3f( -l+c.x,  -l+c.y, -l+c.z);
            glTexCoord2f(  f,   f); glVertex3f( -l+c.x,   l+c.y, -l+c.z);
            glTexCoord2f(  f,   e); glVertex3f( l+c.x,    l+c.y, -l+c.z);
            glTexCoord2f(  e,   e); glVertex3f( l+c.x,   -l+c.y, -l+c.z);
        glEnd();


        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    }
};


#endif // OBJECTS_H
