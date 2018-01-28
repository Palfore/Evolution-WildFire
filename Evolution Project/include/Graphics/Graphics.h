#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <time.h> // srand(time(nullptr))
#include <functional>
#include <vector>

#include "myGlut.h"
#include "Math/MyMath.h" // Vec2
#include "Simulation.h"
#include "Drawing/Draw.h" // enum Appearance

typedef std::function<void()> Action;
struct Camera {
    static double constexpr INIT_T_SPEED = 1.0;
    static double constexpr INIT_R_SPEED = 0.04;
    static double constexpr HEIGHT = 1.8;

    double translationSpeed;
    double rotationSpeed;
    Vec pos;
    Vec mov;
    Vec dir;
    Vec ang;
    Vec del;

    Camera() : translationSpeed(INIT_T_SPEED), rotationSpeed(INIT_R_SPEED),
                        pos(0, -40, HEIGHT), mov(0, 0, HEIGHT), dir(0, 1, 0), ang(0,0,0), del(0,0,0) {};
};

struct Mouse {
    int x;
    int y;
    bool clicked;

    Mouse() : x(0), y(0), clicked(false) {};
};

struct UserFunction { // Add Button Functionality -> specify button & move to GUI Folder
    int specialKey;
    char key;
    Action action;
    Action release;

    UserFunction();
    UserFunction(char key_t, Action action_t);
    UserFunction(int specialKey_t, Action action_t);
    UserFunction(int specialKey_t, Action action_t, Action release_t);
};


class Graphics {
    public:
        Vec2 windowSize;
        Dimension drawingState;
        Camera camera;
        Mouse mouse;
        Simulation simulation;

        std::vector<UserFunction> userFunctions;

        static double constexpr FPS = 60;
        bool fullscreen = false;
        bool display = true;

        void run();
        void showScene();

        void setDefaultUserFunctions();

        std::map<enum Appearance, Tex> textureMap = {};
        std::map<enum Appearance, Vec> colorMap = {};

        static Graphics& get() { // Singleton Pattern
            static Graphics instance;
            return instance;
        }
        ~Graphics();
        Graphics(Graphics const&)        = delete;
        void operator=(Graphics const&)  = delete;

    private:
        static char constexpr* WINDOW_TITLE = (char*) "Evolution WildFire";
        static int constexpr INIT_WINDOW_X = 600;
        static int constexpr INIT_WINDOW_Y = 100;
        static int constexpr INIT_WINDOW_WIDTH = 800;
        static int constexpr INIT_WINDOW_HEIGHT = 400;

        static int constexpr NUM_KEYS = 256;

        Graphics();
        void initializeGlut();
        void loadTextures();
        void loadColors();
    public:
        bool keyStates[NUM_KEYS] = {false}; // keypresses
};


#endif // GRAPHICS_H
