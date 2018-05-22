#include "Population.h"
#include <stdio.h>
#include "Creature.h"
#include "Genome.h"
template class Population<Genome>;

template<class T>
Population<T>::Population(int numMembers) : population({}), gen(0), currentMember(0,0), memberIndex(0), simStep(0) {
    for (int i = 0; i < numMembers; i++) {
        population.push_back(new T(5,6));
    }
    currentMember = Creature(*population[memberIndex]);
}

template<class T>
Population<T>::~Population() {
    for (auto * member: population) {
        delete member;
    }
}

template<class T>
void Population<T>::draw() const {
    currentMember.draw(simStep);
}

template<class T>
void Population<T>::nextStep() {
    currentMember.update(simStep++);
}

template<class T>
std::vector<Creature> Population<T>::getCreatures() const {
    std::vector<Creature> creatures;
    creatures.reserve(this->population.size());
    for (const auto& genome: this->population) {
        creatures.push_back(Creature(*genome));
    }
    return creatures;
}

template<class T>
void Population<T>::nextCreature() {
    this->memberIndex = (this->memberIndex + 1) % this->population.size();
    currentMember = Creature(*population[memberIndex]);
}

template <class T>
void Population<T>::showCreature(int index) {
    memberIndex = index;
    currentMember = Creature(*population[memberIndex]);
}

template<class T>
double Population<T>::getAvg() const {
    double sum = 0;
    for (unsigned int i = 0; i < this->population.size(); i++) {
        sum += this->population[i]->fitness;
    }
    return sum / this->population.size();
}

















