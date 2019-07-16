#ifndef FITNESS_H
#define FITNESS_H

#include <unordered_map>
#include <map>

#include <functional>
#include "Body.h"

typedef std::function<void(double*, Body&)> oneBodyFunc;

class FitnessTop {
    public:
        virtual void call(double*, Body&) {}
        double fitness;
        virtual ~FitnessTop() {}

    protected:
        FitnessTop() : fitness(0) {}
};

class OneBodyFitness : public FitnessTop {
    public:
        oneBodyFunc post;
        void call(double* f, Body& body) override {
            post(f, body);
        }
        OneBodyFitness(oneBodyFunc func) : post(func) {}
};

class FitnessCollector {
public:
    enum FitnessType {
        TOTAL_DISTANCE,
        MOVE_TO,
        STRAIGHT_LINE,
        LAST__
    };

    std::unordered_map<FitnessType, FitnessTop*> fitbit = {
        {TOTAL_DISTANCE      , new OneBodyFitness([](double* f, Body& body) {
                                            body.moveTo = Vec(10000, 10000, 0);
                                            *f = euc2D(body.com, body.initCOM);
                                       })},
        {MOVE_TO              , new OneBodyFitness([](double* f, Body& body) {
                                            Vec p = body.moveTo - body.com;
                                            p.z = 0;
                                            Vec r_hat = p / p.length();
                                            Vec deltaX = body.com - body.prevCOM;
                                            deltaX.z = 0;
                                            double deltaXinRHat = deltaX.dot(r_hat);
                                            *f += deltaXinRHat / p.length();
                                            if (p.length() < 10) {
                                                body.moveTo += Vec(pmRandf(30, 80), pmRandf(30, 80), 0);
                                                // body.moveTo *= -1;//+= Vec(pmRandf(100, 200), 0, 0);
                                            }
                                        })},
        {STRAIGHT_LINE        , new OneBodyFitness([](double*f, Body& body) {
                                            body.moveTo = Vec(10000, 10000, 0);
                                            *f = body.com.x - body.initCOM.x - 0.00001*(body.com.y * body.com.y);
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

    void postUpdate(FitnessType fitnessType, Body& body) {
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
