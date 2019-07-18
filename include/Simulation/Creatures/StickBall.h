#ifndef STICKBALL_H
#define STICKBALL_H

#include "Creature.h"
#include <vector>

class Genome;
class StickBall : public Creature {
	public:
		static Genome* createGenome(int n=5, int m=comb<5>(), int b=0, std::vector<unsigned int> sizes={16});

	    StickBall(const Genome& g);
	    StickBall(const StickBall &other);
	    operator=(const StickBall &other) = delete;
	    virtual ~StickBall();

	    void moveCOMTo(Vec to) override;
		Vec getCOM() const override;
		void lowerToGround() override;
        double getLowestBodyHeight() const override;
        virtual Vec getTop(const double offset) const override;


	    virtual void draw() const override;
        virtual void drawBrain(const bool drawAxons) const override;

        virtual void update(const int t) override;
        std::string getGenomeString() const override;

	protected:
		Vec* head;
		std::vector<Ball*> nodes;
	    std::vector<Piston*> muscles;
	    std::vector<Piston*> bones;
	    NeuralNetwork NN;
};

#endif // STICKBALL_H
