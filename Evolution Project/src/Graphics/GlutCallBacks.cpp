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
            GFramework::get->simulation->run(&GFramework::get->userInput.functions, 1);
        } else {
            static auto getTime = [](){  // in seconds
                auto time = std::chrono::system_clock::now().time_since_epoch();
                auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
                return microseconds / 1000000.0;
            };
//            static auto avg = [](std::vector<double> values) {
//                double sum = 0.0;
//                for (double value : values) {
//                    sum += value;
//                }
//                return sum / double(values.size());
//            };


            ////////////////////////////////////////
            static double lastFrameTime = 0; // This assumes that the background time is negligable.

            double start = getTime();

            GFramework::get->simulation->run(&GFramework::get->userInput.functions, lastFrameTime);
            std::this_thread::sleep_for(std::chrono::milliseconds((int) 10));

            double fps = GFramework::FPS;
            double stop = getTime();
            if ((1/fps - lastFrameTime) > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds((int) (
                    (1/fps) - lastFrameTime
                )));
            }
            lastFrameTime = stop - start;
            //printf("%f %f %f\n", 1/fps, lastFrameTime, (1/fps - lastFrameTime)*fps);
        }
    }

    void update() {
        glutPostRedisplay(); // redisplay everything, maybe this should be moved
        GFramework::get->windowSize = Vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); // maybe move to redisplay

        for (auto & func : GFramework::get->userInput.functions) {
            func.update();
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
    }

    void callMouse(int button, int state, int mx, int my) {
        for (auto & func : GFramework::get->userInput.functions) {
            if (button == GLUT_LEFT_BUTTON) {// only handle left clicks
                switch(state) {
                    case GLUT_DOWN: func.element->affectState(mx, my, CALL_TYPE::ACTION); break;
                    case GLUT_UP: func.element->affectState(mx, my, CALL_TYPE::RELEASE); break;
                    default: break;
                }
            }
        }
        if (button && state) return;
        if (mx && my) return;
    }

    void mouseMove(int mx, int my) {
        if (mx && my) return;
    }

    void passiveMouse(int x, int y) {
        for (auto & func : GFramework::get->userInput.functions) {
            func.element->affectState(x, y, CALL_TYPE::NONE);
        }
        GFramework::get->mouse.x = x;
        GFramework::get->mouse.y = y;
//        std::cout << x << ',' << y << '\n';
        GFramework::get->camera->del.setToZero(); // Prevent motion after mouse release (if used to move camera)
    }

    void keyPressed(unsigned char key, int x, int y) {
        for (auto & func : GFramework::get->userInput.functions) {
            func.element->affectState(key, CALL_TYPE::ACTION);
        }
        if (x && y){}
    }

    void keyUp(unsigned char key, int x, int y) {
        for (auto & func : GFramework::get->userInput.functions) {
            func.element->affectState(key, CALL_TYPE::RELEASE);
        }
        if (x && y) return;
    }

    void pressSpecialKey(int key, int kxx, int kyy) {
        for (auto & func : GFramework::get->userInput.functions) {
            func.element->affectState(key, CALL_TYPE::ACTION);
        }
        if (kxx && kyy) return;
    }

    void releaseSpecialKey(int key, int kx, int ky) {
        for (auto & func : GFramework::get->userInput.functions) {
            func.element->affectState(key, CALL_TYPE::RELEASE);
        }
        if (kx && ky) return;
    }
}

