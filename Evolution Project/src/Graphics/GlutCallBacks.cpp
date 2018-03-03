#include "glutCallBacks.h"
#include "GFramework.h" // Glut functions
#include <SFML/Audio.hpp> //sf::Listener::setPosition, sf::Listener::setDirection
#include "Simulation.h"

#include <chrono>
#include <iostream>
#include <thread>
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
        /* Run Simulation */
        if (!GFramework::get->display) { // Run asap
            GFramework::get->simulation->run(0);
        } else {
            static auto getTime = [](){
                auto time = std::chrono::system_clock::now().time_since_epoch();
                auto seconds = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
                return seconds / 1000000.0;
            };

            static unsigned int COLLECTED_FRAMES = 60;
            static std::vector<double> times = {};

            times.push_back(getTime());
            double currentFPS = 0;
            if (times.size() == COLLECTED_FRAMES) {
                double delta = times[times.size() - 1] - times[0];
                currentFPS = COLLECTED_FRAMES / delta;
                double deviation = currentFPS - GFramework::get->FPS;
                int milliseconds = deviation > 0 ? deviation : 0;

                std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            }
            while (times.size() >= COLLECTED_FRAMES) {
                times.erase(times.begin());
            }
            GFramework::get->simulation->run(currentFPS);
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

        Camera *camera = GFramework::get->camera;

        /* Update Audio */
//        GFramework::get->audio.clearStoppedSounds();
        sf::Listener::setPosition(GFramework::get->camera->pos.x, GFramework::get->camera->pos.y, GFramework::get->camera->pos.z);
        sf::Listener::setDirection(GFramework::get->camera->dir.x, GFramework::get->camera->dir.y, GFramework::get->camera->dir.z);

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
        GFramework::get->camera->del.setToZero(); // Prevent motion after mouse release (if used to move camera)
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

