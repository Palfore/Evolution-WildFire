#ifndef ScenarioB_H
#define ScenarioB_H

#include "ScenarioA.h"

class ScenarioB: public ScenarioA {
 public:
    ScenarioB(Creature* creature, const Terrain& terrain, unsigned int maxEvaluationTime);
    virtual double getCurrentFitness() override;
};

#endif // ScenarioB_H
