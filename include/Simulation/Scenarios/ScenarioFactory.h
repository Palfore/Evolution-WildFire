#ifndef ScenarioFactory_H
#define ScenarioFactory_H

#include <string>
#include <unordered_map>
#include <functional>
#include "Scenario.h"

#define CREATE_SCENARIO(type) [](Creature* c, const Terrain& t, unsigned int mT){return new type(c, t, mT);}
#define SCENARIO_PAIR(name) std::pair(#name, CREATE_SCENARIO(name))
using function = std::function<Scenario*(Creature*, const Terrain&, unsigned int)>;

/* Register Scenarios Here */
#include "ScenarioA.h"
#include "ScenarioB.h"
static std::unordered_map<std::string, function> scenario_map = {
    SCENARIO_PAIR(ScenarioA),
    SCENARIO_PAIR(ScenarioB),
};

class ScenarioFactory {
 public:
    ScenarioFactory(const std::string type_t, const Terrain& terrain_t, unsigned int maxTime) : type(type_t), terrain(terrain_t), maxEvaluationTime(maxTime) {

    }

    Scenario* createScenario(Creature* creature) const {
        return scenario_map[type](creature, terrain, maxEvaluationTime);
    }

 private:
    const std::string type;
    const Terrain& terrain;
    unsigned int maxEvaluationTime;
};


#endif // ScenarioFactory_H
