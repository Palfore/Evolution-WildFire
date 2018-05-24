#ifndef NODEGENE_H
#define NODEGENE_H

#include "Simulation/Genetics/Gene.h"
#include "Math/myMath.h"

class NodeGene : public Gene {
    private:
        static constexpr double CAGE_SIZE = 30;
        static constexpr bool twoD = false;
        static constexpr double MIN_NODE_DISTANCE = 1.5;
    public:
        static constexpr char symbol = 'n';

        Vec position;
        double mass;

        NodeGene(std::string representation);
        NodeGene(const Genome& genes);
        NodeGene(Vec position);
        NodeGene(Vec position, double mass);
        NodeGene(const NodeGene& other);
        ~NodeGene();

        virtual Gene* clone() const;

        virtual std::string toString() const override;
        void mutate(const Genome& genome);

    private:
        void relocateNodes(const Genome& genome, double chance);
        Vec getValidPosition(const Genome& genome) const;


};

#endif // NODEGENE_H
