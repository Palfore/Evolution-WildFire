#include "MultiThread.h"

#include "Creature.h"

void MultiThread::spawn(std::function<void(std::vector<Creature>, std::vector<double> &, bool &)> f, std::vector<Creature> creatures) {
    finished = false;
    fitnesses.clear();
    t = std::thread(f, creatures, std::ref(this->fitnesses), std::ref(this->finished));
    t.detach();
}

bool MultiThread::isFinished() const {
    return finished;
}

#include "Vec.h"
void MultiThread::processCreatures(std::vector<Creature> creatures, std::vector<double> & fitnesses, bool & done) {
    for (auto& creature: creatures) {
        double fitness = 0;
        for (int i = 0; i < SIMULATION_TIME; i++) {
            fitness += creature.update(i);
        }
        fitnesses.push_back(fitness);
    }
    done = true;
}

void MultiThread::spawnChildren(std::vector<MultiThread*> mt, std::vector<Creature> creatures) {
    auto begining = creatures.cbegin();
    const int blockSize = creatures.size() / mt.size();
    for (unsigned int i = 0; i < mt.size(); i++) {
        auto e1 = begining + i     * blockSize;
        auto e2 = begining + (i+1) * blockSize;
        if (i == mt.size() - 1) e2 = creatures.cend();
        mt[i]->spawn(processCreatures, std::vector<Creature>(e1, e2));
    }
}
