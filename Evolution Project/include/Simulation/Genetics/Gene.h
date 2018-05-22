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

        virtual Gene* clone() const;

        std::string toStringFormat(char geneSymbol, std::vector<std::string>) const;
        virtual std::string toString() const;
        virtual void mutate(const Genome& genome);

};

#endif // GENE_H
