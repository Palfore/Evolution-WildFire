#ifndef CREATURE_H
#define CREATURE_H

#include <vector>
#include "MyMath.h"

class Ball;
class Piston;
class Genome;
class Creature {
    public:
        Creature(int n, int m);
        Creature(Genome g);
        Creature(std::string g);
        Creature(const Creature &other);
        Creature& operator=(Creature other);
        virtual ~Creature();

        void initialize(Creature c);

        void draw(double t) const;
        void update(double t);

        void moveCOMTo(Vec to);
        void lowerToGround();
        void centerCOM();
        Vec getCOM() const;

        double getFitness() const;
    private:
        double getLowestBodyHeight() const;

        std::vector<Ball*> nodes;
        std::vector<Piston*> muscles;
};

#endif // CREATURE_H
