#include "Simulation.h"
#include "Graphics.h"
#include "Drawing/Shapes.h"
#include "Drawing/Text.h"
#include "Drawing/Objects.h"
#include "Logger.h"

Simulation::Simulation() : gameMode(INITIAL_GAME_MODE), inputType(INITIAL_INPUT_TYPE) {}
Simulation::~Simulation() {}

#include <iostream>

void Simulation::run() {
    if (inputType != InputType::DEFAULT) {/* Stop Data Processing */}

    /* Normal Procedure */
    DrawPlane<Appearance::GRASS>(10);
    DrawPlane<Appearance::BLACK>(10, 45);
    DrawMySpecificObject<Appearance::GREEN>(0.1, 0.2, 0.5, 0.5);


    DrawCircle<Appearance::GREEN>(0.1, 0.5, 0.2);
    DrawRectangle<Appearance::GRASS>(0.01, 0.5, 0.4, 0.4);
    DrawRectangle<Appearance::BUTTON>(0.1, 0.2, 0, 0);

    DrawString<Appearance::WHITE>("Type something, then hit enter", 0.5,  0.4);

    /* Start and stop getting user input */
    UserInput *userInput = &Graphics::get().userInput;
    if (inputType == InputType::DEFAULT) {
        if (userInput->inputString.empty()) {
            setInputType(InputType::ALPHA_NUMERIC_INPUT);
        } else if (userInput->isInputStringSubmitted()) {
            userInput->drawUserString(0.5, 0.5);
            DrawString<Appearance::WHITE>("Now you can move around", 0.5,  0.6);
        }
    }

    /* Updates based on user input */
    if (inputType == InputType::ALPHA_NUMERIC_INPUT) {
        DrawString<Appearance::WHITE>(userInput->inputString, 0.5,  0.5);
        if (userInput->isInputStringSubmitted()) {
            setInputType(InputType::DEFAULT);
            Graphics::get().audio.playSound("gunShot.wav");
        }
    }

//    static int i = 0;
//    if (i++ % 1000 == 0) {
//        Graphics::get().audio.playSound("gunShot.wav");
//        puts("her");
//    }
    Graphics::get().showScene();
}

void Simulation::init() {
    Simulation::loadGameModeKeyboard();
}

void Simulation::setGameMode(GameMode g) {
    Graphics::get().simulation.gameMode = g;
    Graphics::get().simulation.inputType = InputType::DEFAULT;
    loadGameModeKeyboard();
}

void Simulation::setInputType(InputType t) {
    Graphics::get().simulation.inputType = t;
    setInputTypeKeyboard();
}

void Simulation::setInputTypeKeyboard() {
    std::vector<UserFunction> *userFunctions = &Graphics::get().userInput.functions;
    Graphics::get().userInput.setToDefault();
    switch(inputType) {
        case InputType::DEFAULT:
            loadGameModeKeyboard();
            break;
        case InputType::BLOCKING_MESSAGE:
            userFunctions->push_back(UserFunction(ENTER, [](){
                Graphics::get().simulation.setInputType(InputType::DEFAULT); // Unlock Input
            }));
            break;
        case InputType::FATAL_MESSAGE:
            userFunctions->push_back(UserFunction(ENTER, [](){ // Fatal ending to game
                exit(EXIT_FAILURE);
            }));
            break;
        case InputType::NUMERIC_INPUT:
            Graphics::get().userInput.keyInputIsHeld = false;
            for (char input = '0'; input <= '9'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){Graphics::get().userInput.inputString += input;}));
            }
            userFunctions->push_back(UserFunction(BACKSPACE, [](){
                if (!Graphics::get().userInput.inputString.empty()) {
                    Graphics::get().userInput.inputString.pop_back();
                }
            }));
            userFunctions->push_back(UserFunction(ENTER, [](){
                Graphics::get().userInput.inputString += '#';
                Graphics::get().userInput.keyInputIsHeld = true;
            }));
            break;
        case InputType::ALPHA_NUMERIC_INPUT:
            Graphics::get().userInput.keyInputIsHeld = false;

            for (char input = '0'; input <= '9'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){Graphics::get().userInput.inputString += input;}));
            }
            for (char input = 'a'; input <= 'z'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){Graphics::get().userInput.inputString += input;}));
            }
            for (char input = 'A'; input <= 'Z'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){Graphics::get().userInput.inputString += input;}));
            }
            userFunctions->push_back(UserFunction(' ', [](){Graphics::get().userInput.inputString += ' ';}));
            userFunctions->push_back(UserFunction(BACKSPACE, [](){
                if (Graphics::get().userInput.inputString.size() > 0) {
                    Graphics::get().userInput.inputString.pop_back();
                }
            }));
            userFunctions->push_back(UserFunction(ENTER, [](){
                Graphics::get().userInput.inputString += '#';
                Graphics::get().userInput.keyInputIsHeld = true;
            }));
            break;
        default:
            LOG(LogDegree::WARNING, LogType::INPUT, "UNKNOWN INPUT TYPE KEYBOARD SELECTED.");
            break;
    }
}

void Simulation::loadGameModeKeyboard() {
    Graphics::get().userInput.setToDefault();
    std::vector<UserFunction> *userFunctions = &Graphics::get().userInput.functions;
    switch(gameMode) {
        case GameMode::MAIN_MENU:
            break;
        case GameMode::SIMULATION: { // Add changing playbackspeed
            Camera *camera = &Graphics::get().camera;

            /* Setting Controls */
            userFunctions->push_back(UserFunction(GLUT_KEY_F1, [](){Graphics::get().display ^= true;}));

            /* Camera Controls */
            userFunctions->push_back(UserFunction(GLUT_KEY_LEFT,  [camera](){
                camera->del.x = +camera->rotationSpeed; }, [camera](){camera->del.x = 0.0;
            }));
            userFunctions->push_back(UserFunction(GLUT_KEY_RIGHT, [camera](){
                camera->del.x = -camera->rotationSpeed; }, [camera](){camera->del.x = 0.0;
            }));
            userFunctions->push_back(UserFunction(GLUT_KEY_UP,    [camera](){
                camera->del.z = +camera->rotationSpeed; }, [camera](){camera->del.z = 0.0;
            }));
            userFunctions->push_back(UserFunction(GLUT_KEY_DOWN,  [camera](){
                camera->del.z = -camera->rotationSpeed; }, [camera](){camera->del.z = 0.0;
            }));

            /* Position Controls */
            userFunctions->push_back(UserFunction('w', [camera](){camera->mov.x =  camera->translationSpeed;}));
            userFunctions->push_back(UserFunction('s', [camera](){camera->mov.x = -camera->translationSpeed;}));
            userFunctions->push_back(UserFunction('a', [camera](){camera->mov.y = -camera->translationSpeed;}));
            userFunctions->push_back(UserFunction('d', [camera](){camera->mov.y =  camera->translationSpeed;}));
            userFunctions->push_back(UserFunction('e', [camera](){camera->mov.z =  camera->translationSpeed;}));
            userFunctions->push_back(UserFunction('z', [camera](){camera->mov.z = -camera->translationSpeed;}));
            break;
        }
        default:
            LOG(LogDegree::WARNING, LogType::INPUT, "UNKNOWN GAMEMODE SELECTED.");
            break;
    }
}
