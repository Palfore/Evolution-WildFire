#ifndef PISTON_H
#define PISTON_H

#include <vector>
#include "MyMath.h"

class Ball;
class MuscleGene;
class Genome;
class Piston {
    public:
        Piston(MuscleGene muscleGene, std::vector<Ball*> nodes);
        Piston(int a, int b, const Ball* ball1, const Ball* ball2, double speed_t); // This is used to copy instead.
        Piston(const Piston& obj) = delete; // Can't copy piston because of Ball reference (copy doesnt make sense)
        Piston& operator= (const Piston &obj) = delete;
        virtual ~Piston();

        const double initialLength;
        const double speed;

        void draw() const;
        void update(int t);

        int getIndex1() const;
        int getIndex2() const;
        Vec getPosition1() const;
        Vec getPosition2() const;
    private:
        int a;
        int b;

        const Ball *b1;
        const Ball *b2;

};

#endif // PISTON_H
