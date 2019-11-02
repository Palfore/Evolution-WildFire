#include "ScenarioB.h"
#include "Creature.h"

ScenarioB::ScenarioB(Creature* creature_t, const Terrain& terrain_t, unsigned int maxTime):
    ScenarioA(creature_t, terrain_t, maxTime) {
    //Empty
}

double ScenarioB::getCurrentFitness() {
    return 0;
    Vec p = creature->moveTo - creature->getCOM();
    p.z = 0;
    double dist = p.length();
    if (dist < 5) {
        creature->moveTo += Vec(pmRandf(30, 80), pmRandf(30, 80), 0);
        creature->moveTo.z = terrain.getHeight(creature->moveTo);
        if (p.length() < 1) {
            return 0;
        }
        return 1;
    }

    return 0;
    Vec r_hat = p / dist;
    Vec deltaX = creature->getCOM() - creature->getCOM(-1);
    deltaX.z = 0;
    double deltaXinRHat = deltaX.dot(r_hat);
    return deltaXinRHat / dist;
}
