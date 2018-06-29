#ifndef BODY_H
#define BODY_H

#include "NeuralNetwork.h"
#include "Vec.h"
class Genome;
class Ball;
class Piston;
class Body {
    public:
        Vec moveTo;
        Vec com;
        double radius;
        Vec initCOM;

        Body(const Genome& g);
        virtual ~Body();

        Body(const Body &other);
        Body& operator=(Body other);

        void moveCOGTo(Vec to);
        void moveCOMTo(Vec to);
        void centerCOG();
        void centerCOM();
        void lowerToGround();

        void draw() const;
        void drawBrain(bool drawAxons) const;

        void update(int t);

        Vec getCOG() const; // Center of Geometry
        Vec getTop() const; // Centralized (COM) Top of Creature
        Vec getNodePosition(int index) const;
        int getTopNodeIndex() const; // Highest Node of Creature
        std::vector<Vec> getNodePositions() const;

        std::string getGenomeString() const;
    private:
        double getLowestBodyHeight() const;
        Vec getCOM() const; // Center of Mass

        std::vector<Ball*> nodes;
        std::vector<Piston*> muscles;
        std::vector<Piston*> bones;
        NeuralNetwork NN;
};

#endif // BODY_H
