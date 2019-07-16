#include "Viewer.h"
#include "Genome.h"
#include "Creature.h"
#include "Logger.h"

#include "Factory.h"

Viewer::Viewer(std::vector<Genome*> population, const Factory factory): displayingCreature(nullptr), viewingGenomes({}), activeCreatureIndex(0), simStep(0), factory(factory) {
	this->updateViewingGenomes(population);
}

void Viewer::draw() const {
    displayingCreature->draw();
}

void Viewer::nextStep() {
    displayingCreature->update(simStep++);
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
    activeCreatureIndex = index;
    delete displayingCreature;
    try {
    	displayingCreature = new Creature(
            viewingGenomes[activeCreatureIndex],
            this->factory.createBody(viewingGenomes[activeCreatureIndex])
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

const Creature& Viewer::getActiveCreature() const {
    return *displayingCreature;
}

int Viewer::getMemberIndex() const {
    return activeCreatureIndex;
}


