#ifndef BALL_H
#define BALL_H

#include "MyMath.h"
#include "Component.h"
#include "Vec.h"

class NodeGene;
class Ball : public Component{
    public:
        Ball(Vec pos);
        Ball(Vec pos, double r_t);
        Ball(Vec pos, double r_t, double m_t);
        Ball(NodeGene nodeGene);

        virtual ~Ball();

        Vec position;
        Vec velocity;
        Vec acceleration;
        double radius;
        double mass;

        void draw() const override;
        void update(int t) override;
};

#endif // BALL_H
