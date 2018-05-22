#include "MuscleGene.h"
#include "NodeGene.h"
#include "Genome.h"
#include "MyMath.h"
#include "Logger.h"
#include "utility.h"
constexpr char MuscleGene::symbol;
MuscleGene::MuscleGene(int numNodes, std::vector<Gene*> muscleGenes): connection(getGoodConnection(numNodes, muscleGenes)) {}
MuscleGene::MuscleGene(int a, int b): MuscleGene(Connection(a, b)){}
MuscleGene::MuscleGene(Connection c): connection(c) {}
MuscleGene::MuscleGene(const MuscleGene& other) : Gene(other), connection(other.connection){}

MuscleGene::MuscleGene(std::string representation) : connection(-1, -1) {
    std::vector<std::string> values = utility::split<std::vector>(representation, ",");
    this->connection = Connection(utility::strToNum<int>(values[0]),
                                    utility::strToNum<int>(values[1]));
}

Gene* MuscleGene::clone() const {
    return new MuscleGene(*this);
}
MuscleGene::~MuscleGene() {

}

std::string MuscleGene::toString() const {
    return Gene::toStringFormat(this->symbol, std::vector<std::string>({
        utility::numToStr<int>(connection.a),
        utility::numToStr<int>(connection.b),
    }));
}

void MuscleGene::mutate(const Genome& genome) {
//    if (genome.empty()) return;
}

Connection MuscleGene::getGoodConnection(int numNodes, std::vector<Gene*> muscleGenes) const {
    // This doesnt check that there IS a valid connection (ie infinite loop)
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

                Connection c(static_cast<MuscleGene*>(gene)->connection.a, static_cast<MuscleGene*>(gene)->connection.b);
                if ((c.a == a && c.b == b) ||
                    (c.b == a && c.a == b) ){
                    validConn = false;
                    continue;
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

