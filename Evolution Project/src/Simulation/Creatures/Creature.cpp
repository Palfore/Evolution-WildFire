#include "Creature.h"

#include "MuscleGene.h"
#include "NodeGene.h"
#include "Ball.h"
#include "Piston.h"
#include "Genome.h"
#include "Phylogeny.h"
#include <utility>


Creature::Creature(const std::string& g) : Creature(Genome(g)) {}
Creature::Creature(int n, int m, int b, const std::vector<unsigned int>& N) : Creature(Genome(n, m, b, N)) {}
Creature::Creature(const Genome& genome) : body(genome), phylogeny(genome), fitness(), headIndex(body.getTopNodeIndex()), COMTrail(), nodeTrails(genome.getGenes<NodeGene>().size()) {

}

Creature::~Creature() {

}

Creature::Creature(const Creature &other) :
    body(other.body), phylogeny(other.phylogeny), fitness(other.fitness), headIndex(other.headIndex), COMTrail(other.COMTrail), nodeTrails(other.nodeTrails) {

}

Creature& Creature::operator=(Creature& other) {
    std::swap(headIndex, other.headIndex);
    std::swap(COMTrail, other.COMTrail);
    std::swap(nodeTrails, other.nodeTrails);
    std::swap(fitness, fitness);
    std::swap(body, other.body);
    return *this;
}

void Creature::draw() const {
    body.draw();
}

void Creature::drawBrain(bool drawLines) const {
    body.drawBrain(drawLines);
}

#include "Text.h"
#include "utility.h"
void Creature::drawNodeData() const {
    auto poses = body.getNodePositions();
    for (const auto& pos: poses) {
        DrawString<Appearance::BLUE_VIOLET>(utility::numToStr<double>((pos.x - body.com.x)/body.radius-1), pos, false, false);
    }
}

void Creature::drawTrails(bool drawNodeTrails, bool drawCOMTrails) const {
    if (drawNodeTrails) {
        for (auto const& trail : this->nodeTrails) {
            trail.draw();
        }
    }
    if (drawCOMTrails) {
        COMTrail.draw();
    }
}

Vec Creature::getHead() const {
    return body.getNodePosition(headIndex);
}

void Creature::update(int t) { // returns fitness update
    if (t % Trail::SAMPLING_FREQUENCY == 0) {
        COMTrail.addPoint(body.com);
        auto positions = body.getNodePositions();
        for (unsigned int i = 0; i < positions.size(); i++) {
            nodeTrails[i].addPoint(positions[i]);
        }
    }
//    if (t == 0) {
//        body.moveTo = Vec(pmRandf(70, 100), pmRandf(70, 100), 0);
//    }
    fitness.preUpdate(&body);
    body.update(t);
    fitness.postUpdate(FitnessCollector::MOVE_TO, body);
    fitness.postUpdate(FitnessCollector::TOTAL_DISTANCE, body);
}











