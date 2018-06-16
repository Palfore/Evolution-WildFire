#ifndef CREATURE_H
#define CREATURE_H

#include <vector>
#include "MyMath.h"

class Ball;
class Piston;
#include <Genome.h>
//class Genome;
class Creature {
    public:
        Creature(int n, int m, int b);
        Creature(Genome g);
        Creature(std::string g);
        Creature(const Creature &other);
        Creature& operator=(Creature other);
        virtual ~Creature();

        void draw(double t) const;
        void update(double t);

        void moveCOGTo(Vec to);
        void moveCOMTo(Vec to);
        void centerCOG();
        void centerCOM();
        void lowerToGround();
        Vec getCOM() const; // Center of Mass
        Vec getCOG() const; // Center of Geometry
        Vec getTop() const; // Centralized Top of Creature

        double getFitness() const; // This should consider the initial COM.
        std::string getGenomeString() const;
    private:
        double getLowestBodyHeight() const;

        std::vector<Ball*> nodes;
        std::vector<Piston*> muscles;
        std::vector<Piston*> bones;

        std::string genomeString;
};

#endif // CREATURE_H
