#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include <unordered_map>
#include <string>
#include <math.h>
#include <limits>

#include <iostream>
#include <algorithm>

class Gene;
class Genome {
    public:
        Genome(int n, int m, int b, std::vector<unsigned int> N);
        Genome(const Genome &obj);
        Genome(std::string genomeString);
        Genome& operator=(Genome other);
        virtual ~Genome();

        double fitness;

        virtual std::string toString() const;
        void mutate();

        template<class GeneType>
        std::vector<GeneType*> getGenes() const {
            std::vector<Gene*> untypedGenes;
            if (this->genes.count(GeneType::symbol) > 0) {
                untypedGenes = this->genes.at(GeneType::symbol);
            } else {
                return {};
            }
            std::vector<GeneType*> typedGenes;
            typedGenes.reserve(untypedGenes.size());
            for (const auto& g: untypedGenes) {
                typedGenes.push_back(static_cast<GeneType*>(g));
            }
            return typedGenes;
        }
    private:
        Genome();
        std::unordered_map<char, std::vector<Gene*>> genes;
        static constexpr char VALUE_DELIMITER = ',';
        static constexpr char GENE_DELIMITER = '|';

        /// Mutations
        void addNodes(double chance);
        void removeNodes(double chance);
};

#endif // GENOME_H
