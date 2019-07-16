#include "Vec.h"
#include "Cube.h"
#include "CubeGene.h"
#include "Shapes.h"

Cube::Cube(CubeGene cubeGene) : Cube(cubeGene.position, cubeGene.length, 1) {}
Cube::Cube(Vec pos, double l, double m) : position(pos), velocity(Vec(0,0,0)), acceleration(Vec(0,0,0)), length(l), mass(m) {}
Cube::~Cube() {}

void Cube::draw() const {
    double massPercentage = 1; //- (this->mass - NodeGene::MIN_MASS) / (NodeGene::MAX_MASS - NodeGene::MIN_MASS);
    GLOBAL->colorMap[Appearance::CUSTOM] =  GLOBAL->colorMap[Appearance::LIGHT_BLUE] * massPercentage;
    DrawCube<Appearance::CUSTOM>(this->position, this->length);
}

void Cube::update(int time) {
    if (time < 0) time = time;
}
