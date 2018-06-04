#include "Population.h"
#include <stdio.h>
#include "Creature.h"
#include "Genome.h"

Population::Population(int numMembers) : population({}), gen(0), currentMember(0,0,0), memberIndex(0), simStep(0) {
    for (int i = 0; i < numMembers; i++) {
//        population.push_back(new Genome(5,3,3));
//|b,4,1|b,1,3|b,2,4
        population.push_back(new Genome(5,3,3));//"<MetaData>|m,4,3,0.798331|m,0,3,0.114983|m,1,2,0.875029|n,11.0887,7.67372,10.0779,1|n,-22.9639,0.0986828,22.3698,1|n,-6.49215,18.2589,26.7995,1|n,1.32824,-27.3546,25.1702,1|n,21.3509,-26.9144,24.1151,1"));
    }
    currentMember = Creature(*population[memberIndex]);
}

Population::~Population() {
    for (auto * member: population) {
        delete member;
    }
}

void Population::draw() const {
    currentMember.draw(simStep);
}

void Population::nextStep() {
    currentMember.update(simStep++);
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
    std::cout << this->population[memberIndex]->toString() << '\n';
}

void Population::printGenome(int index) const {
    std::cout << this->population[index]->toString() << '\n';
}

void Population::nextCreature() {
    this->memberIndex = (this->memberIndex + 1) % this->population.size();
    currentMember = Creature(*population[memberIndex]);
}

void Population::showCreature(int index) {
    memberIndex = index;
    currentMember = Creature(*population[memberIndex]);
}

double Population::getAvg() const {
    double sum = 0;
    for (unsigned int i = 0; i < this->population.size(); i++) {
        sum += this->population[i]->fitness;
    }
    return sum / this->population.size();
}

void Population::getThreadedFitnesses(const std::vector<MultiThread*>& threads) {
    int c = 0;
    for (unsigned int i = 0; i < threads.size(); i++) {
        for (unsigned int j = 0; j < threads[i]->fitnesses.size(); j++) {
            this->population[c++]->fitness = threads[i]->fitnesses[j];
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
    return memberIndex;
}












