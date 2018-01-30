#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <time.h> // srand(time(nullptr))
#include <functional>
#include <vector>

#include "myGlut.h"
#include "Math/MyMath.h" // Vec2
#include "Simulation.h"
#include "Drawing/Draw.h" // enum Appearance
#include "UserInput.h" // UserInput
#include "Graphics/Audio/Audio.h"

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

class Graphics {
    public:
        Vec2 windowSize;
        Dimension drawingState;
        Camera camera;
        Mouse mouse;
        UserInput userInput;
        Audio audio;

        Simulation simulation;

        static double constexpr FPS = 60;
        static int constexpr RENDERING_DISTANCE = 1000;

        bool fullscreen = false;
        bool display = true;

        void toggleFullScreen();

        void run();
        void showScene();

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

        Graphics();
        void initializeGlut();
        void loadTextures();
        void loadColors();
};


#endif // GRAPHICS_H
