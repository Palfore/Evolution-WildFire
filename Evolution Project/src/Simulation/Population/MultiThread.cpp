#include "MultiThread.h"

#include "Creature.h"

void MultiThread::spawn(std::function<void(const std::vector<Body*>&, std::vector<double> &, bool &)> f, const std::vector<Body*>& creatures) {
    fitnesses.clear();
    t = std::thread(f, creatures, std::ref(this->fitnesses), std::ref(this->finished));
    t.detach();
    finished = false;
}

bool MultiThread::isFinished() const {
    return finished;
}

void MultiThread::processCreatures(const std::vector<Body*>& creatures, std::vector<double> & fitnesses, bool & done) {
    for (Body* body: creatures) {
        FitnessCollector f;
        body->moveTo = Vec(pmRandf(100, 101) + 0*pmRandf(100, 200), 0*pmRandf(70, 100), 0);
        for (int i = 0; i < SIMULATION_TIME; i++) {
            body->update(i);
            f.postUpdate(FitnessCollector::MOVE_TO, *body);
        }
        delete body;
        fitnesses.push_back(f.getFitness(FitnessCollector::MOVE_TO));
    }
    done = true;
}

void MultiThread::spawnChildren(std::vector<MultiThread*>& mt, const std::vector<Body*>& creatures) {
    if (mt.empty()) return;
    auto begining = creatures.cbegin();
    const int blockSize = creatures.size() / mt.size();
    for (unsigned int i = 0; i < mt.size(); i++) {
        auto e1 = begining + i     * blockSize;
        auto e2 = begining + (i+1) * blockSize;
        if (i == mt.size() - 1) e2 = creatures.cend();
        mt[i]->spawn(processCreatures, std::vector<Body*>(e1, e2));
    }
}
