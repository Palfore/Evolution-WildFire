#include "Viewer.h"
#include "Genome.h"
#include "Creature.h"
#include "Logger.h"
#include "Shapes.h"

#include "Factory.h"



CreatureViewer::CreatureViewer(const std::string& g, Creature* b) : CreatureViewer(Genome(g), b) {}
CreatureViewer::CreatureViewer(const Genome& genome, Creature* b) : body(b), phylogeny(genome), fitness(), COMTrail() {
    body->moveTo = Vec(pmRandf(100, 200), 0*pmRandf(70, 100), 0);
}

CreatureViewer::CreatureViewer(const CreatureViewer &other) :
    body(other.body), phylogeny(other.phylogeny), fitness(other.fitness), COMTrail(other.COMTrail) {
}

CreatureViewer::~CreatureViewer() {
    delete body;
}

CreatureViewer& CreatureViewer::operator=(CreatureViewer& other) {
    std::swap(COMTrail, other.COMTrail);
    std::swap(fitness, fitness);
    std::swap(body, other.body);
    return *this;
}

void CreatureViewer::draw() const {
    body->draw();
    DrawRing<Appearance::WHITE>(Vec(body->com.x, body->com.y, 0), 10, 0.5);
}

void CreatureViewer::drawBrain(bool drawLines) const {
    body->drawBrain(drawLines);
}

void CreatureViewer::drawDebug(bool doDraw) const {
    body->drawDebug(doDraw);
}

void CreatureViewer::drawTrails(bool drawCOMTrails) const {
    if (drawCOMTrails) {
        COMTrail.draw();
    }
}

void CreatureViewer::update(int t) { // returns fitness update
    if (t % Trail::SAMPLING_FREQUENCY == 0) {
       COMTrail.addPoint(body->com);
    }
    body->update(t);
    fitness.postUpdate(FitnessCollector::MOVE_TO, *body);
}


Viewer::Viewer(std::vector<Genome*> population, const Factory& factory_t): factory(factory_t), displayingCreature(nullptr), viewingGenomes({}), activeCreatureIndex(0), simStep(0) {
	this->updateViewingGenomes(population);
}

Viewer::~Viewer() {
    delete this->displayingCreature;
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
    	displayingCreature = new CreatureViewer(
            viewingGenomes[activeCreatureIndex],
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

const CreatureViewer& Viewer::getActiveCreature() const {
    return *displayingCreature;
}

int Viewer::getMemberIndex() const {
    return activeCreatureIndex;
}

