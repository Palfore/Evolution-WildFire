#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include <unordered_map>
#include <string>
#include <math.h>
#include <limits>

#include <iostream>
class Gene;
class Genome {
    public:
        Genome();
        Genome(int n, int m);
        Genome(const Genome &obj);
        Genome(std::string genomeString);
        Genome& operator=(Genome other);
        virtual ~Genome();

        double fitness;

        virtual std::string toString() const;

        template<class GeneType>
        std::vector<Gene*> getGenes() const {
            return this->genes.at(GeneType::symbol);
        }

        std::unordered_map<char, std::vector<Gene*>> genes;
};

#endif // GENOME_H
