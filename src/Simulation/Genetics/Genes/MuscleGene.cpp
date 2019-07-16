#include "MuscleGene.h"
#include "NodeGene.h"
#include "Genome.h"
#include "MyMath.h"
#include "Logger.h"
#include "utility.h"
#include "BoneGene.h"
constexpr char MuscleGene::symbol;
MuscleGene::MuscleGene(int numNodes, const Genome& genome): BoneGene(numNodes, genome), speed(getRandomSpeed()) {}
MuscleGene::MuscleGene(int a, int b, double s): MuscleGene(Connection(a, b), s) {}
MuscleGene::MuscleGene(Connection c, double s): BoneGene(c), speed(s) {}
MuscleGene::MuscleGene(const MuscleGene& other) : BoneGene(other), speed(other.speed) {}

MuscleGene::MuscleGene(std::string representation) : BoneGene(-2, -2), speed(-1) {
    std::vector<std::string> values = utility::split<std::vector>(representation, ",");
    this->connection = Connection(utility::strToNum<int>(values[0]),
                                    utility::strToNum<int>(values[1]));
    this->speed = utility::strToNum<double>(values[2]);
}

Gene* MuscleGene::clone() const {
    return new MuscleGene(*this);
}

MuscleGene::~MuscleGene() {

}

std::string MuscleGene::toString() const {
    return Gene::toString() + Gene::toStringFormat(std::vector<std::string>({
        std::string(1, this->getSymbol()),
        utility::numToStr<int>(this->connection.a),
        utility::numToStr<int>(this->connection.b),
        utility::numToStr<double>(this->speed)
    }));
}

void MuscleGene::mutate(Genome& genome) {
    if (genome.fitness < 0) return;
}

double MuscleGene::getRandomSpeed() {
    return randf(1.0);
}




