#include "Ball.h"
#include "Shapes.h"

Ball::Ball(Vec pos) : Ball(pos, 1) {}
Ball::Ball(Vec pos, double r) : Ball(pos, r, 1) {}
Ball::Ball(Vec pos, double r, double m) : position(pos), velocity(Vec(0,0,0)), acceleration(Vec(0,0,0)), radius(r), mass(m) {}
Ball::Ball(NodeGene nodeGene) : Ball(nodeGene.position, 1, nodeGene.mass) {}
Ball::~Ball() {}

void Ball::draw(double time) const {
    DrawSphere<Appearance::BLUE_VIOLET>(this->position, this->radius);
    if (time < 0) time = time;
}

void Ball::update(double time) {
    if (time < 0) time = time;
}
