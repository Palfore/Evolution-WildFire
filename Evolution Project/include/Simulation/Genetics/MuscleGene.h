#ifndef MUSCLEGENE_H
#define MUSCLEGENE_H

#include "Genetics/Gene.h"
#include <string>

struct Connection {
    int a;
    int b;
    Connection(int a_t, int b_t): a(a_t), b(b_t){};
    Connection(): a(-1), b(-1){};
    Connection(const Connection &c): a(c.a), b(c.b){};
};

//// This is the new one
class Genome;
class MuscleGene : public Gene {
    public:
        static constexpr char symbol = 'm';
        Connection connection;

        MuscleGene(std::string representation);
        MuscleGene(int numNodes, std::vector<Gene*> muscleGenes);
        MuscleGene(int a, int b);
        MuscleGene(Connection c);
        MuscleGene(const MuscleGene& other);
        virtual ~MuscleGene();

        virtual Gene* clone() const;

        void mutate(const Genome& genome);
        std::string toString() const override;

    private:
        Connection getGoodConnection(int numNodes, std::vector<Gene*> muscleGenes) const;
};

#endif // MUSCLEGENE_H
