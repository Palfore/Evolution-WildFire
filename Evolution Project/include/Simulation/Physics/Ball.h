#ifndef BALL_H
#define BALL_H

#include "MyMath.h"
#include "NodeGene.h"
class Ball {
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

        void draw() const;
        void update(int t);
};

#endif // BALL_H
