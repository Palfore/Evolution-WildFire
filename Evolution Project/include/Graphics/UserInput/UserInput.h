#ifndef USERINPUT_H
#define USERINPUT_H

#include "UserFunction.h"
#include "Draw.h"
#include <vector>

class UserInput {
    private:
        static int constexpr NUM_KEYS = 256;
        static char constexpr TERMINATING_CHAR = '#';
    public:
        std::string inputString;
        std::vector<UserFunction> functions;
        bool keyStates[NUM_KEYS] = {false}; // keypresses
        bool keyInputIsHeld;

        UserInput();
        ~UserInput();

        void setToDefault();
        bool isInputStringSubmitted();
        void submitInputString();
        void drawUserString(double x, double y, bool xCenter=true, bool yCenter=true);
};

#endif // USERINPUT_H