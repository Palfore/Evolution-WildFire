#include "Genome.h"

#include "GeneMapping.h"
#include "Logger.h"
#include "utility.h"

#include <deque>

Genome::Genome() : fitness(0), genes({}) {
    for (auto const& [geneSymbol, _] : getGeneMap()) {
        this->genes.insert({geneSymbol, {}});
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

Genome::Genome(const std::string genomeString) : Genome() {
    // puts("okay");
    std::deque<std::string> geneStrings = utility::split<std::deque>(genomeString, GENE_DELIMITER);
    std::string metaData = geneStrings[0];
    geneStrings.pop_front();
    // puts("1");
    for (auto const& gene : geneStrings) {
        // puts("2");
        std::vector<std::string> headAndValues = utility::split<std::vector>(gene, VALUE_DELIMITER, 1);
        // puts("3");
        char head = headAndValues[0][0];
        // puts("4");
        // std::cout << head<<','<<getGeneMap().size()<<'\n';
        auto geneGenerator = getGeneMap().find(head)->second;
        // puts("5");
        this->genes[head].push_back( geneGenerator(headAndValues[1]) );
    }
    // puts("done");
}

Genome::~Genome() {
    for (auto geneTypes : this->genes) {
        for (Gene* gene : geneTypes.second) {
            delete gene;
        }
    }
}

void Genome::addGene(Gene* gene) {
    this->genes[gene->getSymbol()].push_back(gene);
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
    for (auto * gene: this->getGenes<AxonGene>()) {
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










