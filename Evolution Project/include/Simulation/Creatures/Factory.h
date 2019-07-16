#ifndef FACTORY_H
#define FACTORY_H

#include <string>
class Genome;
class Body;
#include "Genome.h"
#include "StickBall.h"
#include "Cuboid.h"

class Factory {
	public:
	    Factory(const std::string type_t) : type(type_t) {}

	    Genome* createGenome() const {
	        if (type == "StickBall") return StickBall::createGenome();
	        else if (type == "Cuboid") return Cuboid::createGenome();
	        else exit(-1241);
	    }
	    Body* createBody(Genome g) const {
            if (type == "StickBall") return new StickBall(g);
            else if (type == "Cuboid") return new Cuboid(g);
            else exit(0143);
	    }

	private:
	    const std::string type;
};

#endif // FACTORY_H
