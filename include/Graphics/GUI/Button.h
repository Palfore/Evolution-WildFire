#ifndef BUTTON_H
#define BUTTON_H

#include "Draw.h"
#include "Shapes.h"
#include "Text.h"

struct by_position{};
struct by_dims{};

class WindowPress : public UIElement {
    public:
        WindowPress(int ID=-1) : UIElement(ID) {}

        void affectState(int, int, CALL_TYPE type) override {
            switch(type) {
                case CALL_TYPE::NONE: break;
                case CALL_TYPE::ACTION: callFunction = type; break;
                case CALL_TYPE::RELEASE: callFunction = type; break;
                default: break;
            }
        }

        void update() override {

        }
};



///< @todo Move Button class into UserInput folder.
template <Appearance A>
class Button : public UIElement { // only handles left clicks
    public:
        bool isClickable() override {
            return true;
        }

        void affectState(int mx, int my, CALL_TYPE type) override {
            hovering = false;
            if (this->isIn(mx, my)) {
                switch(type) {
                    case CALL_TYPE::NONE: hovering = true; break;
                    case CALL_TYPE::ACTION: callFunction = type; highlightTime = std::clock(); break;
                    case CALL_TYPE::RELEASE: callFunction = type; break;
                    default: break;
                }
            }
        }
        void update() {
            if (highlightTime >= 0) {
                if (std::clock() > highlightTime + HIGHLIGHT_DURATION) {
                    highlightTime = -1;
                }
            }
        }

        Button(std::string s, double x_t, double y_t, double X_t, double Y_t, by_position, int ID=-1) : UIElement(ID),
            label(s), x(x_t), y(y_t), X(X_t), Y(Y_t), hovering(false), highlightTime(-1) {}
        Button(std::string s, double x_t, double y_t, double w, double h, by_dims, int ID=-1) : UIElement(ID),
            label(s), x(x_t - w/2.0), y(y_t - h/2.0), X(x_t + w/2.0), Y(y_t + h/2.0), hovering(false), highlightTime(-1) {}
        virtual ~Button(){}

        void draw() const override {
            glDisable(GL_TEXTURE_2D);

            Vec outer(0.2,0.2, 0.2);
            Vec inner(0.9,0.9, 0.9);

            if (highlightTime > 0) { // Only change color on click
                inner = GLOBAL->colorMap[Appearance::SKY_BLUE];
            }

            int t = 3;
            glBegin(GL_QUADS);
                double w = GFramework::get->windowSize.x;
                double h = GFramework::get->windowSize.y;

                Drawing::changeCustomColor(outer);
                glVertex2f(x*w,y*h+t);
                glVertex2f(x*w,Y*h);

                Drawing::changeCustomColor(inner);
                glVertex2f((x+(X-x)/2)*w,Y*h);
                glVertex2f((x+(X-x)/2)*w,y*h+t);

                glVertex2f((x+(X-x)/2)*w,Y*h);
                glVertex2f((x+(X-x)/2)*w,y*h+t);

                Drawing::changeCustomColor(outer);
                glVertex2f(X*w,y*h+t);
                glVertex2f(X*w,Y*h);
            glEnd();

            Drawing::changeColor(hovering ? Appearance::RED : Appearance::WHITE);
            glLineWidth(4);

            glBegin(GL_LINES);
                glVertex2f(x*w, y*h+t);
                glVertex2f(X*w, y*h+t);

                glVertex2f(X*w, Y*h);
                glVertex2f(x*w, Y*h);

                glVertex2f(x*w, y*h+t);
                glVertex2f(x*w, Y*h);

                glVertex2f(X*w, y*h+t);
                glVertex2f(X*w, Y*h);

            glEnd();

            DrawString<Appearance::BLACK>(label, (x+X)/2.0, (y+Y)/2.0, by_percentage(), true, true);
        }
        bool isIn(int x_t, int y_t) {
            double w = GFramework::get->windowSize.x;
            double h = GFramework::get->windowSize.y;
            if (x_t < x*w) return false;
            if (x_t > X*w) return false;
            if (y_t < y*h) return false;
            if (y_t > Y*h) return false;
            return true;
        }
    protected:
        const std::string label;
        double x;
        double y;
        double X;
        double Y;
        bool hovering;
        int highlightTime;
        static constexpr int HIGHLIGHT_DURATION = 200;  // Milliseconds
};

template <Appearance A>
class Checkbox : public Button<A> { /// @todo Maybe this should have a reference to a boolean "state" so it can be affected externally!
    public:
        void affectState(int mx, int my, CALL_TYPE type) override {
            Button<A>::hovering = false;
            if (this->isIn(mx, my)) {
                switch(type) {
                    case CALL_TYPE::NONE: Button<A>::hovering = true; break;
                    case CALL_TYPE::ACTION: Button<A>::callFunction = type; Button<A>::highlightTime = std::clock(); (*this->state) ^= true; break;
                    case CALL_TYPE::RELEASE: Button<A>::callFunction = type; break;
                    default: break;
                }
            }
        }
        void update() {
            if (*state) {
                Button<A>::highlightTime = std::clock() - Button<A>::HIGHLIGHT_DURATION + 1;
            } else {
                Button<A>::highlightTime = -1;
            }
        }

        operator=(const Checkbox&) = delete;
        Checkbox(std::string s, bool *const  active, double x_t, double y_t, double X_t, double Y_t, by_position p, int ID=-1) : Button<A>(s, x_t, y_t, X_t, Y_t, p, ID), state(active) {
            if (*state) { // if checkbox starts off "on", trigger an action on initilization.
                Button<A>::callFunction = CALL_TYPE::ACTION;
            }
        }
        Checkbox(std::string s, bool *const active, double x_t, double y_t, double w, double h, by_dims d, int ID=-1) : Button<A>(s, x_t, y_t, w, h, d, ID), state(active) {
            if (*state) {
                Button<A>::callFunction = CALL_TYPE::ACTION;
            }
        }
        Checkbox(const Checkbox& other) : Button<A>(other), state(other.state) {}
        virtual ~Checkbox(){}

        bool *const state;
    private:

};

#endif // BUTTON_H
