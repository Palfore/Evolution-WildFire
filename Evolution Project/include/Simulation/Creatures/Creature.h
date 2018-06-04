#ifndef CREATURE_H
#define CREATURE_H

#include <vector>
#include "MyMath.h"

class Ball;
class Piston;
class Genome;
class Creature {
    public:
        Creature(int n, int m, int b);
        Creature(Genome g);
        Creature(std::string g);
        Creature(const Creature &other);
        Creature& operator=(Creature other);
        virtual ~Creature();

        void initialize(Creature c);

        void draw(double t) const;
        void update(double t);

        void moveCOGTo(Vec to);
        void lowerToGround();
        void centerCOG();
        Vec getCOM() const; // Center of Mass
        Vec getCOG() const; // Center of Geometry

        double getFitness() const;
    private:
        double getLowestBodyHeight() const;

        std::vector<Ball*> nodes;
        std::vector<Piston*> muscles;
        std::vector<Piston*> bones;
};

#endif // CREATURE_H
