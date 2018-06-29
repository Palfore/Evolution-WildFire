#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include <functional>
#include <thread>
#include <vector>


class Body;
class MultiThread {
    public:
        static constexpr int SIMULATION_TIME = 6000; ///< @todo Move this a better place.

        std::vector<double> fitnesses;
        MultiThread() : fitnesses({}), t(), finished(false) {}

        bool isFinished() const;
        static void spawnChildren(std::vector<MultiThread*>& mt, const std::vector<Body>& creatures);

    private:
        std::thread t;
        bool finished;
        static void processCreatures(const std::vector<Body>& bodies, std::vector<double> & fitnesses, bool & done);
        void spawn(std::function<void(const std::vector<Body>&, std::vector<double> &, bool &)> f, const std::vector<Body>& creatures);

};

#include "Logger.h"
#include <iostream>
class Threader {
public:
    std::vector<MultiThread*> mt; // Dynamic since on exit, statically allocated references get deleted.

    Threader(int num, const std::vector<Body>& bodies) : mt({}), isDone(true) {
        updateThreadCount(num);
        for (int i = 0; i < num; i++) {
            try {
                this->spawnThreads(bodies);
            } catch (std::bad_alloc const& ext) {
                // With ~10000 complex creatures, memory usage spikes to over 2GB, causing a crash.
                LOG("There is not enough memory to spawn new threads.", LogDegree::FATAL);
            }
        }
    }
    ~Threader() {
        for (auto &p: mt) {
            delete p;
        }
    }

    void update(int num) {
        checkIsFinished();
        updateThreadCount(num);
    }

    std::vector<double> getFitnesses(int popSize=0) {
        std::vector<double> fitnesses;
        fitnesses.reserve(popSize);
        for (unsigned int i = 0; i < mt.size(); i++) {
            const std::vector<double>& f = mt[i]->fitnesses;
            for (unsigned int j = 0; j < f.size(); j++) {
                fitnesses.push_back(f[j]);
            }
        }
        return fitnesses;
    }

    long long unsigned int size() {
        return mt.size();
    }

    bool canProcess() {
        if (isDone) {
            if (!mt.empty()) {
                return true;
            }
        }
        return false;
    }

    void spawnThreads(const std::vector<Body>& bodies) {
        MultiThread::spawnChildren(mt, bodies);
        isDone = false;
    }
private:
    bool isDone;

    bool checkIsFinished() {
        isDone = mt.empty() ? true : all_of(mt.cbegin(), mt.cend(), [](const MultiThread* m){return m->isFinished();});
        return isDone;
    }

    void updateThreadCount(unsigned int num) {
        if (isDone) {
            if (mt.size() != num) {
                for (auto &p: mt) {
                    delete p;
                }
                mt.clear();
                while (mt.size() < num) {
                    mt.push_back(new MultiThread());
                }
            }
        }
    }
};


#endif // MULTITHREAD_H
