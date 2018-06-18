#ifndef HISTORY_H
#define HISTORY_H


#include "Genome.h"
#include "utility.h"
#include "Vec2.h"
class HistoryPoint {
    private:
        class Value : public Vec2 { // Holds a value and error as a Vec2(x, y).
            public:
                Value(double v, double err) : Vec2(v, err) {}
                Value() : Vec2(0, 0) {}
                virtual ~Value() {}

                std::string toString() const {
                    return utility::numToStr<double>(x) + "+/-" + utility::numToStr<double>(y);
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
        static constexpr int NUM_PERCENTILES = 10;

        HistoryPoint(std::vector<Genome*> genomes, int gen);
        ~HistoryPoint() {}

        std::string toString() const;

        int generation;
        Value mean;
        GenomeValue top;
        GenomeValue middle;
        GenomeValue bottom;
        std::array<GenomeValue, NUM_PERCENTILES> percentiles;
};

class History {
    public:
        History() : data({}) {}
        virtual ~History() {}

        void addPoint(std::vector<Genome*> genomes, int gen);
        void writeToFile() const;
        void writeToConsole() const;
        void graph() const;

        std::vector<HistoryPoint> data;

    private:
        static constexpr int MAX_NUM_DATA = 1000; ///< After this, the every other data point will be removed.
};
#endif // HISTORY_H
