#ifndef GENE_H
#define GENE_H

#include <vector>
#include <string>

class Genome;
class Gene {
    public:
        static constexpr char symbol = 'G';
        virtual char getSymbol() const = 0;

        Gene();
        Gene(const Gene& other);
        virtual ~Gene();

        virtual Gene* clone() const = 0;

        static std::string toStringFormat(std::vector<std::string> values);
        virtual std::string toString() const;
        virtual void mutate(Genome& genome) = 0;

};

#endif // GENE_H
