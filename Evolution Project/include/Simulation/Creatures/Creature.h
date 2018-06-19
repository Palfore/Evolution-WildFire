#ifndef CREATURE_H
#define CREATURE_H

#include <vector>
#include "NeuralNetwork.h"
#include "Vec.h"

class Ball;
class Piston;
class Genome;
class Creature {
    public:
        Vec moveTo;

        Creature(int n, int m, int b, std::vector<unsigned int> N);
        Creature(Genome g);
        Creature(std::string g);
        Creature(const Creature &other);
        Creature& operator=(Creature other);
        virtual ~Creature();

        void draw() const;
        void drawBrain(bool drawAxons) const;
        double update(int t);

        void moveCOGTo(Vec to);
        void moveCOMTo(Vec to);
        void centerCOG();
        void centerCOM();
        void lowerToGround();
        Vec getCOM() const; // Center of Mass
        Vec getCOG() const; // Center of Geometry
        Vec getTop() const; // Centralized (COM) Top of Creature
        Vec getTopNode() const; // Highest Node of Creature

        double getFitness() const; // Somehow different measures of fitness should be possible or required
        double getFitness(Vec com) const;
        std::string getGenomeString() const;

    private:
        double getLowestBodyHeight() const;
        Vec initCOM;

        std::vector<Ball*> nodes;
        std::vector<Piston*> muscles;
        std::vector<Piston*> bones;
        NeuralNetwork NN;

        std::string genomeString;
};

#endif // CREATURE_H
