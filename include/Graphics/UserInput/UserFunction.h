#ifndef USERFUNCTION_H
#define USERFUNCTION_H

#include <functional>

#define ESC       (char) 27
#define TAB       (char) 9
#define BACKSPACE (char) 8
#define ENTER     (char) 13

#include <memory>
enum class CALL_TYPE: unsigned int {ACTION, RELEASE, NONE};
enum class REPEATABLE: bool {YES, NO};

class UIElement {
    public:
        int id;

        UIElement(int ID=-1) : id(ID), callFunction(CALL_TYPE::NONE) {}
        virtual ~UIElement() {}
        CALL_TYPE shouldCallFunction() {return callFunction;}
        void resetCallFunction() {callFunction = CALL_TYPE::NONE;}

        virtual void affectState(unsigned char keyPress, CALL_TYPE type);
        virtual void affectState(int specialKeyPress, CALL_TYPE type);
        virtual void affectState(int mx, int my, CALL_TYPE type);
        virtual void update() = 0;
        virtual void draw() const {}

        virtual bool isClickable() {
            return false;
        }

        CALL_TYPE callFunction;
    protected:

};

#include "stdio.h"
#include <ctime>
#include <iostream>
struct UIchar : public UIElement {
    void affectState(unsigned char keyPress, CALL_TYPE type) override {
        if (keyPress == key) {
            callFunction = type;
            if (type == CALL_TYPE::ACTION) heldDown = true;
            else heldDown = false;
        }
    }
    void affectState(int specialKeyPress, CALL_TYPE type) override {
        if (specialKeyPress == specialKey) {
            callFunction = type;
            if (type == CALL_TYPE::ACTION) heldDown = true;
            else heldDown = false;
        }
    }

    void update() {
        if (heldDown) {
            if (repeatMS == 0) {
                heldDown = false;
            } else if (repeatMS > 0) {
                bool notYetCalled = lastCalledTime < 0;
                bool pastRepeatTime = lastCalledTime + repeatMS < std::clock();

                if (notYetCalled || pastRepeatTime) {
                    callFunction = CALL_TYPE::ACTION;
                    lastCalledTime = std::clock();
                } else {
                    callFunction = CALL_TYPE::NONE;
                }
            }
        }
        if (!heldDown) {
            lastCalledTime = -1;
        }
    }
    explicit UIchar(char key_t, int repeatability) : key(key_t), specialKey(-1), heldDown(false), repeatMS(repeatability), lastCalledTime(-1) {}
    explicit UIchar(char key_t) : UIchar(key_t, 0) {}
    explicit UIchar(int key_t, int repeatability) : key('\0'), specialKey(key_t), heldDown(false), repeatMS(repeatability), lastCalledTime(-1) {}
    explicit UIchar(int key_t) : UIchar(key_t, 0) {}
    virtual ~UIchar(){}

    private:
        char key;
        int specialKey;
        bool heldDown;
        const int repeatMS;
        double lastCalledTime;
};


typedef std::function<void()> Action;

struct UserFunction {
    UIElement *element;

    Action action;
    Action release;

    void update() {
        this->element->update();
        switch(this->element->shouldCallFunction()) {
            case CALL_TYPE::ACTION: action(); element->resetCallFunction(); break;
            case CALL_TYPE::RELEASE: release(); element->resetCallFunction(); break;
            case CALL_TYPE::NONE: break;
            default: break;
        }

    }

    void draw() const {
        element->draw();
    }

    UserFunction& operator=(const UserFunction& other);
    UserFunction(const UserFunction& other) : element(other.element), action(other.action), release(other.release) {};

    UserFunction(UIElement* ele, Action action_t);
    UserFunction(UIElement* ele, Action action_t, Action release_t);
    ~UserFunction();
};

#endif // USERFUNCTION_H
