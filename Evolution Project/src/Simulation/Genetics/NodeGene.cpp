#include "Simulation/Genetics/NodeGene.h"

#include "utility.h"
#include <string>
constexpr char NodeGene::symbol;
NodeGene::NodeGene(std::vector<Gene*> genes) : NodeGene(getValidPosition(genes)) {}
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
    return Gene::toStringFormat(this->symbol, std::vector<std::string>({
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
#include "Genome.h"
void NodeGene::mutate(const Genome& genome) {
    relocateNodes(genome.genes.at(NodeGene::symbol), 10);
}

void NodeGene::relocateNodes(std::vector<Gene*> genome, double chance) {
    if (randf(100) < chance) {
        this->position = getValidPosition(genome);
    }
}

Vec NodeGene::getValidPosition(std::vector<Gene*> genes) const {
    while (true) {
        bool validPosition = true; // Not False

        /* Try a new location */
        double x = pmRandf(CAGE_SIZE); //+-CageSize SWITCH TO DOUBLE
        double y = twoD ? 0 : pmRandf(CAGE_SIZE);
        double z =  randf(CAGE_SIZE);

        /* Check if it is in the same location as another */
        for (auto const& gene : genes) {
            if (dynamic_cast<NodeGene*>(gene)) {
                Vec pos = dynamic_cast<NodeGene*>(gene)->position;
                if (fabs(x - pos.x) < MIN_NODE_DISTANCE &&
                    fabs(y - pos.y) < MIN_NODE_DISTANCE &&
                    fabs(z - pos.z) < MIN_NODE_DISTANCE ){
                    validPosition = false;
                }
            }
        }

        /* If valid, return the position */
        if (validPosition) {
            return Vec(x, y, z);
        }
    }
}

