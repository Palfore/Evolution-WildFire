#ifndef MUSCLEGENE_H
#define MUSCLEGENE_H

#include "Gene.h"
#include <string>



//// This is the new one
#include <map>
#include "BoneGene.h"
class Genome;

class MuscleGene : public BoneGene {

    public:
        static constexpr char symbol = 'm';
        virtual char getSymbol() const override {return symbol;};

        double speed;

//        enum class Mut : unsigned int {ADD_MUSCLE, REMOVE_MUSCLE};
//
//        std::map<enum Mutation, double> mutationRates = {{Mut::ADD_MUSCLE, 0.4543}, {Mut::REMOVE_MUSCLE, 0.1242}};

        MuscleGene(std::string representation);
        MuscleGene(int numNodes, const Genome& genome);
        MuscleGene(int a, int b, double s=getRandomSpeed());
        MuscleGene(Connection c, double s=getRandomSpeed());
        MuscleGene(const MuscleGene& other);
        virtual ~MuscleGene();

        virtual Gene* clone() const override;
        void mutate(Genome& genome) override;
        std::string toString() const override;

    private:
        static double getRandomSpeed();
};

#endif // MUSCLEGENE_H
