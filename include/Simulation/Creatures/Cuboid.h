#ifndef CUBOID_H
#define CUBOID_H

#include <vector>
#include "Vec.h"
#include "Cube.h"
#include "NeuralNetwork.h"
#include "Creature.h"
#include <deque>

class Genome;
class Cuboid : public Creature {
	public:
		static Genome* createGenome(
			Vec p = Vec(0, 0, 0),
			double l = 10,
			std::vector<unsigned int> sizes = {8, 8, 8, 8, 8}
		);

	    Cuboid(const Genome& g);
	    Cuboid(const Cuboid &other);
	    operator=(const Cuboid &other) = delete;
	    virtual ~Cuboid();

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
		std::vector<Cube*> cubes;
	    NeuralNetwork NN;

	    std::deque<Vec> positions = {};
};

#endif // CUBOID_H
