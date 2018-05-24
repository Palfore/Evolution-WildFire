#include "BoneGene.h"
#include "Genome.h"
#include "Logger.h"
#include "utility.h"
#include "MuscleGene.h"

constexpr char BoneGene::symbol;
BoneGene::BoneGene(int numNodes, const Genome& genome): connection(getGoodConnection(numNodes, genome)) {}
BoneGene::BoneGene(int a, int b): BoneGene(Connection(a, b)) {}
BoneGene::BoneGene(Connection c): connection(c) {}
BoneGene::BoneGene(const BoneGene& other) : Gene(other), connection(other.connection) {}

BoneGene::BoneGene(std::string representation) : connection(-1, -1){
    std::vector<std::string> values = utility::split<std::vector>(representation, ",");
    this->connection = Connection(utility::strToNum<int>(values[0]),
                                    utility::strToNum<int>(values[1]));
}

BoneGene::~BoneGene() {
    //dtor
}

Gene* BoneGene::clone() const {
    return new BoneGene(*this);
}


std::string BoneGene::toString() const {
    return Gene::toString() + Gene::toStringFormat(std::vector<std::string>({
        std::string(1, this->symbol),
        utility::numToStr<int>(this->connection.a),
        utility::numToStr<int>(this->connection.b)
    }));
}

void BoneGene::mutate(const Genome& genome) {
    if (genome.fitness < 0) return;
}

Connection BoneGene::getGoodConnection(int numNodes, const Genome& genome) {
    // This doesnt check that there IS a valid connection (ie infinite loop)
    std::vector<BoneGene*> boneGenes = genome.getGenes<BoneGene>();
    std::vector<MuscleGene*> muscleGenes = genome.getGenes<MuscleGene>();
    while (true) {
        bool validConn = true;

         /* Try a new connection */
        int a = rand() % numNodes;
        int b = rand() % numNodes;

        if (a == b) {
            validConn = false;
            continue;
        }

        /* Check if this connection exist */
        for (auto const& gene : muscleGenes) {
            Connection c(gene->connection.a, gene->connection.b);
            if ((c.a == a && c.b == b) ||
                (c.b == a && c.a == b) ){
                validConn = false;
            }
        }
        for (auto const& gene : boneGenes) {
            Connection c(gene->connection.a, gene->connection.b);
            if ((c.a == a && c.b == b) ||
                (c.b == a && c.a == b) ){
                validConn = false;
            }
        }

        /* Valid Connection Found */
        if (validConn) {
            if (a == b) {
                LOG("Invalid connections deemed valid.", LogDegree::FATAL, LogType::GENETIC);
            }
            Connection validCon(a, b);
            return validCon;
        }
    }
}
