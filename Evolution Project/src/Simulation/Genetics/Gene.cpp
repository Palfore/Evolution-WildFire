#include "Simulation/Genetics/Gene.h"


Gene::Gene(const Gene& other) {
    if (other.symbol == 'q') return; // warning supression
}

Gene* Gene::clone() const {
    return new Gene(*this);
}


Gene::Gene() {
    //ctor
}

Gene::~Gene() {
    //dtor
}

std::string Gene::toStringFormat(char geneSymbol, std::vector<std::string> values) const {
    std::string representation = "|" + std::string(1, geneSymbol) + ":";
    for (auto const& value : values) {
        representation += value + ",";
    }
    return representation.substr(0, representation.size()-1);
}

std::string Gene::toString() const {
    return toStringFormat(this->symbol, {});
}

void Gene::mutate(const Genome& genome) {
//    if (genome.empty()) return;
}
