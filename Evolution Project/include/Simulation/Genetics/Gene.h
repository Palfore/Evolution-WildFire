#ifndef GENE_H
#define GENE_H

#include <vector>
#include <string>

// This class should be abstract, but for the clone function is isnt.
class Genome;
class Gene {
    public:
        static constexpr char symbol = '\0';

        Gene();
        Gene(const Gene& other);
        virtual ~Gene();

        virtual Gene* clone() const = 0;

        std::string toStringFormat(std::vector<std::string> values) const;
        virtual std::string toString() const;
        virtual void mutate(const Genome& genome) = 0;

};

#endif // GENE_H
