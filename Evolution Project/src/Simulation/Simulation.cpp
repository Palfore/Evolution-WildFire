#include "Simulation.h"
#include "Graphics.h"
#include "Drawing/Shapes.h"

#include "Logger.h"

Simulation::Simulation() : gameMode(INITIAL_GAME_MODE) {}
Simulation::~Simulation() {}

void Simulation::run() {
    if (gameMode == GameMode::BLOCKING_MESSAGE || gameMode == GameMode::FATAL_MESSAGE) return;

    DrawPlane<Appearance::GRASS>(10);
    DrawPlane<Appearance::BLACK>(10, 45);

    DrawRectangle<Appearance::GREEN>(500, 50, 300, 300);
    DrawRectangle<Appearance::GRASS>(50, 50, 300, 300);
    DrawRectangle<Appearance::BUTTON>(200, 100, 0, 0);

    Graphics::get().showScene();
}

void Simulation::init() {
    Simulation::loadKeyboard();
}

void Simulation::changeGameMode(GameMode g) {
    Graphics::get().simulation.gameMode = g;
    loadKeyboard();
}

void Simulation::loadKeyboard() {
    Graphics::get().setDefaultUserFunctions();
    std::vector<UserFunction> *userFunctions = &Graphics::get().userFunctions;
    switch(gameMode) {
        case MAIN_MENU:
        case SIMULATION: { // Add changing playbackspeed
            /* Setting Controls */
            userFunctions->push_back(UserFunction('i', [](){Graphics::get().display ^= true;}));

            /* Camera Controls */
            userFunctions->push_back(UserFunction(GLUT_KEY_LEFT, [](){
                Graphics::get().camera.del.x =  Graphics::get().camera.rotationSpeed;}, [](){
                Graphics::get().camera.del.x = 0.0;}));
            userFunctions->push_back(UserFunction(GLUT_KEY_RIGHT, [](){
                Graphics::get().camera.del.x = -Graphics::get().camera.rotationSpeed;}, [](){
                Graphics::get().camera.del.x = 0.0;}));
            userFunctions->push_back(UserFunction(GLUT_KEY_UP, [](){
                Graphics::get().camera.del.z =  Graphics::get().camera.rotationSpeed;}, [](){
                Graphics::get().camera.del.z = 0.0;}));
            userFunctions->push_back(UserFunction(GLUT_KEY_DOWN, [](){
                Graphics::get().camera.del.z = -Graphics::get().camera.rotationSpeed;}, [](){
                Graphics::get().camera.del.z = 0.0;}));

            /* Position Controls */
            userFunctions->push_back(UserFunction('w', [](){Graphics::get().camera.mov.x =  Graphics::get().camera.translationSpeed;}));
            userFunctions->push_back(UserFunction('s', [](){Graphics::get().camera.mov.x = -Graphics::get().camera.translationSpeed;}));
            userFunctions->push_back(UserFunction('a', [](){Graphics::get().camera.mov.y = -Graphics::get().camera.translationSpeed;}));
            userFunctions->push_back(UserFunction('d', [](){Graphics::get().camera.mov.y =  Graphics::get().camera.translationSpeed;}));
            userFunctions->push_back(UserFunction('e', [](){Graphics::get().camera.mov.z =  Graphics::get().camera.translationSpeed;}));
            userFunctions->push_back(UserFunction('z', [](){Graphics::get().camera.mov.z = -Graphics::get().camera.translationSpeed;}));
            break;
        }
        case BLOCKING_MESSAGE:
            userFunctions->push_back(UserFunction((char) 13, [](){
                Graphics::get().simulation.changeGameMode(GameMode::SIMULATION);
            }));
            break;
        case FATAL_MESSAGE:
            userFunctions->push_back(UserFunction((char) 13, [](){
                exit(EXIT_FAILURE);
            }));
        default:
            break;
    }
}
