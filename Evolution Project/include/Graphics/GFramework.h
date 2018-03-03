#ifndef GFramework_H
#define GFramework_H

#include "myGlut.h"
#include "Math/MyMath.h" // Vec2
#include "UserInput.h" // UserInput
#include "Drawing/Draw.h" // enum Appearance

#include <memory> // unique_ptr

class Audio;
class Simulation;
class Camera;


/** @brief This class contains information about the user's mouse.
    @details Specifically, its position, whether is had been clicked, and if its held down.
    It is not fully implemented.
 */
struct Mouse {
    int x;          ///< x coordinate of mouse (from left to right)
    int y;          ///< y coordinate of mouse (from top to bottom)
    bool clicked;   ///< Is the mouse clicked?
    bool heldDown;  ///< Is the mouse button held down?

    Mouse() : x(0), y(0), clicked(false), heldDown(false) {};
};


/** @brief This class holds all the functions required for a 3D simulator to be run.
    @details It includes camera, audio, user input amoung other things. This class implements
    the singleton pattern and as a result only one instance is ever allowed.
 */
class GFramework {
    public:
        Vec2 windowSize;         ///< (width, height) dimensions of the window.
        Drawing::Dimension drawingState;  ///< What dimension the GFramework is prepared to draw in.
        Camera * camera;           ///< The camera on the users head.
        Mouse mouse;             ///< The mouse the user controls.
        UserInput userInput;     ///< Set of user inputs.
        Audio * audio;             ///< Audio system for the engine.

        Simulation * simulation;   ///< The simulation to be run in this framework.

        bool display = true;     ///< Whether the scene is being shown to the user. (set to false for maximum speed)

        static double constexpr FPS = 60;                ///< Frames per second for the simulation to run at.
        static int constexpr RENDERING_DISTANCE = 1000;  ///< How far objects should be rendered.

        std::map<enum Appearance, Tex> textureMap = {};  ///< Maps an appearance to a texture for drawing.
        std::map<enum Appearance, Vec> colorMap = {};    ///< Maps an appearance to a color for drawing.


        /** @brief Initialize simulation and start the glutmainloop
         *
         * @return void
         *
         */
        void startup();

        /** @brief Sets the proper display settings & calls glutswapbuffers.
         *
         * @return void
         *
         */
        void showScene();

        /** @brief Prepares the scene for the next frame.
         *
         * @return  void
         *
         */
        void readyDrawing();

        static std::unique_ptr<GFramework> const get;   ///< Singleton instance. Unique_ptr insures destruction.
        ~GFramework();                                  ///< Destructor deletes singleton instance
        GFramework(GFramework const&)      = delete;    ///< Copy constructor is deleted in singleton.
        void operator=(GFramework const&)  = delete;    ///< Assignment operator is deleted in singleton.

    private:
        static char constexpr const * WINDOW_TITLE = "Evolution WildFire";  ///< Name of the window / game.
        static int constexpr INIT_WINDOW_X = 600;                           ///< Initial x position of the window on start up.
        static int constexpr INIT_WINDOW_Y = 100;                           ///< Initial y position of the window on start up.
        static int constexpr INIT_WINDOW_WIDTH = 1920 / 2.0;                ///< Initial width of the window on start up.
        static int constexpr INIT_WINDOW_HEIGHT = 1080 / 2.0;               ///< Initial height of the window on start up.

        GFramework(); ///< Constructor initializes freeglut and loads appearances into maps. Private for singleton pattern.

        void initializeGlut(); ///< Intializes all the freeglut functions with callbacks.
        void loadTextures();   ///< Loads all the textures into the framework.
        void loadColors();     ///< Loads all the colors into the framework.
};


/** @brief This class describes the camera placed at the head of the user.
    @todo In future, the camera implementation should be removed from the graphics class
    and put here.
 */
struct Camera {
    static double constexpr DEFAULT_T_SPEED = 60.0 / GFramework::FPS;  ///< The default translation speed.
    static double constexpr DEFAULT_R_SPEED = 2.40 / GFramework::FPS; ///< The default rotation speed.
    static double constexpr DEFAULT_HEIGHT = 1.8;   ///< The default height of the camera (wrt z=0)

    double translationSpeed;  ///< How fast the camera moves forward
    double rotationSpeed;     ///< How fast the camera rotates
    Vec pos;  ///< The global coordinates of the camera.
    Vec mov;  ///< The next frame movement of the camera.
    Vec dir;  ///< The direction the camera is pointing to.
    Vec ang;  ///< The angle around the z axis, starting at +x. (I think)
    Vec del;  ///< The next frame rotation of the camera.

    Camera() : translationSpeed(DEFAULT_T_SPEED), rotationSpeed(DEFAULT_R_SPEED),
                        pos(0, -40, DEFAULT_HEIGHT), mov(0, 0, DEFAULT_HEIGHT), dir(0, 1, 0), ang(0,0,0), del(0,0,0) {};
};

#endif // GFramework_H
