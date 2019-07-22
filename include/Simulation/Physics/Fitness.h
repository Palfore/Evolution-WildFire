#ifndef FITNESS_H
#define FITNESS_H

#include <unordered_map>
#include <map>

#include <functional>
#include "Creature.h"

typedef std::function<void(double*, Creature&)> oneCreatureFunc;

class FitnessTop {
    public:
        virtual void call(double*, Creature&) {}
        double fitness;
        virtual ~FitnessTop() {}

    protected:
        FitnessTop() : fitness(0) {}
};

class OneCreatureFitness : public FitnessTop {
    public:
        oneCreatureFunc post;
        void call(double* f, Creature& body) override {
            post(f, body);
        }
        OneCreatureFitness(oneCreatureFunc func) : post(func) {}
};
#include <iostream>

class FitnessCollector {
public:
    enum FitnessType {
        TOTAL_DISTANCE,
        MOVE_TO,
        STRAIGHT_LINE,
        LAST__
    };

    std::unordered_map<FitnessType, FitnessTop*> fitbit = {
        {TOTAL_DISTANCE      , new OneCreatureFitness([](double* f, Creature& body) {
                                            body.moveTo = Vec(10000, 10000, 0);
                                            *f = euc2D(body.getCOM(), body.initCOM);
                                       })},
        {MOVE_TO              , new OneCreatureFitness([](double* f, Creature& body) {
            Vec p = body.moveTo - body.getCOM();
            p.z = 0;
            if (p.length() < 5) {
                body.moveTo += Vec(pmRandf(30, 80), pmRandf(30, 80), 0);
                // body.moveTo.z = terrain.getHeight(body.moveTo);
                // body.moveTo *= -1;//+= Vec(pmRandf(100, 200), 0, 0);
                if (p.length() < 1) {
                    return;
                }
            }


            double dist = p.length();
            Vec r_hat = p / p.length();
            Vec deltaX = body.getCOM() - body.getCOM(-1);
            deltaX.z = 0;
            double deltaXinRHat = deltaX.dot(r_hat);
            *f += deltaXinRHat / p.length();
        })},
        {STRAIGHT_LINE        , new OneCreatureFitness([](double*f, Creature& body) {
                                            body.moveTo = Vec(10000, 10000, 0);
                                            *f = body.getCOM().x - body.initCOM.x - 0.00001*(body.getCOM().y * body.getCOM().y);
                                        })},
    };

    FitnessCollector(const FitnessCollector&) : FitnessCollector() {};
    FitnessCollector& operator=(const FitnessCollector& other) {
        if (&other != this) {
            fitbit = other.fitbit;
        }
        return *this;
    }

    FitnessCollector() {};
    ~FitnessCollector() {};

    void postUpdate(FitnessType fitnessType, Creature& body) {
        fitbit.at(fitnessType)->call(&fitbit.at(fitnessType)->fitness, body);
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
#endif // FITNESS_H
