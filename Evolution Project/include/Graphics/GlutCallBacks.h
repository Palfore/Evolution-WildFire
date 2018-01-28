#ifndef GLUTCALLBACKS_H
#define GLUTCALLBACKS_H

namespace glutCB {
    void renderScene();
    void update();
    void changeSize(int w, int h);
    void callMouse(int button, int state, int mx, int my);
    void mouseMove(int mx, int my);
    void passiveMouse(int mx, int my);
    void keyPressed(unsigned char key, int x, int y);
    void keyUp(unsigned char key, int x, int y);
    void pressSpecialKey(int key, int kxx, int kyy);
    void releaseSpecialKey(int key, int kx, int ky);
}

#endif // GLUTCALLBACKS_H
