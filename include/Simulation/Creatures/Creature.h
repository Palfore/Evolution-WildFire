#ifndef BODY_H
#define BODY_H

#include "NeuralNetwork.h"
#include "Vec.h"
#include "Terrain.h"
#include "Trail.h"
class Genome;
class Ball;
class Piston;
class Cube;
class Senario;

class Creature {
 public:
    Terrain terrain;

    Vec moveTo;
    Vec initCOM;
    Trail positions;

    Creature();
    Creature(const Genome& genome);
    Creature(const Creature &other);
    virtual ~Creature() {};

    /* Movers */
    Vec getCOM(int offset=0) const;
    virtual void moveCOMTo(Vec to) = 0;
    virtual void lowerToGround() = 0;
    void centerCOM();

    /* Getters */
    virtual Vec calculateCOM() const = 0;
    virtual double getLowestBodyHeight() const = 0;
    virtual Vec getTop(const double offset) const = 0; // Centralized (COM) Top of Creature
    virtual std::string getGenomeString() const;

    /* Drawing */
    virtual void draw(const Senario* senario) const = 0;
    virtual void drawBrain(bool drawAxons) const;
    virtual void drawDebug(bool doDraw) const;

    /* Physics */
    virtual void update(Senario* senario, int t);

 protected:
};

#endif // BODY_H
