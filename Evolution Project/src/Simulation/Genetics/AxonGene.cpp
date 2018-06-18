#include "AxonGene.h"
#include "Genome.h"
#include "utility.h"

constexpr char AxonGene::symbol;
AxonGene::AxonGene(int a_t, int b_t, int layer_t, double weight_t) : a(a_t), b(b_t), layer(layer_t), weight(weight_t) {}
AxonGene::AxonGene(std::string representation) : AxonGene(0, 0, 0, 0) {
    std::vector<std::string> values = utility::split<std::vector>(representation, ",");
    a = utility::strToNum<double>(values[0]);
    b = utility::strToNum<double>(values[1]);
    layer = utility::strToNum<double>(values[2]);
    weight = utility::strToNum<double>(values[3]);
}
#include <iostream>
AxonGene::AxonGene(const AxonGene& other) : Gene(other), a(other.a), b(other.b), layer(other.layer), weight(other.weight) {}
Gene* AxonGene::clone() const {
    return new AxonGene(*this);
}
AxonGene::~AxonGene() {}


std::string AxonGene::toString() const {
    return Gene::toString() + Gene::toStringFormat(std::vector<std::string>({
        std::string(1, this->symbol),
        utility::numToStr<int>(a),
        utility::numToStr<int>(b),
        utility::numToStr<int>(layer),
        utility::numToStr<double>(weight)
    }));
}

#include "MyMath.h"
void AxonGene::mutate(Genome& genome) {
    if (randf(100) < 10) {
        weight = pmRandf(1);
    }
    if (genome.fitness > 0) return;
}

