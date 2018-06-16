#include "Simulation/Genetics/NodeGene.h"

#include "utility.h" // numToStr
#include <string> // std::string
#include "Genome.h" // Genome

constexpr char NodeGene::symbol;
NodeGene::NodeGene(const Genome& genes) : NodeGene(getValidPosition(genes)) {}
NodeGene::NodeGene(Vec position_t) : NodeGene(position_t, 1) {} // randf(4) + 1
NodeGene::NodeGene(Vec position_t, double mass_t) : position(position_t), mass(mass_t) {}
NodeGene::NodeGene(std::string representation) : position(Vec(0,0,0)), mass(1) {
    std::vector<std::string> values = utility::split<std::vector>(representation, ",");
    position = Vec(utility::strToNum<double>(values[0]),
                   utility::strToNum<double>(values[1]),
                   utility::strToNum<double>(values[2]));
    mass = utility::strToNum<double>(values[3]);
}

std::string NodeGene::toString() const {
    return Gene::toString() +
    Gene::toStringFormat(std::vector<std::string>({
        std::string(1, this->symbol),
        utility::numToStr<double>(position.x),
        utility::numToStr<double>(position.y),
        utility::numToStr<double>(position.z),
        utility::numToStr<double>(mass)
    }));
}

NodeGene::~NodeGene() {

}

NodeGene::NodeGene(const NodeGene& other) : Gene(other), position(other.position), mass(other.mass) {
}

Gene* NodeGene::clone() const {
    return new NodeGene(*this);
}

void NodeGene::mutate(Genome& genome) {
    int c = 10;
    suffleNodes(genome, c);
    shiftNodes(genome, c);
    suffleStats(genome, c);
    shiftStats(genome, c);
}

void NodeGene::suffleNodes(const Genome& genome, double chance) {
    if (randf(100) < chance) {
        this->position = getValidPosition(genome);
    }
}

void NodeGene::shiftNodes(const Genome& genome, double chance) {
    if (randf(100) < chance) {
        this->position = getValidShift(genome, 1);
    }
}

void NodeGene::suffleStats(const Genome& genome, double chance) {
    if (randf(100) < chance) {
        this->mass = getValidMass();
    }
    if (genome.fitness > 0) return;
}
void NodeGene::shiftStats(const Genome& genome, double chance) {
    if (randf(100) < chance) {
        this->mass = this->getValidMassShift(this->mass, 0.5);
    }
    if (genome.fitness > 0) return;
}

Vec NodeGene::getValidPosition(const Genome& genes) {
    for (int numAttempts = 0; numAttempts < 1000; numAttempts++) {
        bool validPosition = true; // Not False

        /* Try a new location */
        Vec newPos = Vec(pmRandf(CAGE_SIZE),
                         NodeGene::twoD ? 0 : pmRandf(CAGE_SIZE),
                         randf(CAGE_SIZE));

        /* Check if it is in the same location as another */
        for (auto const& gene : genes.getGenes<NodeGene>()) { // Allowed to avoid self. No self check.
            if (euc(newPos, gene->position) < MIN_NODE_DISTANCE) {
                validPosition = false;
                break;
            }
        }

        /* If valid, return the position */
        if (validPosition) {
            return newPos;
        }
    }
    LOG("Could not find node location.", LogDegree::FATAL, LogType::GENETIC);
    return Vec(0,0,0);
}

Vec NodeGene::getValidShift(const Genome& genome, const NodeGene& node, double amount) {
    for (int numAttempts = 0; numAttempts < 50; numAttempts++) {
        bool validPosition = true; // Not False

        /* Try a new location */
        Vec newPos = node.position + Vec(pmRandf(amount),
                                      NodeGene::twoD ? 0 : pmRandf(amount),
                                      pmRandf(amount));

        for (auto const& gene : genome.getGenes<NodeGene>()) {
            if (&node == gene) continue; // Skip the same node

            /* Check if it is in the same location as another */
            if (euc(node.position, gene->position) < MIN_NODE_DISTANCE) {
                validPosition = false;
                break;
            }
            /* Or outside the bounds */
            if (fabs(newPos.x) > CAGE_SIZE || // Could clamp instead
                fabs(newPos.y) > CAGE_SIZE ||
                      newPos.z > CAGE_SIZE ||
                      newPos.z < 1 ){ // 1 is radius
                validPosition = false;
                break;
             }
        }
        if (validPosition) {
            return newPos;
        }
    }
    /* Couldn't find a valid position */
    return node.position;
}


Vec NodeGene::getValidShift(const Genome& genome, double amount) const {
    return getValidShift(genome, *this, amount);
}

double NodeGene::getValidMass() {
    return randf(MIN_MASS, MAX_MASS);
}

double NodeGene::getValidMassShift(double currentMass, double amount) {
    if (amount < 0.001) return currentMass;
    while (true) {
        double newMass = currentMass + pmRandf(amount);
        if (newMass < MIN_MASS) {
            continue;
        }
        if (newMass > MAX_MASS) {
            continue;
        }
        return newMass;
    }
}

double NodeGene::getValidMassShift(double amount) const {
    return getValidMassShift(this->mass, amount);
}

















