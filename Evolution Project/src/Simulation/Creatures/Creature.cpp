#include "Creature.h"

#include "MuscleGene.h"
#include "NodeGene.h"
#include "Ball.h"
#include "Piston.h"
#include "Genome.h"
#include "Phylogeny.h"
#include <utility>

#include "StickBall.h"

Creature::Creature(const std::string& g, Body* b) : Creature(Genome(g), b) {}
Creature::Creature(const Genome& genome, Body* b) : body(b), phylogeny(genome), fitness(),
             COMTrail(), nodeTrails(20) {
    body->moveTo = Vec(pmRandf(100, 200), 0*pmRandf(70, 100), 0);
}

Creature::Creature(const Creature &other) :
    body(other.body), phylogeny(other.phylogeny), fitness(other.fitness), COMTrail(other.COMTrail), nodeTrails(other.nodeTrails) {

}

Creature::~Creature() {
    delete body;
}

Creature& Creature::operator=(Creature& other) {
    std::swap(COMTrail, other.COMTrail);
    std::swap(nodeTrails, other.nodeTrails);
    std::swap(fitness, fitness);
    std::swap(body, other.body);
    return *this;
}

#include "Shapes.h"
void Creature::draw() const {
    body->draw();
    DrawRing<Appearance::WHITE>(Vec(body->com.x, body->com.y, 0), 10, 0.5);
}

void Creature::drawBrain(bool drawLines) const {
    body->drawBrain(drawLines);
}

#include "Text.h"
#include "utility.h"
void Creature::drawNodeData() const {
//    auto poses = body->getNodePositions();
//    int i = 0;
//    for (const auto& pos: poses) {
//        //DrawString<Appearance::BLUE_VIOLET>(utility::numToStr<double>((pos.x - body->com.x)/body->radius-1), pos, false, false);
//        DrawString<Appearance::BLUE_VIOLET>(utility::numToStr<int>(i), pos, false, false);
//        i++;
//    }
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


void Creature::update(int t) { // returns fitness update
    if (t % Trail::SAMPLING_FREQUENCY == 0) {
       COMTrail.addPoint(body->com);
       nodeTrails[0].addPoint(body->com);
       // auto positions = body->getNodePositions();
       // for (unsigned int i = 0; i < positions.size(); i++) {
       //     nodeTrails[i].addPoint(positions[i]);
       // }
    }
//    if (t == 0) {
//        body->moveTo = Vec(pmRandf(70, 100), pmRandf(70, 100), 0);
//    }
//    fitness.preUpdate(&body);

    body->update(t);
    fitness.postUpdate(FitnessCollector::MOVE_TO, *body);
    //fitness.postUpdate(FitnessCollector::TOTAL_DISTANCE, body);
}











