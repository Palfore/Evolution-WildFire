#ifndef PHYLOGENY_H
#define PHYLOGENY_H

#include <vector>
#include <string>
#include <map>

class Connection;
class Genome;
class Phylogeny {
        static constexpr int NUM_RANKS = 6;
    public:
        const std::array<std::string, NUM_RANKS> RANKS = {"Domain", "Kingdom", "Phylum", "Order", "Genus", "Species"};
        const std::array<std::string, NUM_RANKS> RANKS_scientific = {"Domain", "Concept", "Components", "Quantities", "Structure", "Motion"};

        static bool scientificNames;

        Phylogeny(Genome g);
        virtual ~Phylogeny();

        std::string getDomain() const;
        std::string getKingdom() const;
        std::string getPhylum() const;
        std::string getOrder() const;
        std::string getGenus() const;
        std::string getSpecies() const;

        std::vector<std::array<std::string, 2>> getPhylogenyDict() const;
        std::vector<std::string> getPhylogenyNames() const;
        std::string toString() const;

        static int getNumTrails(std::vector<Connection> connections, int numNodes=-1); ///< @todo Move these to Metrics Class
        static int getNumLoops(std::vector<Connection> connections);
    private:
        int numNodes;
        int numMuscles;
        int numBones;
        int numAxons;

        int numLoops;
        int numTrails;


};

#endif // PHYLOGENY_H
