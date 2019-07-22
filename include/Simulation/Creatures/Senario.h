#ifndef SENARIO_H
#define SENARIO_H

#include "Vec.h"
#include "Trail.h"

class Terrain;
class Creature;
class Senario {
 public:
 	Creature* creature;
 	Trail food;
 	const Terrain& terrain;
    unsigned int evaluationTime;

    Senario(Creature* creature, const Terrain& terrain, unsigned int evaluationTime);
    Senario(const& Senario) = delete;
    Senario& operator=(const& Senario) = delete;
    virtual ~Senario();

    void draw() const;
    void update(int step);
    double getCumulativeFitness() const;
    double getCurrentFitness() const;
};

class SenarioFactory {
 public:
    SenarioFactory(const std::string type_t, const Terrain& terrain, unsigned int maxTime) : type(type_t), terrain(terrain), evaluationTime(maxTime) {

    }

    Senario* createSenario(Creature* creature) const {
        if (type == "SenarioA") {
            return new Senario(creature, terrain, evaluationTime);
        } else {
            exit(-254354);
        }
    }

 private:
    const std::string type;
    const Terrain& terrain;
    unsigned int evaluationTime;
};

#endif // SENARIO_H
