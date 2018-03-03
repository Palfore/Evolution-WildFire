#include "UserInput.h"
#include "GFramework.h"
#include "Drawing/text.h"
#include "Audio.h"


UserInput::UserInput() : inputString(""), functions({}), keyInputIsHeld(true) {
    //ctor
}

UserInput::~UserInput() {
    //dtor
}
#include <iostream>
void UserInput::setToDefault() { // Functions that all gamemodes should have
    functions = {};
    functions.push_back(UserFunction(ESC, [](){NORMAL_EXIT();}));
    functions.push_back(UserFunction(TAB, [this](){
        glutFullScreenToggle();
        keyStates[TAB] = false; // stop multiple toggle per key press
    }));
    /* Audio Test */
    functions.push_back(UserFunction('.', [this](){
        GFramework::get->audio->playSound("gunShot.wav");
        keyStates['.'] = false; // stop multiple toggle per key press
    }));
    functions.push_back(UserFunction(',', [](){
        saveScreenShot();
    }));
    functions.push_back(UserFunction('0', [](){
        LOG("Test debug message.", LogDegree::DEBUG, LogType::GENERAL);
    }));
    functions.push_back(UserFunction('9', [](){
        LOG("Test fatal message.", LogDegree::FATAL, LogType::GENERAL);
    }));
    functions.push_back(UserFunction('8', [](){
        LOG("Test warning message.", LogDegree::WARNING, LogType::GENERAL);
    }));
    functions.push_back(UserFunction('+', [this](){
        GFramework::get->audio->volumeUp(5);
//        std::cout << "Up\n";
        keyStates['+'] = false; // stop multiple toggle per key press
    }));
    functions.push_back(UserFunction('-', [this](){
        GFramework::get->audio->volumeDown(5);
//        std::cout << "Down\n";
        keyStates['-'] = false; // stop multiple toggle per key press
    }));
    functions.push_back(UserFunction('*', [this](){
        GFramework::get->audio->toggleMute();
//        std::cout << "muted\n";
        keyStates['*'] = false; // stop multiple toggle per key press
    }));
}

bool UserInput::isInputStringSubmitted() {
    return inputString.back() == TERMINATING_CHAR;
}
void UserInput::submitInputString() {
    inputString += TERMINATING_CHAR;
}

void UserInput::drawUserString(double x, double y, bool xCenter, bool yCenter) {
    if (isInputStringSubmitted() && ((inputString.size() - 1) > 0)) {
        DrawString<Appearance::GRASS>(inputString.substr(0, inputString.size()-1), x, y, xCenter, yCenter);
    }
}

