#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "Creature.h"
template<class TmemberType>
class Population {
    public:
        Population(int numMembers);
        virtual ~Population();

        void draw() const;
        void nextStep();
        std::vector<Creature> getCreatures() const;

        /// Statistics Functions
        double getAvg() const;

        std::vector<TmemberType*> population;
        int gen;
        Creature currentMember;
        void nextCreature();
        void showCreature(int index);
        // show best, show worst

    private:
        int memberIndex;
        int simStep;



};

#endif // POPULATION_H
