#ifndef HISTORY_H
#define HISTORY_H


#include "Genome.h"
#include "utility.h"
#include "Vec2.h"


/** @brief This class statistics about a collection of Genome's.
    @details
    The statistics collected are:
        - Mean
        - Best Creature
        - Median Creature
        - Worse Creature
        - 10 Percentiles

    Future statistics which may be collected are:
        - Average Number of NodeGene's
        - Average Number of MuscleGene's
        - Average Number of BoneGene's
        - Average Number of Neuron's (Not a Gene)
        - Average Number of Layers's (Not a Gene)
        - Average Number of AxonGene's

    @todo When fitness selection expands to include different measures, this class may have to change.
    @todo Toggle if genomes should be displayed.
 */
class HistoryPoint {
    private:
        class Value : public Vec2 { // Holds a value and error as a Vec2(x, y).
            public:
                Value(double v, double err) : Vec2(v, err) {}
                Value() : Vec2(0, 0) {}
                virtual ~Value() {}

                std::string toString() const {
                    return utility::numToStr<double>(x) + " +/- " + utility::numToStr<double>(y);
                }
        };

        class GenomeValue { // Holds a value (fitness most likely) and associated (std::string) genome.
            public:
                GenomeValue(double v, std::string g) : x(v), genome(g) {}
                GenomeValue() : x(0), genome("") {}
                virtual ~GenomeValue() {}

                std::string toString() const {
                    return utility::numToStr<double>(x) ;//+ ':' + genome;
                }
                double x;
            private:
                std::string genome;
        };

    public:
        static constexpr int NUM_PERCENTILES = 10;  ///< The number of percentiles to determine (split evenly).

        HistoryPoint(std::vector<Genome*> genomes, int gen);
        ~HistoryPoint() {}

        std::string toString() const;   ///< Convert the statistics to a human-readable format.

        int generation;                                         ///< The generation that the genomes come from.
        Value mean;                                             ///< The average fitness value of the population.
        GenomeValue top;                                        ///< The highest fitness of the population.
        GenomeValue middle;                                     ///< The median fitness of the population.
        GenomeValue bottom;                                     ///< The lowest fitness of the population.
        std::array<GenomeValue, NUM_PERCENTILES> percentiles;   ///< The percentiles of the population. @todo The calculation is approximate.
};


/** @brief This class holds the statistics about the generations in a population. @see HistoryPoint for more details about the statistics collected.
 */
class History {
    public:
        History() : data({}) {}
        virtual ~History() {}

        /** @brief Add a point (containing statistics) to the history data given a genome.
         *
         * @param genomes The std::vector of Genome*'s to be analyzed.
         * @param gen The number of iterations this creature has gone through.
         * @return
         *
         */
        void addPoint(std::vector<Genome*> genomes, int gen);

        /** @brief Writes the history data to a file.
         *
         * @return
         */
        void writeToFile(const std::string& filename) const;

        /** @brief Writes the history data to std::cout.
         *
         * @return
         */
        void writeToConsole() const;

        /** @brief Draws a graph in a black box covering [0.1, 0.1] to [0.9, 0.9].
         *
         * @return
         */
        void graph() const;

        std::vector<HistoryPoint> data; ///< The std::vector which contains all the history data. (Should be made private with a getter)

    private:
        static constexpr int MAX_NUM_DATA = 300; ///< After the number of data points exceeds this, every other data point stored will be removed to make room.
};
#endif // HISTORY_H
