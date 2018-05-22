#include "Genome.h"

#include "NodeGene.h"
#include "MuscleGene.h"
#include "Logger.h"
#include "utility.h"

#include <deque>

Genome::Genome() : fitness(0), genes({{NodeGene::symbol, {}}, {MuscleGene::symbol, {}}}) {}
Genome::Genome(int n, int m) : Genome() {
    if (m /*+b*/ > comb(n)) LOG("Too many connections for valid creature", LogDegree::FATAL, LogType::GENETIC);

    for (int i = 0; i < n; i++) {
        this->genes.at(NodeGene::symbol).push_back(new NodeGene(this->genes[NodeGene::symbol]));
    }
    for (int i = 0; i < m; i++) {
        this->genes[MuscleGene::symbol].push_back(new MuscleGene(this->genes[NodeGene::symbol].size(), this->genes[MuscleGene::symbol]));
    }
}

Genome::Genome(const Genome &obj) : Genome() {
    for (auto const& geneTypePair: obj.genes) {
        this->genes.insert({geneTypePair.first, {}});
        for (auto const& gene: geneTypePair.second) {
            this->genes.at(geneTypePair.first).push_back(gene->clone());
        }
    }
}

Genome& Genome::operator=(Genome other) {
    std::swap(genes, other.genes);
    return *this;
}



Genome::Genome(std::string genomeString) : Genome() {
    std::deque<std::string> geneStrings = utility::split<std::deque>(genomeString, "|");
    std::string metaData = geneStrings[0];
    geneStrings.pop_front();

    for (auto const& gene : geneStrings) {
        std::vector<std::string> headAndValues= utility::split<std::vector>(gene, ":");
        char head = headAndValues[0][0];

        switch(head) {
        case NodeGene::symbol:
            this->genes[NodeGene::symbol].push_back(new NodeGene(headAndValues[1]));
            break;
        case MuscleGene::symbol:
            this->genes[MuscleGene::symbol].push_back(new MuscleGene(headAndValues[1]));
            break;
        default:
            LOG("Genome(std::string) parsed invalid string.", LogDegree::FATAL, LogType::GENETIC);
        }
    }

}

Genome::~Genome() {
    for (auto geneTypes : this->genes) {
        for (Gene* gene : geneTypes.second) {
            delete gene;
        }
    }
}




std::string Genome::toString() const {
    std::string representation = "<MetaData>"; // Generation, Fitness

    for (auto const& geneType: this->genes) {
        for (auto const& gene: geneType.second) {
            representation += gene->toString();
        }
    }
    return representation;
}












