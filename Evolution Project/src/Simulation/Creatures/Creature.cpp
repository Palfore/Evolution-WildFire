#include "Creature.h"

#include "MuscleGene.h"
#include "NodeGene.h"
#include "Ball.h"
#include "Piston.h"
#include "Genome.h"

#include <utility>

Creature::Creature(std::string g) : Creature(Genome(g)) {}
Creature::Creature(int n, int m, int b) : Creature(Genome(n, m, b)) {}
Creature::Creature(Genome genome) : nodes({}), muscles({}), bones({}) {
    for (auto const& gene: genome.getGenes<NodeGene>()) {
        this->nodes.push_back(new Ball(*gene));
    }
    for (auto const& gene: genome.getGenes<MuscleGene>()) {
        this->muscles.push_back(new Piston(*gene, this->nodes));
    }
    for (auto const& gene: genome.getGenes<BoneGene>()) {
        this->bones.push_back(new Piston(*static_cast<MuscleGene*>(gene), this->nodes));
    }
  //  lowerToGround();
 //   centerCOG();
}



Creature::~Creature() {
    for (Ball* b : nodes) {
        delete b;
    }
    for (Piston* m : muscles) {
        delete m;
    }
    for (Piston* b : bones) {
        delete b;
    }
}

Creature::Creature(const Creature &other) : nodes({}), muscles({}), bones({}) {
    for (auto const& node: other.nodes) {
        this->nodes.push_back(new Ball(*node));
    }
    for (auto const& muscle: other.muscles) {
        this->muscles.push_back(new Piston(muscle->getIndex1(), muscle->getIndex2(),
                                            this->nodes[muscle->getIndex1()], this->nodes[muscle->getIndex2()], muscle->speed));
    }
    for (auto const& bone: other.bones) {
        this->bones.push_back(new Piston(bone->getIndex1(), bone->getIndex2(),
                                            this->nodes[bone->getIndex1()], this->nodes[bone->getIndex2()], bone->speed));
    }
}

Creature& Creature::operator=(Creature other) {
    std::swap(nodes, other.nodes);
    std::swap(muscles, other.muscles);
    std::swap(bones, other.bones);
    return *this;
}


void Creature::moveCOGTo(Vec to) {
    Vec COG = getCOG();
    Vec dr = to - COG;
    for (auto * node : this->nodes) {
        node->position += dr;
    }
}
void Creature::lowerToGround() {
    double dz = this->getLowestBodyHeight() - 1; //-1 for not embeding creatures
    for (auto * node : this->nodes) {
        node->position.z -= dz;
    }
}
void Creature::centerCOG() {
    this->moveCOGTo(Vec(0,0, this->getCOG().z));
}

Vec Creature::getCOM() const {
    Vec COM = Vec(0,0,0);
    double mass = 0.0;
    for (auto const& node : this->nodes) {
            COM += node->position * node->mass;
            mass += node->mass;
    }
    if (mass < 0.000000001) return Vec(0,0,0);
    return COM / mass;
}

Vec Creature::getCOG() const {
    Vec COM = Vec(0,0,0);
    for (auto const& node : this->nodes) {
            COM += node->position;
    }
    return COM;
}

#include <limits>
double Creature::getLowestBodyHeight() const {
    double minHeight = std::numeric_limits<double>::max();
    for (auto const& node : this->nodes) {
        double lowestPart = node->position.z - node->radius;
        minHeight = lowestPart < minHeight  ? lowestPart : minHeight ;
    }
    return minHeight;
}

double Creature::getFitness() const {
//    return randf(0, 100);
    return euc2D(Vec(0,0,0), this->getCOM());
}

#include "Shapes.h"
void Creature::draw(double t) const {
    DrawSphere<Appearance::BLUE>(getCOM(), 0.5);
    for (auto const& node : this->nodes) {
        node->draw(t);
    }
    for (auto const& muscle : this->muscles) {
        muscle->draw(t);
    }
    for (auto const& bone : this->bones) {
        bone->draw(t);
    }
}

void Creature::update(double t) {
    double dt = 1;
    for (auto const& node: this->nodes) {
        node->acceleration = Vec(0, 0, -0.0066);
    }

    /* Get Muscle Forces : Find force needed to get to this frames length */
    for (Piston * muscle: this->muscles) {
        double currLength    = euc(muscle->getPosition1(), muscle->getPosition2());
        double desiredLength = muscle->initialLength * (1 + 0.2 * sin(0.005 * (10* ((t) / dt))));
        double deviation     = desiredLength - currLength;

        Vec radVector = (muscle->getPosition1() - muscle->getPosition2());
        Vec deltaAcc  = radVector * (deviation * 0.4) / (radVector.length() * dt * dt);

        this->nodes[muscle->getIndex1()]->acceleration += deltaAcc;
        this->nodes[muscle->getIndex2()]->acceleration -= deltaAcc;
    }
    for (Piston * bone: this->bones) {
        double currLength    = euc(bone->getPosition1(), bone->getPosition2());
        double desiredLength = bone->initialLength;
        double deviation     = desiredLength - currLength;

        Vec radVector = (bone->getPosition1() - bone->getPosition2());
        Vec deltaAcc  = radVector * (deviation * 0.4) / (radVector.length() * dt * dt);

        this->nodes[bone->getIndex1()]->acceleration += deltaAcc;
        this->nodes[bone->getIndex2()]->acceleration -= deltaAcc;
    }

    for (auto const& node: this->nodes) {
        /// Apply Constraints
        if (node->position.z < node->radius) {
            node->position.z = node->radius;

            node->velocity.x *= (1 - 0.5 * dt) ;// * (isinf(x) || (x>100)? 1 : x / sqrt(1 + x*x));
            node->velocity.y *= (1 - 0.5 * dt) ;// * (isinf(y) || (y>100)? 1 : y / sqrt(1 + y*y));
            node->velocity.z *= -0.01;
        }

        node->velocity *= 1 - 0.03 * dt;


        /// Apply Kinematics
        node->velocity += node->acceleration * dt;
        node->position += node->velocity * dt;

    }
    if (t<0) t = t;
}



