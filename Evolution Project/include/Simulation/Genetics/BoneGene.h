#ifndef BONEGENE_H
#define BONEGENE_H

#include <string>
#include "Gene.h"
#include "Connection.h"

class BoneGene : public Gene {
    public:
        static constexpr char symbol = 'b';
        Connection connection;

        BoneGene(std::string representation);
        BoneGene(int numNodes, const Genome& genome);
        BoneGene(int a, int b);
        BoneGene(Connection c);
        BoneGene(const BoneGene& other);
        virtual ~BoneGene();

        virtual Gene* clone() const;

        static double getRandomSpeed();
        void mutate(Genome& genome);
        std::string toString() const override;

    protected:
        static Connection getGoodConnection(int numNodes, const Genome& genome);

};

#endif // BONEGENE_H
