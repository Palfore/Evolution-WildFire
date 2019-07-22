#include "Cuboid.h"
#include "Genome.h"
#include "CubeGene.h"
#include "AxonGene.h"
#include "NodeGene.h"
#include "Logger.h"
#include "utility.h"
#include "Shapes.h"
#include <limits>

#include "Senario.h"
#include <numeric>

Genome* Cuboid::createGenome(Vec p, double l, std::vector<unsigned int> sizes) {
   Genome *g = new Genome();
   g->addGene(new CubeGene(p, l));

   sizes.insert(sizes.begin(), 4+0*1*5);
   sizes.push_back(2);
   for (unsigned int layer = 0; layer < sizes.size() - 1; layer++) {
       for (unsigned int i = 0; i < sizes[layer]; i++) {
           for (unsigned int j = 0; j < sizes[layer+1]; j++) {
               g->addGene(new AxonGene(i, j, layer, pmRandf(1)));
           }
       }
   }
   return g;
}

Cuboid::Cuboid(const Genome& genome) : Creature(genome), head(nullptr), body(nullptr), NN(genome.getGenes<AxonGene>()), eye() {
    for (auto const& gene: genome.getGenes<CubeGene>()) {
        this->body = new Cube(*gene);
    }

    lowerToGround();
    centerCOM();
    initCOM = getCOM();
    this->head = &this->body->position;
}


Cuboid::~Cuboid() {
    delete this->body;
}

#include "Eye.h"
Cuboid::Cuboid(const Cuboid &other) : Creature(other), head(nullptr), body(nullptr), NN(other.NN), eye(other.eye) {
    this->body = new Cube(*body);
    this->head = &this->body->position;
}

void Cuboid::draw(const Senario* senario) const {
    DrawSphere<Appearance::FACE>(*head + Vec(0,0,8), 5,
        180.0/3.1415926*atan2(moveTo.y - getCOM().y, moveTo.x - getCOM().x) + 90
    );
    DrawCylinder<Appearance::BLUE>(
		Vec(body->position.x, body->position.y, this->getLowestBodyHeight()),
		body->position + Vec(0, 0, body->length/2),
		1
	);
    const Vec averageV = -(this->positions[this->positions.size() - 1] - this->positions.getPoints()[0]);
    eye.draw(body->position, averageV.getUnit());

    senario->terrain.draw();

    double distanceFromGround = this->getLowestBodyHeight() - senario->terrain.getHeight(this->getCOM());
    constexpr double error = 1;

    //     DrawRing<Appearance::WHITE>(Vec(
    // this->creature->getCOM().x,
    // this->creature->getCOM().y,
    // this->creature->getLowestBodyHeight() + 5
    // ), 5, 0.5);
    if (distanceFromGround < error) {
        DrawSphere<Appearance::WHITE>(this->getCOM() - Vec(0, 0, 2) + 3*averageV.getUnit(), 3);
        DrawSphere<Appearance::WHITE>(this->getCOM() - Vec(0, 0, 2) - 3*averageV.getUnit(), 3);
    } else {
        DrawSphere<Appearance::RED>(this->getCOM() - Vec(0, 0, 2) + 3*averageV.getUnit(), 3);
        DrawSphere<Appearance::RED>(this->getCOM() - Vec(0, 0, 2) - 3*averageV.getUnit(), 3);
    }


    // return;

    // // DrawSphere<Appearance::BLUE>(
    // //     Vec(body->position.x, body->position.y, this->getLowestBodyHeight() + 1.5),
    // //     1.5
    // // );
    // // DrawSphere<Appearance::BLUE>(
    // //     body->position + Vec(0, 0, body->length/2),
    // //     0.5
    // // );

    // if (this->positions.size() <= 2) {
    //     return;
    // }

    // Vec fcom = getCOM();
    // Vec fpcom = getCOM(-1);

    // Vec averageP = Vec(0, 0, 0);
    // for (const auto& v: this->positions.getPoints()) {
    //     averageP += v;
    // }
    // averageP /= this->positions.size();//std::accumulate(this->creature->positions.getPoints().begin(), this->creature->positions.getPoints().end(), Vec(0, 0, 0)) / this->creature->positions.size();

    // const Vec averageV = -(this->positions[this->positions.size() - 1] - this->positions.getPoints()[0]);


    // DrawCylinder<Appearance::WHITE>(
    //     averageP+Vec(0, 0, 20),
    //     averageP+Vec(0, 0, 20)-50*(averageV).getUnit(),
    //     1
    // );

    // DrawCylinder<Appearance::RED>(
    //     fcom+Vec(0, 0, 20),
    //     fcom+Vec(0, 0, 20)+50*(getCOM() - getCOM(-1)).getUnit(),
    //     1
    // );

    // DrawCylinder<Appearance::GREEN>(
    //     fcom+Vec(0, 0, 20)+50*(getCOM() - getCOM(-1)).getUnit(),
    //     fpcom+Vec(0, 0, 0)+(moveTo - getCOM(-1)),
    //     1
    // );
    // DrawCylinder<Appearance::BLUE>(
    //     fpcom+Vec(0, 0, 20),
    //     fcom+Vec(0, 0, 0)+(moveTo - getCOM()),
    //     1
    // );
}

void Cuboid::drawBrain(const bool drawLines) const {
    this->NN.draw(drawLines);
}


Vec Cuboid::calculateCOM() const {
    Vec COM = Vec(0,0,0);
    double mass = 0.0;

    COM += body->position * body->mass;
    mass += body->mass;

    if (mass < 0.000000001) return Vec(0,0,0);
    return COM / mass;
}


void Cuboid::moveCOMTo(Vec to) {
    Vec COM = getCOM();
    Vec dr = to - COM;
    body->position += dr;
}

void Cuboid::lowerToGround() {
    double dz = this->getLowestBodyHeight() - 1; //-1 for not embeding creatures/padding
    body->position.z -= dz;
}

double Cuboid::getLowestBodyHeight() const {
    double minHeight = std::numeric_limits<double>::max();
    double lowestPart = body->position.z - body->length / 2;
    minHeight = lowestPart < minHeight  ? lowestPart : minHeight ;
    return minHeight;
}

Vec Cuboid::getTop(const double offset=0) const {
    double highestNode = -std::numeric_limits<double>::max();
    double comX = 0;
    double comY = 0;

    double mass = 0.0;
    highestNode = body->position.z > highestNode ? body->position.z : highestNode;
    comX += body->position.x * body->mass;
    comY += body->position.y * body->mass;
    mass += body->mass;
    return Vec(comX / mass, comY / mass, highestNode + offset);
}

void Cuboid::update(Senario* senario, int t) {
    constexpr double dt = 0.05;

    if (t == 0) {
        this->moveTo = Vec(pmRandf(100, 101) + 0*pmRandf(100, 200), 0*pmRandf(70, 100), 0);
        this->moveTo.z = terrain.getHeight(this->moveTo);

        this->moveCOMTo(Vec(1, 1, terrain.getHeight(1, 1) + 50));

        this->positions.clear();
        this->initCOM = this->calculateCOM();
    }
    this->Creature::update(senario, t);

    /* Brain Processing */
    Vec averageP = Vec(0, 0, 0);
    for (const auto& v: this->positions.getPoints()) {
        averageP += v;
    }
    averageP /= this->positions.size();//std::accumulate(this->positions.getPoints().begin(), this->positions.getPoints().end(), Vec(0, 0, 0)) / this->positions.size();

    const Vec averageV = -(this->positions[this->positions.size() - 1] - this->positions.getPoints()[0]);
    const Vec red   = averageV;
    const Vec blue  = -(this->moveTo - averageP + averageV);
    const Vec green = (this->moveTo - averageP);
    const double sign = sgn(red.y*green.x - red.x*green.y); // xprod
    const double angle = sign * atan2(red.cross(blue).length(), red.dot(blue)) / PI;

    std::vector<double> inputs = {1};
    inputs.push_back(angle);
    inputs.push_back(tanh(0.01*green.length()));
    inputs.push_back(tanh(0.05*averageV.length()));

    Vec para = -averageV;  // Signs chosen for +1 forward/right
    Vec perp = -averageV.cross(Vec(0, 0, 1));
    para = para.length() < 1e-8 ? Vec(0, 1, 0) : para / para.length();
    perp = perp.length() < 1e-8 ? Vec(0, 1, 0) : perp / perp.length();

    /* Gravity */
    const Vec gravity = 0.2 * Vec(0, 0, -1);
    this->body->acceleration = gravity;

    /* Drag */
    const Vec drag = 0.0001*averageV.lengthSquare() * para;
    body->acceleration += drag;

    /* Ground Contact Forces */
    double distanceFromGround = this->getLowestBodyHeight() - senario->terrain.getHeight(this->getCOM());
    constexpr double error = 1;
    if (distanceFromGround < error) {
        const double strength = 0.6;
        std::vector totalInputs(inputs);
        // this->eye.look(this->body->position, averageV);

        // for (const auto& ray: this->eye.rays) {
        //     totalInputs.push_back(ray.k);
        // }

        Vec para = averageV;  // Signs chosen for +1 forward/right
        Vec perp = averageV.cross(Vec(0, 0, 1));
        para = para.length() < 1e-8 ? Vec(0, 1, 0) : para / para.length();
        perp = perp.length() < 1e-8 ? Vec(0, 1, 0) : perp / perp.length();

        std::vector<double> da = this->NN.propagate(totalInputs);
        this->body->acceleration += strength*(da[0]*perp + da[1]*para) / body->velocity.length();

        if (distanceFromGround < 1e-4) {
            /* Normal Force */
            double C = 0.2;
            Vec v1 = body->velocity;
            Vec v2 = Vec(0, 0, 0);
            Vec N = senario->terrain.getNormal(this->getCOM());
            body->velocity += -2 * C * (v1 - v2).dot(N) * N;
            body->position.z = senario->terrain.getHeight(this->body->position) +
                                    (this->body->position.z - this->getLowestBodyHeight()) + 1e-4;

            /* Friction */
            body->velocity *= (1 - 0.01 * dt);
        }
    }
    body->velocity += body->acceleration * dt;
    body->position += body->velocity * dt;
}


std::string Cuboid::getGenomeString() const {
    std::string genomeString = Creature::getGenomeString();
    genomeString += Gene::toStringFormat(std::vector<std::string>({std::string(1, CubeGene::symbol),
        utility::numToStr<double>(body->position.x),
        utility::numToStr<double>(body->position.y),
        utility::numToStr<double>(body->position.z),
        utility::numToStr<double>(body->mass)
    }));
    return genomeString;
}




