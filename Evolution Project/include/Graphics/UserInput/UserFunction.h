#ifndef USERFUNCTION_H
#define USERFUNCTION_H

#include <functional>

#define ESC       (char) 27
#define TAB       (char) 9
#define BACKSPACE (char) 8
#define ENTER     (char) 13

typedef std::function<void()> Action;
struct UserFunction {
    int specialKey;
    char key;
    Action action;
    Action release;

    UserFunction();
    UserFunction(char key_t, Action action_t);
    UserFunction(int specialKey_t, Action action_t);
    UserFunction(int specialKey_t, Action action_t, Action release_t);
    ~UserFunction();
};

#endif // USERFUNCTION_H
