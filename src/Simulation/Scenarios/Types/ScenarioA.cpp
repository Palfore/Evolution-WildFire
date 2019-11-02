#include "ScenarioA.h"
#include "Creature.h"
#include "Shapes.h"
#include <algorithm>
ScenarioA::ScenarioA(Creature* creature_t, const Terrain& terrain_t, unsigned int maxTime):
	Scenario(creature_t, terrain_t, maxTime) {

}

void ScenarioA::draw() const {
    // Draw a border nxn chunks centered on the creature.
    int n = 2;
	DrawCylinder<Appearance::WHITE>(this->creature->getCOM() + Vec(-n*Chunks::l, n*Chunks::l, 1),
                                    this->creature->getCOM() + Vec(+n*Chunks::l, n*Chunks::l, 1),
                                    1);
    DrawCylinder<Appearance::WHITE>(this->creature->getCOM() + Vec(-n*Chunks::l, -n*Chunks::l, 1),
                                    this->creature->getCOM() + Vec(+n*Chunks::l, -n*Chunks::l, 1),
                                    1);
    DrawCylinder<Appearance::WHITE>(this->creature->getCOM() + Vec(+n*Chunks::l, -n*Chunks::l, 1),
                                    this->creature->getCOM() + Vec(+n*Chunks::l, +n*Chunks::l, 1),
                                    1);
    DrawCylinder<Appearance::WHITE>(this->creature->getCOM() + Vec(-n*Chunks::l, -n*Chunks::l, 1),
                                    this->creature->getCOM() + Vec(-n*Chunks::l, +n*Chunks::l, 1),
                                    1);

    auto foods = food.getFrom(creature->getCOM().x, creature->getCOM().y, 5);
    for (unsigned int i = 0; i < foods.size(); i++) {
        DrawSphere<Appearance::WHITE>(foods[i] + Vec(0, 0, 5), 3);
	}
	this->terrain.draw();
	this->creature->draw(this);
}

void ScenarioA::update(int step) {
    this->creature->update(this, step);
}

double ScenarioA::getCurrentFitness() {
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

double ScenarioA::getFinalFitness() {
    auto foods = food.getFrom(creature->getCOM().x, creature->getCOM().y);
    double closest = -1;
    for (unsigned int i = 0; i < foods.size(); i++) {
        double dr = (foods[i] - creature->getCOM()).length();
        if (closest < 0 || dr < closest) {
            closest = dr;
        }
    }

    if (closest <= 0) {
        return 0;
    }
    return exp(-(closest - 20) / 50);
}

