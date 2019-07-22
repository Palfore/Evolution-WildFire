#include "Viewer.h"
#include "Genome.h"
#include "Creature.h"
#include "Logger.h"
#include "Shapes.h"

#include "Factory.h"



Viewer::Viewer(std::vector<Genome*> population, const Factory& factory_t, const SenarioFactory& senFact):
         senario(nullptr), phylogeny(*population[0]), fitness(0.0), factory(factory_t), senarioFactory(senFact),
         viewingGenomes({}), activeCreatureIndex(0), simStep(0), COMTrail({}) {
    this->updateViewingGenomes(population);
}
Viewer::~Viewer() {
    delete this->senario;
}

void Viewer::drawBrain(bool drawLines) const {
    this->senario->creature->drawBrain(drawLines);
}

void Viewer::drawDebug(bool doDraw) const {
    this->senario->creature->drawDebug(doDraw);
}

void Viewer::drawTrails(bool drawCOMTrails) const {
    if (drawCOMTrails) {
        COMTrail.draw();
    }
}

void Viewer::draw() const {
    this->senario->draw();
}

void Viewer::moveTo(const Vec& v) {
    this->senario->creature->moveTo = v;
}

void Viewer::update() {
    if (simStep % COMTrail.samplingFrequency == 0) {
       COMTrail.addPoint(this->senario->creature->getCOM());
    }

    senario->update(simStep);
    fitness += senario->getCurrentFitness();
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
    delete senario;
    try {
        senario = this->senarioFactory.createSenario(
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
    return *(senario->creature);
}

const Senario& Viewer::getSenario() const {
    return *senario;
}
