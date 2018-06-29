#include "Genome.h"

#include "NodeGene.h"
#include "MuscleGene.h"
#include "BoneGene.h"
#include "AxonGene.h" //NewGeneEditHere
#include "Logger.h"
#include "utility.h"

#include <deque>

Genome::Genome() : fitness(0), genes({{NodeGene::symbol, {}}, {MuscleGene::symbol, {}}, {BoneGene::symbol, {}}, {AxonGene::symbol, {}}}) {} //NewGeneEditHere
Genome::Genome(int n, int m, int b, std::vector<unsigned int> sizes) : Genome() {
    if (m + b > comb(n)) LOG("Too many connections for valid creature", LogDegree::FATAL, LogType::GENETIC);
    if (n < 0) LOG("Creature created with 0 nodes.", LogDegree::FATAL, LogType::GENETIC);

    for (int i = 0; i < n; i++) {
        this->genes[NodeGene::symbol].push_back(new NodeGene(*this));
    }
    for (int i = 0; i < m; i++) {
        this->genes[MuscleGene::symbol].push_back(new MuscleGene(this->genes[NodeGene::symbol].size(), *this));
    }
    for (int i = 0; i < b; i++) {
        this->genes[BoneGene::symbol].push_back(new BoneGene(this->genes[NodeGene::symbol].size(), *this));
    }

    sizes.insert(sizes.begin(), m + 2);
    sizes.push_back(m);
    for (unsigned int layer = 0; layer < sizes.size() - 1; layer++) {
        for (unsigned int i = 0; i < sizes[layer]; i++) {
            for (unsigned int j = 0; j < sizes[layer+1]; j++) {
                this->genes[AxonGene::symbol].push_back(new AxonGene(i, j, layer, pmRandf(1)));
            }
        }
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



Genome::Genome(std::string genomeString) : Genome() { //NewGeneEditHere
    std::deque<std::string> geneStrings = utility::split<std::deque>(genomeString, GENE_DELIMITER);
    std::string metaData = geneStrings[0];
    geneStrings.pop_front();

    for (auto const& gene : geneStrings) {
        std::vector<std::string> headAndValues = utility::split<std::vector>(gene, VALUE_DELIMITER, 1);
        char head = headAndValues[0][0];
        switch(head) {
            case NodeGene::symbol:
                this->genes[NodeGene::symbol].push_back(new NodeGene(headAndValues[1]));
                break;
            case MuscleGene::symbol:
                this->genes[MuscleGene::symbol].push_back(new MuscleGene(headAndValues[1]));
                break;
            case BoneGene::symbol:
                this->genes[BoneGene::symbol].push_back(new BoneGene(headAndValues[1]));
                break;
            case AxonGene::symbol:
                this->genes[AxonGene::symbol].push_back(new AxonGene(headAndValues[1]));
                break;
            default:
                LOG("Genome(std::string) parsed invalid string:(" + std::string(1, headAndValues[0][0]) + ")", LogDegree::FATAL, LogType::GENETIC);
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



void Genome::mutate() {
//    addNodes(100);
//    removeNodes(0);

    for (auto * gene: this->getGenes<NodeGene>()) {
        gene->mutate(*this);
    }
}

void Genome::addNodes(double chance) { ///< @todo: Add connections to new nodes
    if (randf(100) < chance) {
        this->genes[NodeGene::symbol].push_back(new NodeGene(*this));

    }
}

void Genome::removeNodes(double chance) { ///< @todo: remove connection from removed nodes
    int len = this->getGenes<NodeGene>().size();
    if (len == 1) return; // Can't remove last node

    if (randf(100) < chance) {
        int removeIndex = randi(len-1);

        /// Remove Node
        this->genes[NodeGene::symbol].erase(this->genes[NodeGene::symbol].begin() + removeIndex);

        /// Remove Detached Connections
        auto muscleGenes = &this->genes[MuscleGene::symbol];
        auto boneGenes = &this->genes[BoneGene::symbol];
        for (unsigned int i = 0; i < muscleGenes->size(); i++) {
            auto m = static_cast<MuscleGene*>((*muscleGenes)[i]);
            if ((m->connection.a == removeIndex) || (m->connection.b == removeIndex)) {
                delete (*muscleGenes)[i];
                muscleGenes->erase(muscleGenes->begin()+i--);
            }
        }
        for (unsigned int i = 0; i < boneGenes->size(); i++) {
            auto b = static_cast<BoneGene*>((*boneGenes)[i]);
            if ((b->connection.a == removeIndex) || (b->connection.b == removeIndex)) {
                delete (*boneGenes)[i];
                boneGenes->erase(boneGenes->begin()+i--);
            }
        }

        /// Adjust Connections
        for (auto* g: *muscleGenes) {
            auto m = static_cast<MuscleGene*>(g);
            if (m->connection.a >= removeIndex) m->connection.a--;
            if (m->connection.b >= removeIndex) m->connection.b--;
        }
        for (auto* g: *boneGenes) {
            auto b = static_cast<BoneGene*>(g);
            if (b->connection.a >= removeIndex) b->connection.a--;
            if (b->connection.b >= removeIndex) b->connection.b--;
        }
    }
}










