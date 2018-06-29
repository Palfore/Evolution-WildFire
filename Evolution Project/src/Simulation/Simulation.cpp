#include "Simulation.h"
#include "GFramework.h"
#include "Drawing/Shapes.h"
#include "Drawing/Text.h"
#include "Drawing/Objects.h"
#include "Logger.h"
#include "Audio.h"
#include "utility.h" // numToStr
#include <iostream>
#include "version.h"

Simulation::Simulation() : gameMode(INITIAL_GAME_MODE), inputType(INITIAL_INPUT_TYPE) {
    std::cout << "Compiled With GCC: " << __GNUC__ << '.' << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__ << " (" << __VERSION__ << ')' << '\n';
    std::cout << "Running Version: " << AutoVersion::FULLVERSION_STRING << '\n';
    LOG("Initialized Simulation.");
}
Simulation::~Simulation() {}

#include "Creature.h"
#include <ctime>
#include <iostream>

#include "Genome.h"


#include "Population.h"
#include "NodeGene.h"
#include "Shapes.h"
#include "MainMenu.h"
void Simulation::run(std::vector<UserFunction> * userFunctions, const double fps) {
    GFramework::get->readyDrawing();

    if (userFunctions != nullptr) {}
    if (inputType == InputType::DEFAULT) { // should pass userFunctions to methods
        switch (this->gameMode) {
            case GameMode::MAIN_MENU: Simulation::mainMenuMode(); break;
            case GameMode::EVOLVE: Simulation::evolveMode(fps); break;
            default:
                LOG("Unknown GameMode.", LogDegree::FATAL, LogType::GENERAL);
                break;
        }
    }

    if (inputType == InputType::FATAL_MESSAGE) {
        DrawRectangle<Appearance::BUTTON>(0.2, 0.2, 0.8, 0.8);
        DrawString<Appearance::BLACK>("A fatal error occured. Press ENTER to quit.", 0.5, 0.5, by_percentage());
    }
    if (inputType == InputType::BLOCKING_MESSAGE) {
        DrawRectangle<Appearance::BUTTON>(0.2, 0.2, 0.8, 0.8);
        DrawString<Appearance::BLACK>("A warning has been logged. Press ENTER to continue.", 0.5, 0.5, by_percentage());
    }


    GFramework::get->userInput.draw();
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
//    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;
    GFramework::get->userInput.setToDefault();
//    switch(inputType) {
//        case InputType::DEFAULT:
//            loadGameModeKeyboard();
//            break;
//        case InputType::BLOCKING_MESSAGE:
//            userFunctions->push_back(UserFunction(ENTER, [&](){
//                this->setInputType(InputType::DEFAULT); // Unlock Input
//            }));
//            break;
//        case InputType::FATAL_MESSAGE:
//            userFunctions->push_back(UserFunction(ENTER, [](){ // Fatal ending to game
//                exit(EXIT_FAILURE);
//            }));
//            break;
//        case InputType::NUMERIC_INPUT:
//            GFramework::get->userInput.keyInputIsHeld = false;
//            for (char input = '0'; input <= '9'; input++) {
//                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
//            }
//            userFunctions->push_back(UserFunction(BACKSPACE, [](){
//                if (!GFramework::get->userInput.inputString.empty()) {
//                    GFramework::get->userInput.inputString.pop_back();
//                }
//            }));
//            userFunctions->push_back(UserFunction(ENTER, [](){
//                GFramework::get->userInput.submitInputString();
//                GFramework::get->userInput.keyInputIsHeld = true;
//            }));
//            break;
//        case InputType::ALPHA_NUMERIC_INPUT:
//            GFramework::get->userInput.keyInputIsHeld = false;
//
//            for (char input = '0'; input <= '9'; input++) {
//                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
//            }
//            for (char input = 'a'; input <= 'z'; input++) {
//                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
//            }
//            for (char input = 'A'; input <= 'Z'; input++) {
//                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
//            }
//            userFunctions->push_back(UserFunction(' ', [](){GFramework::get->userInput.inputString += ' ';}));
//            userFunctions->push_back(UserFunction(BACKSPACE, [](){
//                if (GFramework::get->userInput.inputString.size() > 0) {
//                    GFramework::get->userInput.inputString.pop_back();
//                }
//            }));
//            userFunctions->push_back(UserFunction(ENTER, [](){
//                GFramework::get->userInput.submitInputString();
//                GFramework::get->userInput.keyInputIsHeld = true;
//            }));
//            break;
//        default:
//            LOG(UNKNOWN_INPUT_TYPE_MESSAGE, LogDegree::WARNING, LogType::INPUT);
//            break;
//    }
}

void Simulation::loadGameModeKeyboard() {
    GFramework::get->userInput.setToDefault();
    switch(gameMode) {
        case GameMode::MAIN_MENU: loadMainMenu(); break;
        case GameMode::EVOLVE: loadEvolve(); break;
        default:
            LOG(UNKNOWN_GAME_MODE_MESSAGE, LogDegree::WARNING, LogType::GENERAL);
            break;
    }
}

