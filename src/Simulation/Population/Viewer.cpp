#include "Viewer.h"
#include "Genome.h"
#include "Creature.h"
#include "Logger.h"
#include "Shapes.h"
#include "Factory.h"
#include "ScenarioFactory.h"

Viewer::Viewer(std::vector<Genome*> population, const Factory& factory_t, const ScenarioFactory& senFact):
         phylogeny(*population[0]), fitness(0.0), factory(factory_t), scenarioFactory(senFact),
         scenario(nullptr), viewingGenomes({}), activeCreatureIndex(0), simStep(0), COMTrail() {
    this->updateViewingGenomes(population);
}
Viewer::~Viewer() {
    delete this->scenario;
}

void Viewer::drawBrain(bool drawLines) const {
    this->scenario->creature->drawBrain(drawLines);
}

void Viewer::drawDebug(bool doDraw) const {
    this->scenario->creature->drawDebug(doDraw);
}

void Viewer::drawTrails(bool drawCOMTrails) const {
    if (drawCOMTrails) {
        COMTrail.draw();
    }
}

void Viewer::draw() const {
    this->scenario->draw();
}

void Viewer::moveTo(const Vec& v) {
    this->scenario->creature->moveTo = v;
}

void Viewer::update() {
    if (simStep % COMTrail.samplingFrequency == 0) {
       COMTrail.addPoint(this->scenario->creature->getCOM());
    }

    scenario->update(simStep);
    fitness += scenario->getCurrentFitness();
    simStep++;
    if (simStep > 1'000'000'000) simStep = 0; // Prevent Overflow
}

int Viewer::getSimStep() const {
    return simStep;
}

void Viewer::printCurrentGenome() const {
    std::cout << this->viewingGenomes[activeCreatureIndex].toString() << '\n';
}

void Viewer::printGenome(int index) const {
    std::cout << this->viewingGenomes[index].toString() << '\n';
}

void Viewer::nextCreature() {
    int index = (this->activeCreatureIndex + 1) % this->viewingGenomes.size();
    showCreature(index);
}

void Viewer::prevCreature() {
    int index = (this->activeCreatureIndex - 1) >= 0 ? this->activeCreatureIndex - 1 :  this->viewingGenomes.size() - 1;
    showCreature(index);
}

void Viewer::showCreature(int index) {
    COMTrail.clear();
    activeCreatureIndex = index;
    delete scenario;
    try {
        scenario = this->scenarioFactory.createScenario(
            this->factory.createCreature(viewingGenomes[activeCreatureIndex])
        );
	} catch (const std::exception& e) {
		LOG(e.what(), LogDegree::FATAL, LogType::GENETIC);
	}
    simStep = 0;
}

void Viewer::resetCreature() {
    showCreature(this->activeCreatureIndex);
}

void Viewer::updateViewingGenomes(std::vector<Genome*> population) {
    viewingGenomes.clear();
    for (const auto& genome: population) {
        viewingGenomes.push_back(*genome);
    }
    showCreature(0);
}

int Viewer::getMemberIndex() const {
    return activeCreatureIndex;
}

const Creature& Viewer::getCurrent() const {
    return *(scenario->creature);
}

const Scenario& Viewer::getScenario() const {
    return *scenario;
}
