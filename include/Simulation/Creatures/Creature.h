#ifndef CREATURE_H
#define CREATURE_H

#include "Fitness.h"
#include "NeuralNetwork.h"
#include "Phylogeny.h"
#include "Body.h"
#include "Trail.h"
#include "Vec.h"

#include <vector>
#include <functional>
#include <iostream>

class Ball;
class Piston;
class Genome;

class Creature {
    public:
        //std::string name;
        //Metrics metrics;
        Body* body;
        Phylogeny phylogeny;
        FitnessCollector fitness;

        Creature(const Genome& g, Body* b);
        Creature(const std::string& g, Body* b);
        Creature(const Creature &other);
        ~Creature();
        Creature& operator=(Creature& other);

        void draw() const;
        void drawBrain(bool drawAxons) const;
        void drawTrails(bool drawNodeTrails, bool drawCOMTrails) const;
        void drawNodeData() const;

        void update(int t);

    private:
        Trail COMTrail;
        std::vector<Trail> nodeTrails;
};

#endif // CREATURE_H
