#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
class Genome;
class Creature;
#include "Factory.h"
#include "Phylogeny.h"
#include "Fitness.h"
#include "Trail.h"

class CreatureViewer {
    public:
        Creature* body;
        Phylogeny phylogeny;
        FitnessCollector fitness;

        CreatureViewer(const Genome& g, Creature* b);
        CreatureViewer(const std::string& g, Creature* b);
        CreatureViewer(const CreatureViewer &other);
        ~CreatureViewer();
        CreatureViewer& operator=(CreatureViewer& other);

        void draw() const;
        void drawBrain(bool drawAxons) const;
        void drawTrails(bool drawCOMTrails) const;
        void drawDebug(bool doDraw) const;

        void update(int t);

    private:
        Trail COMTrail;
};

class Viewer {
    public:
        Viewer(std::vector<Genome*> population, const Factory& factory);
        Viewer(const Viewer& other) = delete;
        ~Viewer();
        operator=(const Viewer& other) = delete;

        /// Displayed Creature Methods
        void draw() const;
        void nextStep();
        int getSimStep() const;
        void printCurrentGenome() const;
        void printGenome(int index) const;

        /// Updating Displayed Creature
        void nextCreature();
        void prevCreature();
        void showCreature(int index);
        void resetCreature();

        /// Updating Genomes
        void updateViewingGenomes(std::vector<Genome*> population);

        /// Getters
        int getMemberIndex() const;
        const CreatureViewer& getActiveCreature() const;

    private:
        const Factory& factory;
    	CreatureViewer* displayingCreature;
        std::vector<Genome> viewingGenomes;
        int activeCreatureIndex;
        int simStep;
};

#endif // VIEWER_H
