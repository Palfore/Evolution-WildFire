#ifndef USERINPUT_H
#define USERINPUT_H

#include "UserFunction.h"
#include "Draw.h"
#include <vector>

class UserInput {
    private:
        static char constexpr TERMINATING_CHAR = '#';
    public:
        std::string inputString;
        std::vector<UserFunction> functions;

        UserInput();
        ~UserInput();

        void draw() const;
        void setToDefault();
        bool isInputStringSubmitted();
        void submitInputString();
        void drawUserString(double x, double y, bool xCenter=true, bool yCenter=true);
};

#endif // USERINPUT_H
