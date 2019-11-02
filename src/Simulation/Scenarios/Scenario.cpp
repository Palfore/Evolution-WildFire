#include "Scenario.h"
#include "Creature.h"

Scenario::Scenario(Creature* creature_t, const Terrain& terrain_t, unsigned int maxTime) :
    creature(creature_t), terrain(terrain_t), maxEvaluationTime(maxTime), food() {
        int numFood = 400;
        int exclusion = 100;
        int distance = 2000;
        for (int i = 0; i < numFood; i++) {
            Vec f = Vec(pmRandf(exclusion, distance), pmRandf(exclusion, distance), 0);
            f.z = terrain.getHeight(f);
            food.insertAt(f.x, f.y, f);
        }
}

Scenario::~Scenario() {
    delete creature;
}
