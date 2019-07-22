#include "Creature.h"

#include "Genome.h"

#include <stdexcept>


Creature::Creature() : terrain(), moveTo(Vec(0,0,0)), initCOM(Vec(0, 0, 0)), positions(25, 1) {
	positions.addPoint(this->initCOM);
}

Creature::Creature(const Genome&) : Creature() {

}

Creature::Creature(const Creature &other) : terrain(other.terrain), moveTo(other.moveTo), initCOM(other.initCOM), positions(other.positions) {

}

void Creature::drawBrain(const bool) const {
    // Do nothing by default
}

void Creature::drawDebug(bool) const {
	// Do nothing by default
}
Vec Creature::getCOM(int offset) const {
	if (offset > 0) throw new std::out_of_range("Provided offset should be negative.");
	if (positions.size() - 1 - offset < 0 ) throw new std::out_of_range("Requested com at invalid index.");
	return positions[positions.size() - 1 + offset];
}

void Creature::centerCOM() {
   this->moveCOMTo(Vec(0,0, this->getCOM().z));
}

std::string Creature::getGenomeString() const {
    return "<MetaData>";
}

void Creature::update(Senario*, int t) {
	if (this->positions.size() <= 2) {
		while (this->positions.size() <= 2) {
	        this->positions.addPoint(this->calculateCOM());
	    }
	} else if (t % positions.samplingFrequency == 0) {
		this->positions.addPoint(this->calculateCOM());
	}
}



