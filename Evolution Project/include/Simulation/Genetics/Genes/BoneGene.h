#ifndef BONEGENE_H
#define BONEGENE_H

#include <string>
#include "Gene.h"
#include "Connection.h"

class BoneGene : public Gene {
    public:
        static constexpr char symbol = 'b';
        virtual char getSymbol() const override {return symbol;};
        Connection connection;

        BoneGene(const std::string representation);
        BoneGene(const int numNodes, const Genome& genome);
        BoneGene(const int a, const int b);
        BoneGene(const Connection c);
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
