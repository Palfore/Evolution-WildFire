#ifndef OMNIWALKER_H
#define OMNIWALKER_H

#include <vector>
#include "Vec.h"
#include "Cube.h"
#include "NeuralNetwork.h"
#include "Creature.h"
#include "Trail.h"
#include "Terrain.h"
#include "Eye.h"
class Genome;
class OmniWalker : public Creature {
	public:
		static Genome* createGenome(
			Vec p = Vec(0, 0, 0),
			double l = 10,
			std::vector<unsigned int> sizes = {8, 8, 8, 8},
			std::vector<unsigned int> sizes2 = {16}
		);

	    OmniWalker(const Genome& g);
	    OmniWalker(const OmniWalker &other);
	    OmniWalker& operator=(const OmniWalker &other) = delete;
	    virtual ~OmniWalker();

		virtual Vec calculateCOM() const override;
	    void moveCOMTo(Vec to) override;
		void lowerToGround() override;
        double getLowestBodyHeight() const override;
        virtual Vec getTop(const double offset) const override;

	    virtual void draw(const Senario* senario) const override;
        virtual void drawBrain(const bool drawAxons) const override;

        std::string getGenomeString() const override;

        virtual void update(Senario* senario, const int t) override;
	protected:
		Vec* head;
		Cube* body;
	    NeuralNetwork motorCortex;
	    NeuralNetwork visualCortex;
	    Eye eye;

};

#endif // OMNIWALKER_H
