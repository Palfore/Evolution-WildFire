#ifndef BONEGENE_H
#define BONEGENE_H

#include <string>
#include "Gene.h"
struct Connection {
    int a;
    int b;
    Connection(int a_t, int b_t): a(a_t), b(b_t){};
    Connection(): a(-1), b(-1){};
    Connection(const Connection &c): a(c.a), b(c.b){};
};

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
