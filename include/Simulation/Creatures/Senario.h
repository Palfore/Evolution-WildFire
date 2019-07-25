#ifndef SENARIO_H
#define SENARIO_H

#include "Vec.h"
#include "Trail.h"

#include <unordered_map>
#include <utility>
#include <functional>
#include <iostream>

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

class Chunks {
public:
    static constexpr double l = 200;
    std::unordered_map<std::pair<int, int>, std::vector<Vec>, hash_pair> chunks;

    void insertAt(double x, double y, Vec toPlace) {
        const auto key = std::pair<int, int>(floor(x/l)*l, floor(y/l)*l);
        chunks.try_emplace(key, std::vector<Vec>());
        chunks[key].push_back(toPlace);
    }

    std::vector<Vec> getFrom(double x, double y) const {
        const std::pair<int, int> key = std::pair<int, int>(floor(x/l)*l, floor(y/l)*l);
        if (chunks.find(key) != chunks.end()) {
            return chunks.at(key);
        } else {
            return {};
        }
    }

    std::vector<Vec> getFrom(double x, double y, int chunkDistance) const {
        std::vector<Vec> entities = {};
        for (int i = -chunkDistance; i <= chunkDistance; i++) {
            for (int j = -chunkDistance; j <= chunkDistance; j++) {
                auto chunkEntities = getFrom(x+i*l, y+j*l);
                entities.insert(
                    entities.end(),
                    std::make_move_iterator(chunkEntities.begin()),
                    std::make_move_iterator(chunkEntities.end())
                );
            }
        }
        return entities;
    }


    void remove(double x, double y, Vec toRemove) {
        const std::pair<int, int> key = std::pair<int, int>(floor(x/l)*l, floor(y/l)*l);
        if (chunks.find(key) != chunks.end()) {
            auto& vec = chunks.at(key);
            vec.erase(std::remove(vec.begin(), vec.end(), toRemove), vec.end());
        }
    }
};




class Terrain;
class Creature;
class Senario {
 public:
 	Creature* creature;
 	Chunks food;
 	const Terrain& terrain;
    unsigned int maxEvaluationTime;

    Senario(Creature* creature, const Terrain& terrain, unsigned int maxEvaluationTime);
    Senario(const Senario&) = delete;
    Senario& operator=(const Senario&) = delete;
    virtual ~Senario();

    void draw() const;
    void update(int step);
    virtual double getCurrentFitness();
};

class SenarioB: public Senario {
 public:
    SenarioB(Creature* creature, const Terrain& terrain, unsigned int maxEvaluationTime);
    double getCurrentFitness() override;
};


class SenarioFactory {
 public:
    SenarioFactory(const std::string type_t, const Terrain& terrain_t, unsigned int maxTime) : type(type_t), terrain(terrain_t), maxEvaluationTime(maxTime) {

    }

    Senario* createSenario(Creature* creature) const {
        if (type == "SenarioA") {
            return new Senario(creature, terrain, maxEvaluationTime);
        } else if (type == "SenarioB") {
            return new SenarioB(creature, terrain, maxEvaluationTime);
        } else {
            exit(-254354);
        }
    }

 private:
    const std::string type;
    const Terrain& terrain;
    unsigned int maxEvaluationTime;
};

#endif // SENARIO_H
