#include "StickBall.h"
#include "utility.h"
#include "Genome.h"
#include "NodeGene.h"
#include "MuscleGene.h"
#include "BoneGene.h"
#include "AxonGene.h"
#include "Logger.h"

#include "Ball.h"
#include "Piston.h"
#include "Shapes.h"

#include <limits>


static Vec* get_head_location(const std::vector<Ball*>& nodes) {
    Vec* head = &nodes[0]->position;
    double minZ = -std::numeric_limits<double>::max();
    for (const auto& node: nodes) {
        double nodeZ = node->position.z;
        head = nodeZ > minZ ? &node->position : head;
        minZ = nodeZ > minZ ? nodeZ : minZ;
    }
    return head;
}

Genome* StickBall::createGenome(int n, int m, int b, std::vector<unsigned int> sizes) {
    if (m + b > comb(n)) LOG("Too many connections for valid creature", LogDegree::FATAL, LogType::GENETIC);
    if (n < 0) LOG("Creature created with 0 nodes.", LogDegree::FATAL, LogType::GENETIC);

    Genome *g = new Genome();
    for (int i = 0; i < n; i++) {
        g->addGene(new NodeGene(*g));
    }
    for (int i = 0; i < m; i++) {
        g->addGene(new MuscleGene(n, *g));
    }
    for (int i = 0; i < b; i++) {
        g->addGene(new BoneGene(n, *g));
    }

    sizes.insert(sizes.begin(), m + 2);
    sizes.push_back(m);
    for (unsigned int layer = 0; layer < sizes.size() - 1; layer++) {
        for (unsigned int i = 0; i < sizes[layer]; i++) {
            for (unsigned int j = 0; j < sizes[layer+1]; j++) {
                g->addGene(new AxonGene(i, j, layer, 0, pmRandf(1)));
            }
        }
    }
    return g;
}


StickBall::StickBall(const Genome& genome) : Creature(genome), head(nullptr), nodes({}), muscles({}), bones({}), NN(0, genome.getGenes<AxonGene>()) {
    for (auto const& gene: genome.getGenes<NodeGene>()) {
        this->nodes.push_back(new Ball(*gene));
    }
    for (auto const& gene: genome.getGenes<MuscleGene>()) {
        this->muscles.push_back(new Piston(*gene, this->nodes));
    }
    for (auto const& gene: genome.getGenes<BoneGene>()) {
        this->bones.push_back(new Piston(*static_cast<MuscleGene*>(gene), this->nodes));
    }

    this->head = get_head_location(this->nodes);
    lowerToGround();
    centerCOM();
    initCOM = getCOM();
}


StickBall::~StickBall() {
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


StickBall::StickBall(const StickBall &other) : Creature(other), head(nullptr), nodes({}), muscles({}), bones({}), NN(other.NN) {
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
    this->head = get_head_location(this->nodes);
}


void StickBall::draw(const Scenario*) const {
    DrawSphere<Appearance::FACE>(*head + Vec(0,0,4), 5,
        180.0/3.1415926*atan2(moveTo.y - getCOM().y, moveTo.x - getCOM().x) + 90
    );
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

void StickBall::drawBrain(const bool drawLines) const {
    this->NN.draw(drawLines);
}


Vec StickBall::calculateCOM() const {
    Vec COM = Vec(0,0,0);
    double mass = 0.0;
    for (auto const& node : this->nodes) {
        COM += node->position * node->mass;
        mass += node->mass;
    }
    if (mass < 0.000000001) return Vec(0,0,0);
    return COM / mass;
}


void StickBall::moveCOMTo(Vec to) {
    Vec COM = getCOM();
    Vec dr = to - COM;
    for (auto * node : this->nodes) {
        node->position += dr;
    }
}

void StickBall::lowerToGround() {
    double dz = this->getLowestBodyHeight() - 1; //-1 for not embeding creatures/padding
    for (auto * node : this->nodes) {
        node->position.z -= dz;
    }
}

double StickBall::getLowestBodyHeight() const {
    double minHeight = std::numeric_limits<double>::max();
    for (auto const& node : this->nodes) {
        double lowestPart = node->position.z - node->radius;
        minHeight = lowestPart < minHeight  ? lowestPart : minHeight ;
    }
    return minHeight;
}

Vec StickBall::getTop(const double offset=0) const {
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
    return Vec(comX / mass, comY / mass, highestNode + offset);
}

void StickBall::update(Scenario*, int t) {
    return;
    const double dt = 1.0;
    // this->prevCOM = this->com;

    for (auto const& node: this->nodes) {
        node->acceleration = Vec(0, 0, -0.0066);
    }


    std::vector<double> inputs(this->muscles.size());
//    std::transform(this->nodes.begin(), this->nodes.end(), inputs.begin(), [this](const Ball* ball) {
//        return tanh(0.01*(ball->position.x - moveTo.x));
//    });
//    std::transform(this->nodes.begin(), this->nodes.end(), inputs.begin() + this->nodes.size(), [this](const Ball* ball) {
//        return tanh(0.01*((ball->position.y - moveTo.y) / 6 - 1));
//    });
//    std::transform(this->nodes.begin(), this->nodes.end(), inputs.begin() + 2*this->nodes.size(), [this](const Ball* ball) {
//        return tanh(0.01*((ball->position.z - moveTo.z) / 6 - 1));
//    });

    std::vector<double> musclePercentages(this->muscles.size());
    std::transform(this->muscles.begin(), this->muscles.end(), inputs.begin(), [](const Piston* muscle) {
        return (euc(muscle->getPosition1(), muscle->getPosition2()) / muscle->initialLength - 1.0) / (1.2 - 1.0);
    });

//    const double a = acosh(10) / (this->moveTo.x + 0.3); // This scaling is probably wrong, because of (moveTo.x = 0)
//    const double b = acosh(10) / (this->moveTo.y + 0.3); // find out why +0.1 works, but +2 doesnt.
    const double distanceFactorX = tanh(0.01*(getCOM().x - this->moveTo.x)); //should be +/-(something) depending on direction => tanh
    const double distanceFactorY = tanh(0.01*(getCOM().y - this->moveTo.y));
    inputs.push_back(distanceFactorX);
    inputs.push_back(distanceFactorY);

    std::vector<double> desiredChanges = NN.propagate(inputs);


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
        if (node->position.z < 1.03*node->radius) { // convert to exponential decay
            node->velocity.x *= (1 - (node->mass + 0.5) / NodeGene::MAX_MASS * 0.75 * dt) ;// * (isinf(x) || (x>100)? 1 : x / sqrt(1 + x*x));
            node->velocity.y *= (1 - (node->mass + 0.5) / NodeGene::MAX_MASS * 0.75 * dt) ;// * (isinf(y) || (y>100)? 1 : y / sqrt(1 + y*y));

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

    // this->moveCOMTo(Vec(com.x, 0, com.z)); // Keep it two-d (confined to the x-z plane).
    // this->com = getCOM();
    if (t > 0) return;
}


std::string StickBall::getGenomeString() const {
    std::string genomeString = Creature::getGenomeString();
    for (auto const& node : this->nodes) {
        genomeString += Gene::toStringFormat(std::vector<std::string>({std::string(1, NodeGene::symbol),
            utility::numToStr<double>(node->position.x),
            utility::numToStr<double>(node->position.y),
            utility::numToStr<double>(node->position.z),
            utility::numToStr<double>(node->mass)
        }));
    }
    for (auto const& muscle : this->muscles) {
        genomeString += Gene::toStringFormat(std::vector<std::string>({std::string(1, MuscleGene::symbol),
            utility::numToStr<double>(muscle->getIndex1()),
            utility::numToStr<double>(muscle->getIndex2()),
            utility::numToStr<double>(muscle->speed)
        }));
    }
    for (auto const& bone : this->bones) {
        genomeString += Gene::toStringFormat(std::vector<std::string>({std::string(1, BoneGene::symbol),
            utility::numToStr<double>(bone->getIndex1()),
            utility::numToStr<double>(bone->getIndex2())
        }));
    }
    return genomeString;
}
