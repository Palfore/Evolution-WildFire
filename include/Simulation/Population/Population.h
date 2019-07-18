#ifndef POPULATION_H
#define POPULATION_H

#include "History.h"
#include "Genome.h"
#include <vector>

///< @todo Fill population with sorted, unmutated creatures only.
class Creature;
class MultiThread;
#include "Factory.h"

class Population {
    public:
        Population(int numMembers, const Factory& creatureFactory);
        Population(const Population& other) = delete;
        operator=(const Population& other) = delete;
        virtual ~Population();

        std::vector<Genome*> population;     ///< The set of genomes currently evolving.
        History history;                     ///< The data collected at various geneations.
        int gen;                             ///< The current number of generations that have evolved.

        /// Statistics Functions
        double getAvg() const;
        void recordHistory();
        void printHistory();

        /// Processing Population
        std::vector<Creature*> getBodies() const;
        void getThreadedFitnesses(const std::vector<MultiThread*>& threads);
        void updateFitnesses(const std::vector<double>& fitnesses);
        void sortPop();
        void select();
        void mutate();

    private:
        const Factory& factory;

};

#endif // POPULATION_H
