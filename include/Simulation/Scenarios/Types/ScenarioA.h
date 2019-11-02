#ifndef ScenarioA_H
#define ScenarioA_H

#include "Scenario.h"

class Terrain;
class Creature;
class ScenarioA: public Scenario {
 public:
    ScenarioA(Creature* creature, const Terrain& terrain, unsigned int maxEvaluationTime);

    virtual void draw() const override;
    virtual void update(int step) override;
    virtual double getCurrentFitness() override;
    virtual double getFinalFitness() override;
};

#endif // ScenarioA_H
