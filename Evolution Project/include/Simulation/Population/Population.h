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
            t.detach();
        }

        bool isFinished() const {
            return finished;
        }

        static void processCreatures(std::vector<Creature> creatures, std::vector<double> & fitnesses, bool & done) {
            for (auto creature: creatures) {
                for (int i = 0; i < 9000; i++) {
                    creature.update(i);
                }
                fitnesses.push_back(creature.getFitness());
            }
            done = true;
        }

        static void spawnChildren(std::vector<MultiThread*> mt, std::vector<Creature> creatures) {
            auto begining = creatures.cbegin();
            const int blockSize = creatures.size() / mt.size();
            for (unsigned int i = 0; i < mt.size(); i++) {
                auto e1 = begining + i     * blockSize;
                auto e2 = begining + (i+1) * blockSize;
                if (i == mt.size() - 1) e2 = creatures.cend();
                mt[i]->spawn(processCreatures, std::vector<Creature>(e1, e2));
            }
        }

    private:
        std::thread t;
        bool finished;
};



#include "Genome.h"
#include "utility.h"
#include "Vec2.h"
class HistoryPoint {
    private:
        class Value : public Vec2 {
            public:
                Value(double v, double err) : Vec2(v, err) {}
                Value() : Vec2(0, 0) {}
                virtual ~Value() {}

                std::string toString() const {
                    return utility::numToStr<double>(x) + "+/-" + utility::numToStr<double>(y);
                }
        };

        class GenomeValue {
            public:
                GenomeValue(double v, std::string g) : x(v), genome(g) {}
                GenomeValue() : x(0), genome("") {}
                virtual ~GenomeValue() {}

                std::string toString() const {
                    return utility::numToStr<double>(x) ;//+ ':' + genome;
                }
double x;
            private:

                std::string genome;
        };

    public:
        static constexpr int NUM_PERCENTILES = 10;

        HistoryPoint(std::vector<Genome*> genomes, int gen=-1) :
                generation(gen), mean(), top(), middle(), bottom(), percentiles({{}}) {
            /// These calculations assume and ordered set where 0 is worst, .size() is best.
            for (const Genome* g: genomes) {
                mean.x += g->fitness;
            }
            mean.x /= genomes.size();


            top    = GenomeValue(genomes[genomes.size() - 1]->fitness, genomes[genomes.size() - 1]->toString());
            middle = GenomeValue(genomes[genomes.size() / 2]->fitness, genomes[genomes.size() / 2]->toString());
            bottom = GenomeValue(genomes[0]->fitness, genomes[0]->toString());

            const double percentileSize = genomes.size() / NUM_PERCENTILES;
            for (unsigned int i = 0; i < NUM_PERCENTILES; i++) { // Approxmiate calculation
                const int percentileIndex = i * percentileSize;

                percentiles[i] = GenomeValue(genomes[percentileIndex]->fitness,
                                                           genomes[percentileIndex]->toString());
            }
        }
        ~HistoryPoint() {}

        std::string toString() const {
            std::string s = utility::numToStr<int>(generation) + ", " +
                mean.toString()   + ", " +
                top.toString()    + ", " +
                middle.toString() + ", " +
                bottom.toString() + ", ";
            for (const GenomeValue& p: percentiles) {
                s += p.toString() + ", ";
            }
            return s;
        }

        int generation;
        Value mean;
        GenomeValue top;
        GenomeValue middle;
        GenomeValue bottom;
        std::array<GenomeValue, NUM_PERCENTILES> percentiles;

};

class History {
    public:
        History() : data({}) {}
        virtual ~History() {}

        void addPoint(std::vector<Genome*> genomes, int gen) {
            data.push_back(HistoryPoint(genomes, gen));
        }

        void writeToFile() {

        }

        void writeToConsole() const {
            for (const HistoryPoint h: data) {
                std::cout << h.toString() << '\n';
            }
        }

    private:
        std::vector<HistoryPoint> data;
};

class Population {
    public:
        std::vector<Genome*> population;

        History history;
        int gen;

        std::vector<Genome> viewingGenomes;

        Creature displayingCreature;


        Population(int numMembers);
        virtual ~Population();


        /// Statistics Functions
        double getAvg() const; // todo: Percentiles, stdd, max, min, ...
        void recordHistory();
        void printHistory();

        /// Getting Details
        void printCurrentGenome() const;
        void printGenome(int index) const;  // todo: print best, wosrt, average (one closest to average fitness)

        /// Handling Current Creature
        void draw() const;
        void nextStep();
        int getSimStep() const;

        void nextCreature();
        void prevCreature();
        void showCreature(int index); // todo: show best, show worst, show average
        void resetCreature();
        void updateViewingGenomes();

        int getMemberIndex() const;
        Creature getActiveCreature() const;

        /// Processing Population
        std::vector<Creature> getCreatures() const;
        void getThreadedFitnesses(const std::vector<MultiThread*>& threads);
        void sortPop();
        void select();
        void mutate();
    private:
        int activeCreatureIndex;
        int simStep;
};

#endif // POPULATION_H
