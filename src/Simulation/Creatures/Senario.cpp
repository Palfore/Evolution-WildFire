#include "Senario.h"
#include "Creature.h"
#include "Shapes.h"

Senario::Senario(Creature* creature, const Terrain& terrain, unsigned int maxTime):
	creature(creature), food(75, 1), terrain(terrain), evaluationTime(maxTime) {
		while (!food.isFull()) {
			Vec f = Vec(pmRandf(500), pmRandf(500), 0);
			f.z = terrain.getHeight(f);
			food.addPoint(f);
		}
}

Senario::~Senario() {
	delete creature;
}

void Senario::draw() const {
	for (int i = 0; i < food.size(); i++) {
		DrawSphere<Appearance::WHITE>(food[i], 3);
	}
	this->terrain.draw();
	this->creature->draw(this);
}

void Senario::update(int step) {
    this->creature->update(this, step);
}

double Senario::getCurrentFitness() const {
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
