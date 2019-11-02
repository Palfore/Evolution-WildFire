#ifndef VIEWER_H
#define VIEWER_H

#include "Phylogeny.h"
#include "Fitness.h"
#include "Trail.h"

#include <vector>

class Genome;
class Creature;
class Scenario;
class ScenarioFactory;
class Factory;
class Viewer {
    public:
        Phylogeny phylogeny;
        double fitness;

        Viewer(std::vector<Genome*> population, const Factory& factory, const ScenarioFactory& scenarioFactory);
        Viewer(const Viewer& other) = delete;
        ~Viewer();
        Viewer& operator=(const Viewer& other) = delete;

        /// Displayed Creature Methods
        void draw() const;
        void drawBrain(bool drawAxons) const;
        void drawTrails(bool drawCOMTrails) const;
        void drawDebug(bool doDraw) const;

        void moveTo(const Vec& v);
        void update();
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
        const Creature& getCurrent() const;
        const Scenario& getScenario() const;
        int getSimStep() const;
        int getMemberIndex() const;

    private:
        const Factory& factory;
        const ScenarioFactory& scenarioFactory;
        Scenario* scenario;
        std::vector<Genome> viewingGenomes;
        int activeCreatureIndex;
        int simStep;

        Trail COMTrail;
};

#endif // VIEWER_H
