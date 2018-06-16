#ifndef OBJECTS_H
#define OBJECTS_H

#include "Drawing/Draw.h"
#include "GFramework.h"

/** @brief This will draw a textured cube at a fixed distance from the user.
 * @details This cube will follow the user so that it appears static. The distance parameter will therefore
 * determine the rendering distance in a sense. However because of the corners being farther, this is not exact.
 * This requires that all of Appearance::SKYBOX_{P,M}{X,Y,Z} be set, valid and in order.
 * @return
 *
 */
struct DrawSkybox : DrawItem<Appearance::WHITE> {
    static Drawing::Dimension constexpr dimension = static_cast<Drawing::Dimension>(3);

    /** @brief Draws the skybox at a fixed distance from the user.
     * @param distance The distance away from the user.
     * @return
     *
     */
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
