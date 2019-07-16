#ifndef CUBEGENE_H
#define CUBEGENE_H

#include "Gene.h"
#include "Vec.h"

#include <string>
#include <map>

class Genome;
class CubeGene : public Gene {
    public:
        static constexpr char symbol = 'c';
        virtual char getSymbol() const override {return symbol;};

        Vec position;
        double length;

        CubeGene(std::string representation);
        CubeGene(Vec position, double length=10);
        CubeGene(const CubeGene& other);
        virtual ~CubeGene();

        virtual Gene* clone() const override;
        void mutate(Genome& genome) override;
        std::string toString() const override;
};

#endif // CUBEGENE_H
