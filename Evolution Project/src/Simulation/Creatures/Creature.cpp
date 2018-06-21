#include "Creature.h"

#include "MuscleGene.h"
#include "NodeGene.h"
#include "Ball.h"
#include "Piston.h"
#include "Genome.h"

#include <utility>

Creature::Creature(std::string g) : Creature(Genome(g)) {}
Creature::Creature(int n, int m, int b, std::vector<unsigned int> N) : Creature(Genome(n, m, b, N)) {}
Creature::Creature(Genome genome) : moveTo(Vec(0,0,0)), initCOM(Vec(0,0,0)), nodes({}), muscles({}), bones({}), NN(genome.getGenes<AxonGene>()), genomeString(getGenomeString()) {
    for (auto const& gene: genome.getGenes<NodeGene>()) {
        this->nodes.push_back(new Ball(*gene));
    }
    for (auto const& gene: genome.getGenes<MuscleGene>()) {
        this->muscles.push_back(new Piston(*gene, this->nodes));
    }
    for (auto const& gene: genome.getGenes<BoneGene>()) {
        this->bones.push_back(new Piston(*static_cast<MuscleGene*>(gene), this->nodes));
    }
    lowerToGround();
    centerCOM();
    initCOM = getCOM();
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

Creature::Creature(const Creature &other) : moveTo(other.moveTo), initCOM(other.initCOM), nodes({}), muscles({}), bones({}), NN(other.NN), genomeString(other.genomeString) {
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
    std::swap(NN, other.NN);
    std::swap(genomeString, other.genomeString);
    return *this;
}


void Creature::moveCOGTo(Vec to) {
    Vec COG = getCOG();
    Vec dr = to - COG;
    for (auto * node : this->nodes) {
        node->position += dr;
    }
}

void Creature::moveCOMTo(Vec to) {
    Vec COM = getCOM();
    Vec dr = to - COM;
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

void Creature::centerCOM() {
    this->moveCOMTo(Vec(0,0, this->getCOM().z));
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
    Vec COG = Vec(0,0,0);
    for (auto const& node : this->nodes) {
        COG += node->position;
    }
    return COG;
}
#include <limits>
Vec Creature::getTop() const {
    double highestNode = -std::numeric_limits<double>::max();
    double comX = 0;
    double comY = 0;

    double mass = 0.0;
    for (auto const& node : this->nodes) {
        highestNode = node->position.z > highestNode ? node->position.z : highestNode;
        comX += node->position.x * node->mass;
        comY += node->position.y * node->mass;
        mass += node->mass;
    }
    return Vec(comX / mass, comY / mass, highestNode + 10);
}

Vec Creature::getTopNode() const {
    Vec highestNode = Vec(0, 0, -std::numeric_limits<double>::max());
    for (auto const& node : this->nodes) {
        highestNode = node->position.z > highestNode.z ? node->position : highestNode;
    }
    return highestNode;
}


double Creature::getLowestBodyHeight() const {
    double minHeight = std::numeric_limits<double>::max();
    for (auto const& node : this->nodes) {
        double lowestPart = node->position.z - node->radius;
        minHeight = lowestPart < minHeight  ? lowestPart : minHeight ;
    }
    return minHeight;
}

double Creature::getFitness() const {
    return getFitness(this->getCOM());
}


double Creature::getFitness(Vec com) const {
    return (this->initCOM.x - com.x) - fabs(com.y);
//    return euc2D(this->initCOM, com);
//    return 1.0/cosh(euc2D(com, moveTo));
}

#include "Shapes.h"
void Creature::draw() const {
    DrawSphere<Appearance::BLUE>(getCOM(), 0.5);
    for (auto const& node : this->nodes) {
        node->draw();
    }
    for (auto const& muscle : this->muscles) {
        muscle->draw();
    }
    for (auto const& bone : this->bones) {
        bone->draw();
    }
}

void Creature::drawBrain(bool drawLines) const {
    this->NN.draw(drawLines);
}

double Creature::update(int t) { // returns fitness update
    double fitness = 0;
    const double dt = 1.0;
    const Vec com = getCOM();


//    if (t == 0) {
//        initCOM.x = 75;
//        moveTo.x = initCOM.x;//Vec(initCOM.x, 0, 0);
//    }
//    if (fabs(com.x - moveTo.x) < 5) { // Only if you get it do you get to try again
//        initCOM.x += sgn<double>(com.x - moveTo.x) * (40 + randf(90)); // flip flop the direction
//        moveTo.x = initCOM.x;//Vec(initCOM.x, 0, 0);
//        fitness += 20;
//    }
//    if ((t < 2500) && (t % 100 == 0)) { // Encourage them to move for 2500, after that, they only get points for eating food.
//                    // Hopefully they will learn to find the first food in 2500s and then associate that neuron approaching 0
//                    // with food. #ClassicalConditioning.
//        fitness += getFitness(com);
//    }
    if (t == 2000-1)
    fitness += getFitness();
    for (auto const& node: this->nodes) {
        node->acceleration = Vec(0, 0, -0.0066);
    }

    std::vector<double> musclePercentages(this->muscles.size());
    std::transform(this->muscles.begin(), this->muscles.end(), musclePercentages.begin(), [](const Piston* muscle) {
        return (euc(muscle->getPosition1(), muscle->getPosition2())/muscle->initialLength - 1.0) / (1.2 - 1.0);
    });

//    const double a = acosh(10) / (this->moveTo.x + 0.3); // This scaling is probably wrong, because of (moveTo.x = 0)
//    const double b = acosh(10) / (this->moveTo.y + 0.3); // find out why +0.1 works, but +2 doesnt.
    const double distanceFactorX = tanh(0.2*(com.x - this->moveTo.x)); //should be +/-(something) depending on direction => tanh
    const double distanceFactorY = tanh(0.2*(com.y - this->moveTo.y));
    musclePercentages.push_back(distanceFactorX);
    musclePercentages.push_back(distanceFactorY);

    std::vector<double> desiredChanges = NN.propagate(musclePercentages);

    /* Get Muscle Forces : Find force needed to get to this frames length */
    for (unsigned int i = 0; i < this->muscles.size(); i++) {
        Piston * muscle = this->muscles[i];
        double currLength    = euc(muscle->getPosition1(), muscle->getPosition2());

        static constexpr double L = acosh(10);
        double stretch = currLength / muscle->initialLength - 1;
        double factor = 1.0 / cosh(L/0.4 * stretch); // maxes at stretch=1, ie best power at normal length
        double desiredLength = muscle->initialLength;
        double newFactor = (sgn<double>(stretch) * sgn<double>(factor) > 0 ? factor : 1); // Reduce movement capability if changing length away from init.
        desiredLength = currLength + 0.05 * desiredChanges[i] * newFactor;
        desiredLength *= exp(-0.01*(stretch)); // Low powered return to original length

//        desiredLength = muscle->initialLength * (1 + 0.2 * sin(0.005 * muscle->speed * (10* ((t) / dt))));
        double deviation     = desiredLength - currLength;

        Vec radVector = (muscle->getPosition1() - muscle->getPosition2());
        Vec deltaAcc  = radVector * (deviation * 0.5) / (radVector.length() * dt * dt);

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
        if (node->position.z < 1.05*node->radius) {
            node->velocity.x *= (1 - node->mass / NodeGene::MAX_MASS * 0.5 * dt) ;// * (isinf(x) || (x>100)? 1 : x / sqrt(1 + x*x));
            node->velocity.y *= (1 - node->mass / NodeGene::MAX_MASS * 0.5 * dt) ;// * (isinf(y) || (y>100)? 1 : y / sqrt(1 + y*y));

        }
        if (node->position.z < 1.01*node->radius) {
            node->position.z = node->radius;
            node->velocity.z *= (1 - (node->mass + 0.5) / (NodeGene::MAX_MASS+0.5) * 0.1 * dt); // +0.5, so no v.z*=0 nodes.
        }

        node->velocity *= 1 - 0.01 * dt;


        /// Apply Kinematics
        node->velocity += node->acceleration * dt;
        node->position += node->velocity * dt;

    }
    return fitness;
}

#include "utility.h"
std::string Creature::getGenomeString() const {
    std::string genome = "<MetaData>";
    for (auto const& node : this->nodes) {
        genome += Gene::toStringFormat(std::vector<std::string>({std::string(1, NodeGene::symbol),
            utility::numToStr<double>(node->position.x),
            utility::numToStr<double>(node->position.y),
            utility::numToStr<double>(node->position.z),
            utility::numToStr<double>(node->mass)
        }));
    }
    for (auto const& muscle : this->muscles) {
        genome += Gene::toStringFormat(std::vector<std::string>({std::string(1, MuscleGene::symbol),
            utility::numToStr<double>(muscle->getIndex1()),
            utility::numToStr<double>(muscle->getIndex2()),
            utility::numToStr<double>(muscle->speed)
        }));
    }
    for (auto const& bone : this->bones) {
        genome += Gene::toStringFormat(std::vector<std::string>({std::string(1, BoneGene::symbol),
            utility::numToStr<double>(bone->getIndex1()),
            utility::numToStr<double>(bone->getIndex2())
        }));
    }
    return genome;
}










