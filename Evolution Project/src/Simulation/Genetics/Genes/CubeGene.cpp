#include "CubeGene.h"
#include "Genome.h"
#include "MyMath.h"
#include "utility.h"
constexpr char CubeGene::symbol;
CubeGene::CubeGene(Vec pos, double l): position(pos), length(l) {}
CubeGene::CubeGene(const CubeGene& other) : CubeGene(other.position, other.length) {}
CubeGene::CubeGene(std::string representation) : position(Vec(0, 0, 0)), length(1) {
    const auto values = utility::split<std::vector>(representation, ",");
    this->position = Vec(utility::strToNum<double>(values[0]),
                   utility::strToNum<double>(values[1]),
                   utility::strToNum<double>(values[2]));
    this->length = utility::strToNum<double>(values[3]);
}
CubeGene::~CubeGene() {}

Gene* CubeGene::clone() const {
    return new CubeGene(*this);
}

std::string CubeGene::toString() const {
    return Gene::toString() + Gene::toStringFormat(std::vector<std::string>({
        std::string(1, this->getSymbol()),
        utility::numToStr<double>(position.x),
        utility::numToStr<double>(position.y),
        utility::numToStr<double>(position.z),
        utility::numToStr<double>(length)
    }));
}

void CubeGene::mutate(Genome& genome) {
    if (genome.fitness < 0) return;
}




