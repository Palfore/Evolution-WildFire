#include "Creature.h"

#include "MuscleGene.h"
#include "NodeGene.h"
#include "CubeGene.h"
#include "Ball.h"
#include "Cube.h"
#include "Piston.h"
#include "Genome.h"
#include "Phylogeny.h"
#include <utility>

Creature::Creature() : moveTo(Vec(0,0,0)), com(Vec(0,0,0)), initCOM(Vec(0,0,0)), prevCOM(Vec(0, 0, 0)) {

}

Creature::Creature(const Genome&) : Creature() {

}

Creature::Creature(const Creature &other) : moveTo(other.moveTo), com(other.com), initCOM(other.initCOM), prevCOM(other.prevCOM) {

}

void Creature::drawBrain(const bool) const {
    // Do nothing by default
}

void Creature::drawDebug(bool) const {
	// Do nothing by default
}

void Creature::centerCOM() {
   this->moveCOMTo(Vec(0,0, this->getCOM().z));
}

std::string Creature::getGenomeString() const {
    return "<MetaData>";
}










