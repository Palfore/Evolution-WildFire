#include "Population.h"
#include <stdio.h>
#include "Creature.h"
#include "Genome.h"

Population::Population(int numMembers) : population({}), history(), gen(0), viewingGenomes({}), displayingCreature(0,0,0), activeCreatureIndex(0), simStep(0) {
    for (int i = 0; i < numMembers; i++) {
        Genome* g = new Genome(5,3,3);
        population.push_back(g);
        viewingGenomes.push_back(*g);
    }
    showCreature(0);
}

Population::~Population() {
    for (auto * member: population) {
        delete member;
    }
}

void Population::draw() const {
    displayingCreature.draw(simStep);
}

void Population::nextStep() {
    displayingCreature.update(simStep++);
}

int Population::getSimStep() const {
    return simStep;
}

std::vector<Creature> Population::getCreatures() const {
    std::vector<Creature> creatures;
    creatures.reserve(this->population.size());
    for (const auto& genome: this->population) {
        creatures.push_back(Creature(*genome));
    }
    return creatures;
}

void Population::printCurrentGenome() const {
    std::cout << this->population[activeCreatureIndex]->toString() << '\n';
}

void Population::printGenome(int index) const {
    std::cout << this->population[index]->toString() << '\n';
}

void Population::nextCreature() {
    int index = (this->activeCreatureIndex + 1) % this->population.size();
    showCreature(index);
}

void Population::prevCreature() {
    int index = (this->activeCreatureIndex - 1) >= 0 ? this->activeCreatureIndex - 1 :  this->population.size() - 1;
    showCreature(index);
}

void Population::showCreature(int index) {
    activeCreatureIndex = index;
    displayingCreature = Creature(viewingGenomes[activeCreatureIndex]);
    simStep = 0;
}

void Population::resetCreature() {
    showCreature(this->activeCreatureIndex);
}

void Population::updateViewingGenomes() {
    viewingGenomes.clear();
    for (const auto& genome: this->population) {
        viewingGenomes.push_back(*genome);
    }
    showCreature(0);
}

Creature Population::getActiveCreature() const {
    return displayingCreature;
}

double Population::getAvg() const {
    double sum = 0;
    for (unsigned int i = 0; i < this->population.size(); i++) {
        sum += this->population[i]->fitness;
    }
    return sum / this->population.size();
}


void Population::recordHistory() {
    history.addPoint(population, gen);
}


void Population::printHistory() {
    history.writeToConsole();
}

void Population::getThreadedFitnesses(const std::vector<MultiThread*>& threads) {
    int c = 0;
    for (unsigned int i = 0; i < threads.size(); i++) {
        const std::vector<double> f = threads[i]->fitnesses;
        for (unsigned int j = 0; j < f.size(); j++) {
            this->population[c++]->fitness = f[j];
        }
    }
}

void Population::sortPop() {
    std::sort(this->population.begin(), this->population.end(), [](Genome* a, Genome* b){
        return a->fitness > b->fitness; // '<' ==> worst to best
    });
}

void Population::select() {
    for (unsigned int i = 0; i < this->population.size() / 2; i++) {
        delete this->population[i + this->population.size() / 2]; // Use smart pointers for population manager [future]
        this->population[i + this->population.size() / 2] = new Genome(*this->population[i]);
    }
}

void Population::mutate() {
    for (unsigned int i = 0; i < this->population.size(); i++) {
        population[i]->mutate();
    }
}


int Population::getMemberIndex() const {
    return activeCreatureIndex;
}














