#include "MultiThread.h"

#include "Senario.h"
#include "Creature.h"
#include "Fitness.h"

void MultiThread::spawn(Processor f, const SenarioFactory& factory, const std::vector<Creature*>& creatures) {
    fitnesses.clear();
    t = std::thread(f, creatures, factory, std::ref(this->fitnesses), std::ref(this->finished));
    t.detach();
    finished = false;
}

bool MultiThread::isFinished() const {
    return finished;
}

void MultiThread::processCreatures(const std::vector<Creature*>& creatures, const SenarioFactory& factory, std::vector<double>& fitnesses, bool& done) {
    for (Creature* body: creatures) {
        Senario* senario = factory.createSenario(body);
        double fitness = 0;
        for (unsigned int i = 0; i < senario->maxEvaluationTime; i++) {
            senario->update(i);
            fitness += senario->getCurrentFitness();
        }
        fitnesses.push_back(fitness);
        delete senario;
    }
    done = true;
}

void MultiThread::spawnChildren(std::vector<MultiThread*>& mt, const std::vector<Creature*>& creatures, const SenarioFactory& factory) {
    if (mt.empty()) return;
    auto begining = creatures.cbegin();
    const int blockSize = creatures.size() / mt.size();
    for (unsigned int i = 0; i < mt.size(); i++) {
        auto e1 = begining + i     * blockSize;
        auto e2 = begining + (i+1) * blockSize;
        if (i == mt.size() - 1) e2 = creatures.cend();
        mt[i]->spawn(processCreatures, factory, std::vector<Creature*>(e1, e2));
    }
}
