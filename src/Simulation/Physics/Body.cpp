#include "Body.h"

#include "MuscleGene.h"
#include "NodeGene.h"
#include "CubeGene.h"
#include "Ball.h"
#include "Cube.h"
#include "Piston.h"
#include "Genome.h"
#include "Phylogeny.h"
#include <utility>

Body::Body(const Genome& genome) : moveTo(Vec(0,0,0)), com(Vec(0,0,0)), initCOM(Vec(0,0,0)), prevCOM(Vec(0, 0, 0)) {

}

Body::Body(const Body &other) : moveTo(other.moveTo), com(other.com), initCOM(other.initCOM), prevCOM(other.prevCOM) {
    // Empty
}

void Body::drawBrain(const bool drawAxons) const {
    // Do nothing
}

void Body::centerCOM() {
   this->moveCOMTo(Vec(0,0, this->getCOM().z));
}

std::string Body::getGenomeString() const {
    return "<MetaData>";
}










