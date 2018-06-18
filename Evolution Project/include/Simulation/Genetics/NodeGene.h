#ifndef NODEGENE_H
#define NODEGENE_H

#include "Simulation/Genetics/Gene.h" // Gene
#include "Math/Vec.h" // Vec

class NodeGene : public Gene {
    private:
        static constexpr double CAGE_SIZE = 50;
        static constexpr double MIN_NODE_DISTANCE = 10;
        static constexpr bool twoD = true;
    public:
        static constexpr char symbol = 'n';
        static constexpr double MAX_MASS = 5.0;
        static constexpr double MIN_MASS = 0.1;

        Vec position;
        double mass;

        NodeGene(std::string representation);
        NodeGene(const Genome& genes);
        NodeGene(Vec position);
        NodeGene(Vec position, double mass);
        NodeGene(const NodeGene& other);
        virtual ~NodeGene();

        virtual Gene* clone() const;
        virtual std::string toString() const override;
        void mutate(Genome& genome);

    private:
        /// Mutations
        void suffleNodes(const Genome& genome, double chance);
        void shiftNodes(const Genome& genome, double chance);
        void addNodes(Genome& genome, double chance);
        void removeNodes(Genome& genome, double chance);
        void suffleStats(const Genome& genome, double chance);
        void shiftStats(const Genome& genome, double chance);

        /// Getting Valid Parameters
        static Vec getValidPosition(const Genome& genome);
        static Vec getValidShift(const Genome& genome, const NodeGene& node, double amount);
        Vec getValidShift(const Genome& genome, double amount) const;
        static double getValidMass();
        static double getValidMassShift(double currentMass, double amount);
        double getValidMassShift(double amount) const;
};

#endif // NODEGENE_H

















