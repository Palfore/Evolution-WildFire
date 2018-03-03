#include "Simulation.h"
#include "GFramework.h"
#include "Drawing/Shapes.h"
#include "Drawing/Text.h"
#include "Drawing/Objects.h"
#include "Logger.h"
#include "Audio.h"
#include "utility.h" // numToStr
#include <iostream>

Simulation::Simulation() : gameMode(INITIAL_GAME_MODE), inputType(INITIAL_INPUT_TYPE) {LOG("Initialized Simulation.");}
Simulation::~Simulation() {}


void Simulation::run(const double fps) {
    if (inputType != InputType::DEFAULT) {/* Stop Data Processing */}

    GFramework::get->readyDrawing();

    /* Normal Procedure */
    DrawPlane<Appearance::GRASS>(10);
    DrawPlane<Appearance::BLACK>(10, 45);
    DrawMySpecificObject<Appearance::GREEN>(0.1, 0.2, 0.5, 0.5);


    DrawCircle<Appearance::GREEN>(0.1, 0.5, 0.2);
    DrawRectangle<Appearance::GRASS>(0.01, 0.5, 0.4, 0.4);
    DrawRectangle<Appearance::BUTTON>(0.1, 0.2, 0, 0);

    DrawString<Appearance::WHITE>("Type something, then hit enter", 0.5,  0.4);
    DrawString<Appearance::WHITE>(utility::numToStr<int>(fps), 0.1,  0.1);

    /* Start and stop getting user input */
    UserInput *userInput = &GFramework::get->userInput;
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
            GFramework::get->audio->playSound("gunShot.wav");
        }
    }

    if (inputType == InputType::FATAL_MESSAGE) {
        DrawRectangle<Appearance::BUTTON>(0.2, 0.2, 0.8, 0.8);
        DrawString<Appearance::BLACK>("A fatal error occured. Press ENTER to quit.", 0.5, 0.5);
    }
    if (inputType == InputType::BLOCKING_MESSAGE) {
        DrawRectangle<Appearance::BUTTON>(0.2, 0.2, 0.8, 0.8);
        DrawString<Appearance::BLACK>("A warning has been logged. Press ENTER to continue.", 0.5, 0.5);
    }
    GFramework::get->showScene();
}

void Simulation::init() {
    Simulation::loadGameModeKeyboard();
}

void Simulation::setGameMode(GameMode g) {
    gameMode = g;
    inputType = InputType::DEFAULT;
    loadGameModeKeyboard();
}

void Simulation::setInputType(InputType t) {
    inputType = t;
    setInputTypeKeyboard();
}

void Simulation::setInputTypeKeyboard() {
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;
    GFramework::get->userInput.setToDefault();
    switch(inputType) {
        case InputType::DEFAULT:
            loadGameModeKeyboard();
            break;
        case InputType::BLOCKING_MESSAGE:
            userFunctions->push_back(UserFunction(ENTER, [this](){
                this->setInputType(InputType::DEFAULT); // Unlock Input
            }));
            break;
        case InputType::FATAL_MESSAGE:
            userFunctions->push_back(UserFunction(ENTER, [](){ // Fatal ending to game
                exit(EXIT_FAILURE);
            }));
            break;
        case InputType::NUMERIC_INPUT:
            GFramework::get->userInput.keyInputIsHeld = false;
            for (char input = '0'; input <= '9'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
            }
            userFunctions->push_back(UserFunction(BACKSPACE, [](){
                if (!GFramework::get->userInput.inputString.empty()) {
                    GFramework::get->userInput.inputString.pop_back();
                }
            }));
            userFunctions->push_back(UserFunction(ENTER, [](){
                GFramework::get->userInput.submitInputString();
                GFramework::get->userInput.keyInputIsHeld = true;
            }));
            break;
        case InputType::ALPHA_NUMERIC_INPUT:
            GFramework::get->userInput.keyInputIsHeld = false;

            for (char input = '0'; input <= '9'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
            }
            for (char input = 'a'; input <= 'z'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
            }
            for (char input = 'A'; input <= 'Z'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
            }
            userFunctions->push_back(UserFunction(' ', [](){GFramework::get->userInput.inputString += ' ';}));
            userFunctions->push_back(UserFunction(BACKSPACE, [](){
                if (GFramework::get->userInput.inputString.size() > 0) {
                    GFramework::get->userInput.inputString.pop_back();
                }
            }));
            userFunctions->push_back(UserFunction(ENTER, [](){
                GFramework::get->userInput.submitInputString();
                GFramework::get->userInput.keyInputIsHeld = true;
            }));
            break;
        default:
            LOG(UNKNOWN_INPUT_TYPE_MESSAGE, LogDegree::WARNING, LogType::INPUT);
            break;
    }
}

void Simulation::loadGameModeKeyboard() {
    GFramework::get->userInput.setToDefault();
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;
    switch(gameMode) {
        case GameMode::MAIN_MENU:
            break;
        case GameMode::SIMULATION: {
            Camera *camera = GFramework::get->camera;

            /* Setting Controls */
            userFunctions->push_back(UserFunction(GLUT_KEY_F1, [](){GFramework::get->display ^= true;}));

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
            LOG(UNKNOWN_GAME_MODE_MESSAGE, LogDegree::WARNING, LogType::INPUT);
            break;
    }
}
