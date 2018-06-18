#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
//#include "Creature.h"
#include "History.h"

///< @todo Fill population with sorted, unmutated creatures only.
class Creature;
class MultiThread;
class Population { // is this becoming a God class?
    public:
        Population(int numMembers);
        Population(const Population& other) = delete;
        operator=(const Population& other) = delete;
        virtual ~Population();

        // Why not just store the strings instead of genomes?
        std::vector<Genome*> population;     ///< The set of genomes currently evolving.
        std::vector<Genome> viewingGenomes;  ///< The set of genomes currently being looked at.
        Creature * displayingCreature;       ///< The creature which is shown to the user. Future expansion might make this a vector
        History history;                     ///< The data collected at various geneations.
        int gen;                             ///< The current number of generations that have evolved.

        /// Statistics Functions
        double getAvg() const;
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
        void showCreature(int index); // todo: show best, show worst, show average, etc
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
