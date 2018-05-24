#include "Simulation/Genetics/Gene.h"


Gene::Gene(const Gene& other) {
    if (other.symbol == 'q') return; // warning supression
}

Gene::Gene() {
    //ctor
}

Gene::~Gene() {
    //dtor
}

#include "stdio.h"
std::string Gene::toStringFormat(std::vector<std::string> values) const {
    std::string representation = "";
    for (auto const& value : values) {
        representation += value + ",";
    }
    return representation.substr(0, representation.size()-1) + "";
}

std::string Gene::toString() const {
    return "|";
}
