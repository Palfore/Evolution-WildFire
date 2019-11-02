/* This file contains the factory for creating different creature types.
	To add a new creature to the factory, include its header and add its class
	To the map.

	Usage:
		const Factory factory("StickBall");

		Genome* genome = factory.createGenome();
		Creature* creature = factory.createCreature(genome);
		// OR
		Creature* creature = factory.createCreature();


		Population population = Population(1000, factory);
		Viewer viewer = Viewer(population.genomes, factory);
*/

#ifndef FACTORY_H
#define FACTORY_H

#include "Genome.h"
#include "Creature.h"

#include <string>
#include <unordered_map>
#include <functional>

#define CREATE_BODY(type) [](Genome g){return new type(g);}
#define CREATE_GENOME(type) [](){return type::createGenome();}
#define CREATURE_PAIR(name) std::pair(#name, std::pair(CREATE_GENOME(name), CREATE_BODY(name)))
using functions = std::pair<std::function<Genome*()>, std::function<Creature*(Genome g)>>;

/* Register Creatures Here */
#include "StickBall.h"
#include "OmniWalker.h"
#include "EyeWalker.h"
#include "RecurEyeWalker.h"
static std::unordered_map<std::string, functions> function_map = {
	CREATURE_PAIR(StickBall),
	CREATURE_PAIR(OmniWalker),
	CREATURE_PAIR(EyeWalker),
	CREATURE_PAIR(RecurEyeWalker),
};

class Creature;
class Factory {
	public:
	    Factory(const std::string type_t) : type(type_t) {}

	    Genome* createGenome() const {
	    	return function_map[type].first();
	    }

	    Creature* createCreature() const {
	    	Genome* g = this->createGenome();
	    	Creature* body = this->createCreature(*g);
	    	delete g;
            return body;
	    }

	    Creature* createCreature(Genome g) const {
            return function_map[type].second(g);
	    }

	private:
	    const std::string type;
};

#endif // FACTORY_H
