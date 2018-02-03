#include "UserInput.h"
#include "Graphics.h"
#include "Drawing/text.h"

UserInput::UserInput() : inputString(""), functions({}), keyInputIsHeld(true) {
    //ctor
}

UserInput::~UserInput() {
    //dtor
}

void UserInput::setToDefault() { // Functions that all gamemodes should have
    functions = {};
    functions.push_back(UserFunction(ESC, [](){NORMAL_EXIT();}));
    functions.push_back(UserFunction(TAB, [this](){
        Graphics::get().toggleFullScreen();
        keyStates[TAB] = false; // stop multiple toggle per key press
    }));
    /* Audio Test */
    functions.push_back(UserFunction('.', [this](){
        Graphics::get().audio.playSound("gunShot.wav");
        keyStates['.'] = false; // stop multiple toggle per key press
    }));
    functions.push_back(UserFunction(',', [](){
                                     saveScreenShot();
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
        DrawString<Appearance::BLACK>(inputString.substr(0, inputString.size()-1), x, y, xCenter, yCenter);
    }
}

