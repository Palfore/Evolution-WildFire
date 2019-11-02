#ifndef Scenario_H
#define Scenario_H

#include "Chunks.h"

class Creature;
class Terrain;
class Scenario {
 public:
 	Creature* creature;
 	const Terrain& terrain;
 	unsigned int maxEvaluationTime;
 	Chunks food;

    Scenario(Creature* creature, const Terrain& terrain, unsigned int maxEvaluationTime);
    Scenario(const Scenario&) = delete;
    Scenario& operator=(const Scenario&) = delete;
    virtual ~Scenario() = 0;

    virtual void draw() const = 0;
    virtual void update(int step) = 0;
    virtual double getCurrentFitness() = 0;
    virtual double getFinalFitness() = 0;
};

#endif // Scenario_H