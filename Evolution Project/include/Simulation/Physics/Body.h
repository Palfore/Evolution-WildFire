#ifndef BODY_H
#define BODY_H

#include "NeuralNetwork.h"
#include "Vec.h"

class Genome;
class Ball;
class Piston;
class Cube;

class Body {
    public:
        Vec moveTo;
        Vec com;
        Vec initCOM;
        Vec prevCOM;

        Body(const Genome& g);
        Body(const Body &other);
        virtual ~Body() {};

        /* Movers */
        virtual Vec getCOM() const = 0;
        virtual void moveCOMTo(Vec to) = 0;
        virtual void lowerToGround() = 0;
        void centerCOM();

        /* Getters */
        virtual double getLowestBodyHeight() const = 0;
        virtual Vec getTop(const double offset) const = 0; // Centralized (COM) Top of Creature
        virtual std::string getGenomeString() const;

        /* Drawing */
        virtual void draw() const = 0;
        virtual void drawBrain(bool drawAxons) const;

        /* Physics */
        virtual void update(int t) = 0;
};

#endif // BODY_H
