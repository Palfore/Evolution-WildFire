#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "Creature.h"

#include <functional>
#include <thread>
class MultiThread {
    public:
        std::vector<double> fitnesses;
        MultiThread() : fitnesses({}), t(), finished(false) {}

        void spawn(std::function<void(std::vector<Creature>, std::vector<double> &, bool &)> f, std::vector<Creature> creatures) {
            finished = false;
            fitnesses.clear();
            t = std::thread(f, creatures, std::ref(this->fitnesses), std::ref(this->finished));
            t.detach(); // need to return values & reinstansiate thread.
        }

        bool isFinished() const {
            return finished;
        }

        static void processCreatures(std::vector<Creature> creatures, std::vector<double> & fitnesses, bool & done) {
            for (auto creature: creatures) {
                double initialFitness = 0;
                for (int i = 0; i < 9000; i++) {
                    initialFitness = i == 1000 ? creature.getFitness() : initialFitness;
                    creature.update(i);
                }
                fitnesses.push_back(creature.getFitness() - initialFitness);
            }
            done = true;
        }

        static void spawnChildren(std::vector<MultiThread*> mt, int numThreads, std::vector<Creature> creatures) {
            auto begining = creatures.cbegin();
            const int blockSize = creatures.size() / numThreads;
            for (int i = 0; i < numThreads; i++) {
                auto e1 = begining + i     * blockSize;
                auto e2 = begining + (i+1) * blockSize;
                if (i == numThreads - 1) e2 = creatures.cend();
                mt[i]->spawn(processCreatures, std::vector<Creature>(e1, e2));
            }
        }

    private:
        std::thread t;
        bool finished;
};

class Population {
    public:
        std::vector<Genome*> population;
        int gen;
        Creature currentMember;

        Population(int numMembers);
        virtual ~Population();


        /// Statistics Functions
        double getAvg() const; // todo: Percentiles, stdd, max, min, ...

        /// Getting Details
        void printCurrentGenome() const;
        void printGenome(int index) const;  // todo: print best, wosrt, average (one closest to average fitness)

        /// Handling Current Creature
        void draw() const;
        void nextStep();
        void nextCreature();
        void showCreature(int index); // todo: show best, show worst, show average
        int getMemberIndex() const;

        /// Processing Population
        std::vector<Creature> getCreatures() const;
        void getThreadedFitnesses(const std::vector<MultiThread*>& threads);
        void sortPop();
        void select();
        void mutate();
    private:
        int memberIndex;
        int simStep;
};

#endif // POPULATION_H
