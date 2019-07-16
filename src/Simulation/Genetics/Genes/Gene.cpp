#include "Gene.h"


Gene::Gene(const Gene& other) {
    if (other.symbol == 'q') return; // warning supression
}
Gene::Gene() {}
Gene::~Gene() {}

std::string Gene::toStringFormat(std::vector<std::string> values) {
    std::string representation = "";
    for (auto const& value : values) {
        representation += value + ",";
    }
    return representation.substr(0, representation.size()-1) + "";
}

std::string Gene::toString() const {
    return "|";
}
