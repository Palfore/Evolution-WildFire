#include "Piston.h"

#include "Ball.h"
#include "Shapes.h"
#include "MuscleGene.h"
#include "Genome.h"
#include "MyMath.h"

Piston::Piston(MuscleGene muscleGene, std::vector<Ball*> nodes) : initialLength(euc(nodes[muscleGene.connection.a]->position, nodes[muscleGene.connection.b]->position)),
            a(muscleGene.connection.a), b(muscleGene.connection.b), b1(nodes[a]), b2(nodes[b]) {
}

Piston::Piston(int a_t, int b_t, const Ball* ball1, const Ball* ball2) :
                initialLength(euc(ball1->position, ball2->position)),
                a(a_t), b(b_t), b1(ball1), b2(ball2) {}


Piston::~Piston() {}

int Piston::getIndex1() const {
    return this->a;
}

int Piston::getIndex2() const {
    return this->b;
}

Vec Piston::getPosition1() const {
    return this->b1->position;
}

Vec Piston::getPosition2() const {
    return this->b2->position;
}

void Piston::draw(double t) const {
    DrawCylinder<Appearance::RED>(b1->position, b2->position, b1->radius / 2.0);
    if (t < 0) t = t;
}

void Piston::update(double t) {
    if (t < 0) t = t;
}




