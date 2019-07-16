#ifndef CUBOID_H
#define CUBOID_H

#include <vector>
#include "Vec.h"
#include "Cube.h"
#include "NeuralNetwork.h"
#include "Body.h"

class Genome;
class Cuboid : public Body {
	public:
		static Genome* createGenome(
			Vec p = Vec(0, 0, 0),
			double l = 10,
			std::vector<unsigned int> sizes = {12, 12, 12}
		);

	    Cuboid(const Genome& g);
	    Cuboid(const Cuboid &other);
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
};

#endif // CUBOID_H
