#include "Cuboid.h"
#include "Genome.h"
#include "CubeGene.h"
#include "AxonGene.h"
#include "NodeGene.h"
#include "Logger.h"
#include "utility.h"
#include "Shapes.h"
#include <limits>


Genome* Cuboid::createGenome(Vec p, double l, std::vector<unsigned int> sizes) {
   Genome *g = new Genome();
   g->addGene(new CubeGene(p, l));

   sizes.insert(sizes.begin(), 3);
   sizes.push_back(5);
   for (unsigned int layer = 0; layer < sizes.size() - 1; layer++) {
       for (unsigned int i = 0; i < sizes[layer]; i++) {
           for (unsigned int j = 0; j < sizes[layer+1]; j++) {
               g->addGene(new AxonGene(i, j, layer, pmRandf(1)));
           }
       }
   }
   return g;
}

Cuboid::Cuboid(const Genome& genome) : Body(genome), head(nullptr), cubes({}), NN(genome.getGenes<AxonGene>()) {
    for (auto const& gene: genome.getGenes<CubeGene>()) {
        this->cubes.push_back(new Cube(*gene));
    }

    lowerToGround();
    centerCOM();
    com = prevCOM = initCOM = getCOM();
    this->head = &com;
}


Cuboid::~Cuboid() {
    for (auto* b : this->cubes) {
        delete b;
    }
}


Cuboid::Cuboid(const Cuboid &other) : Body(other), cubes({}), NN(other.NN) {
    for (auto const& cube: other.cubes) {
        this->cubes.push_back(new Cube(*cube));
    }
}

void Cuboid::draw() const {
    DrawSphere<Appearance::FACE>(*head + Vec(0,0,8), 5,
        180.0/3.1415926*atan2(moveTo.y - com.y, moveTo.x - com.x) + 90
    );
    for (auto const& cube: this->cubes) {
    	DrawCylinder<Appearance::BLUE>(
    		cube->position - Vec(0, 0, cube->length/2),
    		cube->position + Vec(0, 0, cube->length/2),
    		5
    	);
    }

}

void Cuboid::drawBrain(const bool drawLines) const {
    this->NN.draw(drawLines);
}


Vec Cuboid::getCOM() const {
    Vec COM = Vec(0,0,0);
    double mass = 0.0;
    for (auto const& cube : this->cubes) {
        COM += cube->position * cube->mass;
        mass += cube->mass;
    }
    if (mass < 0.000000001) return Vec(0,0,0);
    return COM / mass;
}


void Cuboid::moveCOMTo(Vec to) {
    Vec COM = getCOM();
    Vec dr = to - COM;
    for (auto * cube : this->cubes) {
        cube->position += dr;
    }
}

void Cuboid::lowerToGround() {
    double dz = this->getLowestBodyHeight() - 1; //-1 for not embeding creatures/padding
    for (auto * cube : this->cubes) {
        cube->position.z -= dz;
    }
}

double Cuboid::getLowestBodyHeight() const {
    double minHeight = std::numeric_limits<double>::max();
    for (auto const& cube : this->cubes) {
        double lowestPart = cube->position.z - cube->length / 2;
        minHeight = lowestPart < minHeight  ? lowestPart : minHeight ;
    }
    return minHeight;
}

Vec Cuboid::getTop(const double offset=0) const {
    double highestNode = -std::numeric_limits<double>::max();
    double comX = 0;
    double comY = 0;

    double mass = 0.0;
    for (auto const& cube : this->cubes) {
        highestNode = cube->position.z > highestNode ? cube->position.z : highestNode;
        comX += cube->position.x * cube->mass;
        comY += cube->position.y * cube->mass;
        mass += cube->mass;
    }
    return Vec(comX / mass, comY / mass, highestNode + offset);
}

void Cuboid::update(int t) {
    constexpr double dt = 0.1;
    this->prevCOM = this->com;

    std::vector<double> inputs = {1};
    const double distanceFactorX = tanh(0.01*(com.x - this->moveTo.x)); //should be +/-(something) depending on direction => tanh
    const double distanceFactorY = tanh(0.01*(com.y - this->moveTo.y));
    inputs.push_back(distanceFactorX);
    inputs.push_back(distanceFactorY);


    std::vector<double> desiredChanges = NN.propagate(inputs);
    for (unsigned int i = 0; i < this->cubes.size(); i++) {
        constexpr double v = 0.5;
        this->cubes[i]->acceleration = Vec(v*desiredChanges[0], v*desiredChanges[1], 0) * dt;
        this->cubes[i]->velocity += this->cubes[i]->acceleration * dt;
        this->cubes[i]->position += this->cubes[i]->velocity * dt;

        this->cubes[i]->velocity *= 1 - 0.01 * dt;
    }

    this->com = getCOM();
    if (t > 0) return;
}


std::string Cuboid::getGenomeString() const {
    std::string genomeString = Body::getGenomeString();
    // for (auto const& cube : this->cubes) {
    //     genomeString += Gene::toStringFormat(std::vector<std::string>({std::string(1, NodeGene::symbol),
    //         utility::numToStr<double>(node->position.x),
    //         utility::numToStr<double>(node->position.y),
    //         utility::numToStr<double>(node->position.z),
    //         utility::numToStr<double>(node->mass)
    //     }));
    // }
    exit(-25342532);
    return genomeString;
}




