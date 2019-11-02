#ifndef RECUR_EYE_WALKER_H
#define RECUR_EYE_WALKER_H

#include <vector>
#include "Vec.h"
#include "Cube.h"
#include "NeuralNetwork.h"
#include "Creature.h"
#include "Trail.h"
#include "Terrain.h"
#include "RecurrentEye.h"

class Genome;
class RecurEyeWalker : public Creature {
	public:
		static Genome* createGenome(
			Vec p = Vec(0, 0, 0),
			double l = 10,
			std::vector<unsigned int> sizes = {8, 8, 8, 8},
			std::vector<unsigned int> sizes2 = {8}
		);

	    RecurEyeWalker(const Genome& g);
	    RecurEyeWalker(const RecurEyeWalker &other);
	    RecurEyeWalker& operator=(const RecurEyeWalker &other) = delete;
	    virtual ~RecurEyeWalker();

		virtual Vec calculateCOM() const override;
	    void moveCOMTo(Vec to) override;
		void lowerToGround() override;
        double getLowestBodyHeight() const override;
        virtual Vec getTop(const double offset) const override;

	    virtual void draw(const Scenario* scenario) const override;
        virtual void drawBrain(const bool drawAxons) const override;

        std::string getGenomeString() const override;

        virtual void update(Scenario* scenario, const int t) override;
	protected:
		Vec* head;
		Cube* body;
	    // NeuralNetwork visualCortex;
	    RecurrentNeuralNetwork visualCortex;
	    NeuralNetwork motorCortex;
	    RecurrentEye eye;
};

#endif // RECUR_EYE_WALKER_H
