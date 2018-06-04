#ifndef USERFUNCTION_H
#define USERFUNCTION_H

#include <functional>

#define ESC       (char) 27
#define TAB       (char) 9
#define BACKSPACE (char) 8
#define ENTER     (char) 13

#include <memory>

class UIElement {
    public:
        UIElement() {}
        virtual ~UIElement() {}

        virtual void affectState(char keyPress) {};
        virtual void affectState(int specialKeyPress) {};
        virtual void affectState(int mx, int my) {};

        virtual void draw() = 0;

};

class UIChar : UIElement {
    char key;
    int specialKey;

    explicit UIChar(char key_t) : key(key_t), specialKey(-1) {}
    explicit UIChar(int key_t) : key('\0'), specialKey(key_t) {}

    void draw() override {};
};


typedef std::function<void()> Action;

struct UserFunction {
    int specialKey;
    char key;

    std::shared_ptr<UIElement> element;

    Action action;
    Action release;

    UserFunction();
    UserFunction(char key_t, Action action_t);

    UserFunction(int specialKey_t, Action action_t);
    UserFunction(int specialKey_t, Action action_t, Action release_t);

    UserFunction(const UIElement &ele, Action action_t);
    UserFunction(const UIElement &ele, Action action_t, Action release_t);
    ~UserFunction();
};

#endif // USERFUNCTION_H
