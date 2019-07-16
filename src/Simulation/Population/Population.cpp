#include "Population.h"
#include <stdio.h>
#include "Creature.h"
#include "Genome.h"
#include "MultiThread.h"

Population::Population(int numMembers, const Factory factory_t) : population({}), history(), gen(0), factory(factory_t) {
    for (int i = 0; i < numMembers; i++) {
        Genome* g = factory.createGenome();
        population.push_back(g);
    }
}

Population::~Population() {
    for (auto * member: population) {
        delete member;
    }
}

std::vector<Body*> Population::getBodies() const {
    std::vector<Body*> bodies;
    bodies.reserve(this->population.size());
    for (const auto& genome: this->population) {
        bodies.push_back(factory.createBody(*genome));
    }
    return bodies;
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

void Population::updateFitnesses(const std::vector<double>& fitnesses) {
    for (unsigned int i = 0; i < fitnesses.size(); i++) {
        this->population[i]->fitness = fitnesses[i];
    }
}

void Population::sortPop() {
    std::sort(this->population.begin(), this->population.end(), [](Genome* a, Genome* b){
        return a->fitness > b->fitness; // '<' ==> worst to best
    });
}

void Population::select() {
    for (unsigned int i = 0; i < this->population.size() / 2; i++) {
        delete this->population[i + this->population.size() / 2];
        this->population[i + this->population.size() / 2] = new Genome(*this->population[i]);
    }
}

void Population::mutate() {
    for (unsigned int i = 0; i < this->population.size(); i++) {
        population[i]->mutate();
    }
}











