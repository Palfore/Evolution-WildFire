#include "Ball.h"
#include "Shapes.h"

Ball::Ball(Vec pos) : Ball(pos, 1) {}
Ball::Ball(Vec pos, double r) : Ball(pos, r, 1) {}
Ball::Ball(Vec pos, double r, double m) : position(pos), velocity(Vec(0,0,0)), acceleration(Vec(0,0,0)), radius(r), mass(m) {}
Ball::Ball(NodeGene nodeGene) : Ball(nodeGene.position, 1, nodeGene.mass) {}
Ball::~Ball() {}

void Ball::draw() const {
    double massPercentage = 1 - (this->mass - NodeGene::MIN_MASS) / (NodeGene::MAX_MASS - NodeGene::MIN_MASS);
    GLOBAL->colorMap[Appearance::CUSTOM] =  GLOBAL->colorMap[Appearance::LIGHT_BLUE] * massPercentage;
    DrawSphere<Appearance::CUSTOM>(this->position, this->radius);
}

void Ball::update(int time) {
    if (time < 0) time = time;
}
