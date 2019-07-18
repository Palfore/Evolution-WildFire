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
   sizes.push_back(2);
   for (unsigned int layer = 0; layer < sizes.size() - 1; layer++) {
       for (unsigned int i = 0; i < sizes[layer]; i++) {
           for (unsigned int j = 0; j < sizes[layer+1]; j++) {
               g->addGene(new AxonGene(i, j, layer, pmRandf(1)));
           }
       }
   }
   return g;
}

Cuboid::Cuboid(const Genome& genome) : Creature(genome), head(nullptr), cubes({}), NN(genome.getGenes<AxonGene>()), positions({}) {
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


Cuboid::Cuboid(const Cuboid &other) : Creature(other), head(nullptr), cubes({}), NN(other.NN) {
    for (auto const& cube: other.cubes) {
        this->cubes.push_back(new Cube(*cube));
    }
    this->head = &com;
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

    Vec fcom = Vec(com.x, com.y, 0);
    Vec fpcom = Vec(prevCOM.x, prevCOM.y, 0);
    Vec averageP = Vec(0, 0, 0);
    Vec averageV = Vec(0, 0, 0);
    for (unsigned int i = 0; i < this->positions.size(); i++) {
        averageP += this->positions[i];
    }
    for (unsigned int i = 0; i < this->positions.size() - 1; i++) {
        averageV += this->positions[i+1] - this->positions[i];
    }
    averageP /= this->positions.size();
    averageV /= this->positions.size() - 1;
    averageV = Vec(averageV.x, averageV.y, 0);

    DrawCylinder<Appearance::WHITE>(
        averageP+Vec(0, 0, 20),
        averageP+Vec(0, 0, 20)-50*(averageV).getUnit(),
        1
    );

    DrawCylinder<Appearance::RED>(
        fcom+Vec(0, 0, 10),
        fcom+Vec(0, 0, 10)+50*(com - prevCOM).getUnit(),
        1
    );

    Vec red = (fcom - fpcom);
    Vec blue = (moveTo - fpcom);
    // double angle = acos(red.dot(blue) / (red.length() * blue.length()));
    double angle = atan2(red.cross(blue).length(), red.dot(blue));
    double sign = sgn( (fpcom.x - fcom.x)*(moveTo.y - fcom.y) - (fpcom.y - fcom.y)*(moveTo.x - fcom.x) );
    angle *= sign / PI;
    // angle = angle < PI ? angle : angle - 2*PI;
    // std::cout << 180*angle / PI << ", "<< angle<<'\n';
    if (angle < 1e-8) {
        return;
    }


    DrawCylinder<Appearance::GREEN>(
        fcom+Vec(0, 0, 10)+50*(com - prevCOM).getUnit(),
        fpcom+Vec(0, 0, 10)+(moveTo - prevCOM),
        1
    );
    DrawCylinder<Appearance::BLUE>(
        fpcom+Vec(0, 0, 10),
        fcom+Vec(0, 0, 10)+(moveTo - com),
        1
    );
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

#include <numeric>
void Cuboid::update(int t) {
    constexpr double dt = 0.1;

    if (this->positions.size() >= 25) {
        this->positions.pop_back();
    } else if (this->positions.size() == 0) {
        this->positions.push_front(this->com);
    }
    if (t % 5 == 0) {
        this->positions.push_front(this->com);
    }
    const Vec averageP = std::accumulate(this->positions.begin(), this->positions.end(), Vec(0, 0, 0)) / this->positions.size();
    const Vec averageV = (this->positions[this->positions.size() - 1] - this->positions[0]);

    const Vec fcom  = averageP;
    const Vec fpcom = averageP - averageV;
    const Vec red   = averageV;
    const Vec blue  = -(moveTo - fpcom);
    const Vec green = (moveTo - fcom);
    const double sign = sgn(averageV.y*green.x - averageV.x*green.y); // xprod
    const double angle = sign * atan2(red.cross(blue).length(), red.dot(blue)) / PI;

    std::vector<double> inputs = {1};
    inputs.push_back(angle);
    inputs.push_back(tanh(0.01*green.length()));

    this->prevCOM = this->com;
    std::vector<double> da = NN.propagate(inputs);
    for (unsigned int i = 0; i < this->cubes.size(); i++) {
        Vec para = -averageV;  // Signs chosen for +1 forward/right
        Vec perp = -averageV.cross(Vec(0, 0, 1));
        para = para.length() < 1e-8 ? Vec(0, 1, 0) : para / para.length();
        perp = perp.length() < 1e-8 ? Vec(0, 1, 0) : perp / perp.length();

        this->cubes[i]->acceleration = (da[0]*perp + da[1]*para) * dt;
        this->cubes[i]->acceleration += -0.25*this->cubes[i]->velocity.lengthSquare() * para * dt; // Drag
        this->cubes[i]->velocity += this->cubes[i]->acceleration * dt;
        this->cubes[i]->position += this->cubes[i]->velocity * dt;

        this->cubes[i]->velocity *= 1 - 0.01 * dt;
    }
    this->com = getCOM();
    if (t > 0) return;
}


std::string Cuboid::getGenomeString() const {
    std::string genomeString = Creature::getGenomeString();
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




