#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
class Genome;
class Creature;
#include "Factory.h"

struct Viewer {
    Viewer(std::vector<Genome*> population, const Factory factory);
    Viewer(const Viewer& other) = delete;
    operator=(const Viewer& other) = delete;

	Creature * displayingCreature;       ///< The creature which is shown to the user. Future expansion might make this a vector

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
    void updateViewingGenomes(std::vector<Genome*> population);

    int getMemberIndex() const;
    const Creature& getActiveCreature() const;

private:
    const Factory factory;
    std::vector<Genome> viewingGenomes;
    int activeCreatureIndex;
    int simStep;
};

#endif // VIEWER_H
