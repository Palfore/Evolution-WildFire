#ifndef BUTTON_H
#define BUTTON_H

#include "Draw.h"
#include "Shapes.h"
#include "Text.h"

struct by_position{};
struct by_dims{};

template <Appearance A>
class Button : UIElement {
    public:
        Button(std::string s, double x_t, double y_t, double X_t, double Y_t, by_position) :
            label(s), x(x_t), y(y_t), X(X_t), Y(Y_t) {}

        Button(std::string s, double x_t, double y_t, double w, double h, by_dims) :
            label(s), x(x_t - w/2.0), y(y_t - h/2.0), X(x_t + w/2.0), Y(y_t + h/2.0) {}

        void draw() override {
            DrawRectangle<A>(x, y, X, Y);
            DrawString<Appearance::BLACK>(label, (x+X)/2.0, (y+Y)/2.0, true, true);
        }
        bool isIn(double x_t, double y_t) {
            if (x_t < x) return false;
            if (x_t > X) return false;
            if (y_t < y) return false;
            if (y_t > Y) return false;
            return true;
        }
    private:
        const std::string label;
        double x;
        double y;
        double X;
        double Y;
};

#endif // BUTTON_H
