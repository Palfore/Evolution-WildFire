#include "Senario.h"
#include "Creature.h"
#include "Shapes.h"

Senario::Senario(Creature* creature_t, const Terrain& terrain_t, unsigned int maxTime):
	creature(creature_t), food(), terrain(terrain_t), maxEvaluationTime(maxTime) {
        for (int i = 0; i < 800; i++) {
			Vec f = Vec(pmRandf(100, 2000), pmRandf(100, 2000), 0);
			f.z = terrain.getHeight(f);
            food.insertAt(f.x, f.y, f);
		}
}

Senario::~Senario() {
	delete creature;
}

void Senario::draw() const {
	auto foods = food.getFrom(creature->getCOM().x, creature->getCOM().y, 5);
    for (unsigned int i = 0; i < foods.size(); i++) {
        DrawSphere<Appearance::WHITE>(foods[i] + Vec(0, 0, 5), 3);
	}
	this->terrain.draw();
	this->creature->draw(this);
}

void Senario::update(int step) {
    this->creature->update(this, step);
}

double Senario::getCurrentFitness() {
    auto foods = food.getFrom(creature->getCOM().x, creature->getCOM().y);
    for (unsigned int i = 0; i < foods.size(); i++) {
        if ((foods[i] - creature->getCOM()).length() < 20) {
            food.remove(creature->getCOM().x, creature->getCOM().y, foods[i]);
            Vec f = Vec(pmRandf(2000), pmRandf(2000), 0);
            f.z = terrain.getHeight(f);
            food.insertAt(f.x, f.y, f);
            return 1;
        }
    }
	return 0;
}

SenarioB::SenarioB(Creature* creature, const Terrain& terrain, unsigned int maxEvaluationTime):
    Senario(creature, terrain, maxEvaluationTime) {

}

double SenarioB::getCurrentFitness() {
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
