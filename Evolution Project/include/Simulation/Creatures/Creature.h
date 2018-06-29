#ifndef CREATURE_H
#define CREATURE_H

#include <vector>

#include "NeuralNetwork.h"
#include "Phylogeny.h"
#include "Body.h"
#include "Trail.h"

#include <unordered_map>
#include <functional>

#include "Vec.h"

#include <iostream>

typedef std::function<void(double*, Body&, const Body&)> twoBodyFunc;
typedef std::function<void(double*, Body&)> oneBodyFunc;

class FitnessTop {
public:
    virtual void call(double*, Body&, const Body&) {}
    double fitness;
    virtual ~FitnessTop() {}
protected:
    FitnessTop() : fitness(0) {}
};

class OneBodyFitness : public FitnessTop {
public:
    oneBodyFunc post;
    void call(double* f, Body& body, const Body&) override {
        post(f, body);
    }
    OneBodyFitness(oneBodyFunc func) : post(func) {}
};
class TwoBodyFitness : public FitnessTop {
public:
    twoBodyFunc post;
    void call(double* f, Body& body2, const Body& body1) override {
        post(f, body2, body1);
    }
    TwoBodyFitness(twoBodyFunc func) : post(func) {};
};

class FitnessCollector {
public:
    enum FitnessType {TOTAL_DISTANCE, MOVE_TO, STRAIGHT_LINE, LAST__};

    Body* prevBody;
    std::unordered_map<FitnessType, FitnessTop*> fitbit = { /* Body 2 is current */
        {TOTAL_DISTANCE      , new OneBodyFitness([](double* f, Body& body) {
                                            *f = euc2D(body.com, body.initCOM);
                                       })},
        {MOVE_TO              , new OneBodyFitness([](double*f, Body& body) {
                                            double d = euc2D(body.com, body.moveTo);
                                            *f -= 0.0000005*d;//1.0/cosh(d);
                                            if (d < 10) {
                                                body.moveTo += Vec(pmRandf(50, 100), pmRandf(50, 100), 0);
                                                *f += 20;
                                            }
                                        })},
        {STRAIGHT_LINE        , new OneBodyFitness([](double*f, Body& body) {
                                            *f = body.com.x - body.initCOM.x - 0.00001*(body.com.y * body.com.y);
                                        })},
    };

    FitnessCollector(const FitnessCollector&) : FitnessCollector() {};
    FitnessCollector& operator=(const FitnessCollector& other) {
        if (&other != this) {
            fitbit = other.fitbit;
//            prevBody = nullptr;
        }
        return *this;
    }

    FitnessCollector() : prevBody(nullptr) {};
    ~FitnessCollector() {};

    void preUpdate(Body* body) {
        prevBody = body;
    }

    void postUpdate(FitnessType fitnessType, const Body& body) {
        fitbit.at(fitnessType)->call(&fitbit.at(fitnessType)->fitness, *prevBody, body);
    }

    double getFitness(FitnessType fitnessType) const {
        return fitbit.at(fitnessType)->fitness;
    }

    std::map<FitnessType, double> getFitness(const std::vector<FitnessType>& fitnessTypes) const {
        std::map<FitnessType, double> fitnesses = {};
        for (auto const& fitnessType: fitnessTypes) {
            fitnesses.insert({fitnessType, fitbit.at(fitnessType)->fitness});
        }
        return fitnesses;
    }

    std::map<FitnessType, double> getFitness() const {
        std::map<FitnessType, double> fitnesses = {};
        for (int i = 0; i < LAST__; i++) {
            fitnesses.insert({static_cast<FitnessType>(i), fitbit.at(static_cast<FitnessType>(i))->fitness});
        }
        return fitnesses;
    }
};

class Ball;
class Piston;
class Genome;
class Creature {
    public:
        //        std::string name;
        Body body;
        Phylogeny phylogeny;
        //Metrics metrics;
        FitnessCollector fitness; // collect various fitnesses measures

        Creature(int n, int m, int b, const std::vector<unsigned int>& N);
        Creature(const Genome& g);
        Creature(const std::string& g);
        Creature(const Creature &other);
        Creature& operator=(Creature& other);
        virtual ~Creature();

        void draw() const;
        void drawBrain(bool drawAxons) const;
        void drawTrails(bool drawNodeTrails, bool drawCOMTrails) const;
        void drawNodeData() const;

        Vec getHead() const;
        void update(int t);
    private:
        int headIndex;
        Trail COMTrail;
        std::vector<Trail> nodeTrails;
};

#endif // CREATURE_H
