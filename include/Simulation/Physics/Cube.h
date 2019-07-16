#ifndef CUBE_H
#define CUBE_H

#include "MyMath.h"
#include "Component.h"

class CubeGene;
class Cube : public Component {
    public:
        Cube(CubeGene cubeGene);
        Cube(Vec position, double length=1.0, double mass=1.0);

        virtual ~Cube();

        Vec position;
        Vec velocity;
        Vec acceleration;
        double length;
        double mass;

        void draw() const override;
        void update(int t) override;
};

#endif // BALL_H
