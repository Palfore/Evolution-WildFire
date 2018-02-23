#include "glutCallBacks.h"
#include "GFramework.h" // Glut functions
#include <SFML/Audio.hpp> //sf::Listener::setPosition, sf::Listener::setDirection
#include "Simulation.h"

namespace glutCB {
    void changeSize(int w, int h) {
        double windowRatio = w / static_cast<double>(h); // window aspect ratio
        glMatrixMode(GL_PROJECTION); // projection matrix is active
        glLoadIdentity(); // reset the projection
        gluPerspective(45.0, windowRatio, 0.1, GFramework::RENDERING_DISTANCE); // perspective transformation
        glMatrixMode(GL_MODELVIEW); // return to modelview mode
        glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
    }

    void renderScene() {
        /* Clear, reset, camera */
        glClearColor(0.00, 0.75, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        auto camera = GFramework::get->camera;
        gluLookAt(              camera.pos.x,                 camera.pos.y,                 camera.pos.z,
                 camera.pos.x + camera.dir.x,  camera.pos.y + camera.dir.y,  camera.pos.z + camera.dir.z,
                                         0.0,                          0.0,                          1.0);
        /* Run Simulation */
        if (!GFramework::get->display) { // Run asap
            GFramework::get->simulation->run();
        } else {
            /* Keep FPS Constant */
            const int msPerFrame = 1000.0 / GFramework::FPS;
            int init = glutGet(GLUT_ELAPSED_TIME);

            GFramework::get->simulation->run();

            int total = glutGet(GLUT_ELAPSED_TIME);
            while (total - init < msPerFrame) {
                GFramework::get->simulation->run();
                total = glutGet(GLUT_ELAPSED_TIME);
            }
            init = glutGet(GLUT_ELAPSED_TIME);
        }
    }

    void update() {
        glutPostRedisplay(); // redisplay everything, maybe this should be moved
        GFramework::get->windowSize = Vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); // maybe move to redisplay

        for (auto const& func : GFramework::get->userInput.functions) {
            if (GFramework::get->userInput.keyStates[static_cast<int>(func.key)]) func.action(); // key holds
        }

        double eps = 1e-10;
        if (!GFramework::get->display) return;

        Camera *camera = &GFramework::get->camera;

        /* Update Audio */
//        GFramework::get->audio.clearStoppedSounds();
        sf::Listener::setPosition(GFramework::get->camera.pos.x, GFramework::get->camera.pos.y, GFramework::get->camera.pos.z);
        sf::Listener::setDirection(GFramework::get->camera.dir.x, GFramework::get->camera.dir.y, GFramework::get->camera.dir.z);

        /* Update Position */// Note: Not in orthogonal directions
        // Move 'Forward / Backwards'
        if (camera->mov.x > eps || camera->mov.x < -eps) { // Prevents going underground
            camera->pos.x += camera->mov.x * camera->dir.x;
            camera->pos.y += camera->mov.x * camera->dir.y;
            camera->pos.z += camera->pos.z + camera->mov.x * camera->dir.z > 0 ? camera->mov.x * camera->dir.z : 0;
        }
        // Move 'Left / Right'
        if (camera->mov.y > eps || camera->mov.y < -eps) { // Cross product with Up vector
            camera->pos.x += camera->mov.y * camera->dir.y;
            camera->pos.y -= camera->mov.y * camera->dir.x;
        }

        // Move 'Up / Down'
        if (camera->mov.z > eps || camera->mov.z < -eps) { // Simply move Up
            camera->pos.z += camera->pos.z + camera->mov.z > 0.0 ? camera->mov.z : 0; // ground->0
        }

        /* Update Viewing Angle */
        // Pan 'Left / Right'
        if (camera->del.x > eps || camera->del.x < -eps) { // Should rename to phi and theta
            camera->ang.x += camera->del.x;
            camera->dir.x = -sin(camera->ang.x);
            camera->dir.y = cos(camera->ang.x);
        }

        // Pan 'Up / Down'
        if (camera->del.z > eps || camera->del.z < -eps) {  // Restrict angle to PI / 2
            if ((camera->ang.z + camera->del.z <  0.5*PI) && (camera->del.z > 0)) camera->ang.z += camera->del.z;
            if ((camera->ang.z - camera->del.z > -0.5*PI) && (camera->del.z < 0)) camera->ang.z += camera->del.z;
            camera->dir.z = sin(camera->ang.z);
        }

        camera->mov.setToZero();
    }

    void callMouse(int button, int state, int mx, int my) {
        if (button && state) return;
        if (mx && my) return;
    }

    void mouseMove(int mx, int my) {
        if (mx && my) return;
    }

    void passiveMouse(int x, int y) {
        GFramework::get->mouse.x = x;
        GFramework::get->mouse.y = y;
        GFramework::get->camera.del.setToZero(); // Prevent motion after mouse release (if used to move camera)
    }

    void keyPressed(unsigned char key, int x, int y) {
        if (key & x * y){}
        if (GFramework::get->userInput.keyInputIsHeld) {
            GFramework::get->userInput.keyStates[tolower(key)] = true; // Set the state of the current key to pressed for holding
        }
        for (auto const& func : GFramework::get->userInput.functions) {
            if (func.key == key) func.action(); // single press
        }
    }

    void keyUp(unsigned char key, int x, int y) {
        GFramework::get->userInput.keyStates[tolower(key)] = false; // Release the state of the current key to pressed for holding
        for (auto const& func : GFramework::get->userInput.functions) {
            if (func.key == key) func.release();
        }
        if (key && x && y) return;
    }

    void pressSpecialKey(int key, int kxx, int kyy) { // not yet implemented
        for (auto const& func : GFramework::get->userInput.functions) {
            if (func.specialKey == key) func.action();
        }
        if (key && kxx && kyy) return;
    }

    void releaseSpecialKey(int key, int kx, int ky) { // not yet implemented
        for (auto const& func : GFramework::get->userInput.functions) {
            if (func.specialKey == key) func.release();
        }
        if (key && kx && ky) return;
    }
}

