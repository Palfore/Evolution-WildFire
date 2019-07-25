#include "OmniWalker.h"
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

Genome* OmniWalker::createGenome(Vec p, double l, std::vector<unsigned int> sizes, std::vector<unsigned int> sizes2) {
   Genome* g = new Genome("<MetaData>|a,0,0,0,0,0.420063|a,0,1,0,0,0.687784|a,0,2,0,0,-0.254708|a,0,3,0,0,0.718846|a,0,4,0,0,-0.453164|a,0,5,0,0,0.334362|a,0,6,0,0,-0.162064|a,0,7,0,0,-0.408915|a,0,8,0,0,0.232438|a,0,9,0,0,0.191498|a,0,10,0,0,0.685312|a,0,11,0,0,-0.983362|a,0,12,0,0,0.612766|a,0,13,0,0,0.431219|a,0,14,0,0,0.0483559|a,0,15,0,0,0.379093|a,1,0,0,0,-0.773392|a,1,1,0,0,0.583053|a,1,2,0,0,-0.00519792|a,1,3,0,0,-0.452463|a,1,4,0,0,-0.393087|a,1,5,0,0,-0.973426|a,1,6,0,0,0.620525|a,1,7,0,0,0.379932|a,1,8,0,0,0.778292|a,1,9,0,0,-0.655444|a,1,10,0,0,-0.37295|a,1,11,0,0,-0.0206714|a,1,12,0,0,0.67408|a,1,13,0,0,-0.879565|a,1,14,0,0,0.955905|a,1,15,0,0,-0.661512|a,2,0,0,0,-0.673464|a,2,1,0,0,-0.0543843|a,2,2,0,0,-0.754413|a,2,3,0,0,-0.0345751|a,2,4,0,0,-0.692485|a,2,5,0,0,-0.706441|a,2,6,0,0,0.211662|a,2,7,0,0,0.584657|a,2,8,0,0,0.360824|a,2,9,0,0,-0.114418|a,2,10,0,0,0.0946332|a,2,11,0,0,0.71085|a,2,12,0,0,0.101175|a,2,13,0,0,-0.000502881|a,2,14,0,0,-0.767482|a,2,15,0,0,0.48562|a,3,0,0,0,0.0984926|a,3,1,0,0,-0.708502|a,3,2,0,0,-0.75134|a,3,3,0,0,0.66341|a,3,4,0,0,0.753482|a,3,5,0,0,-0.429575|a,3,6,0,0,-0.610176|a,3,7,0,0,-0.160742|a,3,8,0,0,0.237906|a,3,9,0,0,-0.249248|a,3,10,0,0,-0.476208|a,3,11,0,0,-0.680952|a,3,12,0,0,0.143501|a,3,13,0,0,0.99656|a,3,14,0,0,0.663586|a,3,15,0,0,0.288387|a,0,0,1,0,-0.896273|a,0,1,1,0,-0.620004|a,1,0,1,0,0.11342|a,1,1,1,0,-0.247006|a,2,0,1,0,0.611123|a,2,1,1,0,0.492562|a,3,0,1,0,0.395304|a,3,1,1,0,0.397961|a,4,0,1,0,-0.274379|a,4,1,1,0,0.798957|a,5,0,1,0,0.00573515|a,5,1,1,0,0.379439|a,6,0,1,0,0.267684|a,6,1,1,0,0.0606665|a,7,0,1,0,0.542254|a,7,1,1,0,-0.327419|a,8,0,1,0,0.753362|a,8,1,1,0,0.065275|a,9,0,1,0,-0.252295|a,9,1,1,0,0.876419|a,10,0,1,0,-0.322335|a,10,1,1,0,0.509585|a,11,0,1,0,0.583369|a,11,1,1,0,0.360204|a,12,0,1,0,-0.629581|a,12,1,1,0,0.807533|a,13,0,1,0,0.611083|a,13,1,1,0,-0.102697|a,14,0,1,0,-0.866454|a,14,1,1,0,0.446801|a,15,0,1,0,0.130621|a,15,1,1,0,-0.725702|c,0,0,0,10");
   // Genome *g = new Genome();
   // g->addGene(new CubeGene(p, l));

   // int network = 0;
   // sizes.insert(sizes.begin(), 4);
   // sizes.push_back(2);
   // for (unsigned int layer = 0; layer < sizes.size() - 1; layer++) {
   //     for (unsigned int i = 0; i < sizes[layer]; i++) {
   //         for (unsigned int j = 0; j < sizes[layer+1]; j++) {
   //             g->addGene(new AxonGene(i, j, layer, network, pmRandf(1)));
   //         }
   //     }
   // }
   return g;
}

OmniWalker::OmniWalker(const Genome& genome) :
        Creature(genome), head(nullptr), body(nullptr),
        motorCortex(0, genome.getGenes<AxonGene>()), visualCortex(1, genome.getGenes<AxonGene>()),
        eye(2, 8) {
    for (auto const& gene: genome.getGenes<CubeGene>()) {
        this->body = new Cube(*gene);
    }

    lowerToGround();
    centerCOM();
    initCOM = getCOM();
    this->head = &this->body->position;
}


OmniWalker::~OmniWalker() {
    delete this->body;
}

#include "Eye.h"
OmniWalker::OmniWalker(const OmniWalker &other):
        Creature(other), head(nullptr), body(nullptr),
        motorCortex(other.motorCortex), visualCortex(other.visualCortex), eye(other.eye) {
    this->body = new Cube(*body);
    this->head = &this->body->position;
}

void OmniWalker::draw(const Senario* senario) const {
    DrawCylinder<Appearance::BLUE>(
		Vec(body->position.x, body->position.y, this->getLowestBodyHeight()),
		body->position + Vec(0, 0, body->length/2),
		1
	);
    Vec averageP = Vec(0, 0, 0);
    for (const auto& v: this->positions.getPoints()) {
        averageP += v;
    }
    averageP /= this->positions.size();
    const Vec averageV = -(this->positions[this->positions.size() - 1] - this->positions.getPoints()[0]);
    DrawSphere<Appearance::FACE>(*head + Vec(0,0,8), 5,
        180.0/3.1415926*atan2(averageV.y, averageV.x) - 90
    );
    eye.draw();

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

    DrawCylinder<Appearance::WHITE>(
        averageP+Vec(0, 0, 20),
        averageP+Vec(0, 0, 20)-50*(averageV).getUnit(),
        1
    );

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

void OmniWalker::drawBrain(const bool drawLines) const {
    this->visualCortex.draw(drawLines);
    // this->motorCortex.draw(drawLines);
}


Vec OmniWalker::calculateCOM() const {
    Vec COM = Vec(0,0,0);
    double mass = 0.0;

    COM += body->position * body->mass;
    mass += body->mass;

    if (mass < 0.000000001) return Vec(0,0,0);
    return COM / mass;
}


void OmniWalker::moveCOMTo(Vec to) {
    Vec COM = getCOM();
    Vec dr = to - COM;
    body->position += dr;
}

void OmniWalker::lowerToGround() {
    double dz = this->getLowestBodyHeight() - 1; //-1 for not embeding creatures/padding
    body->position.z -= dz;
}

double OmniWalker::getLowestBodyHeight() const {
    double minHeight = std::numeric_limits<double>::max();
    double lowestPart = body->position.z - body->length / 2;
    minHeight = lowestPart < minHeight  ? lowestPart : minHeight ;
    return minHeight;
}

Vec OmniWalker::getTop(const double offset=0) const {
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

void OmniWalker::update(Senario* senario, int t) {
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
    Vec green = (this->moveTo - averageP);
    const double sign = sgn(red.y*green.x - red.x*green.y); // xprod
    double angle = sign * atan2(red.cross(blue).length(), red.dot(blue)) / PI;

    std::vector<double> inputs = {-1};
    inputs.push_back(angle);
    inputs.push_back(tanh(0.01*green.length()));
    inputs.push_back(tanh(0.05*averageV.length()));

    Vec para = averageV;  // Signs chosen for +1 forward/right
    Vec perp = averageV.cross(Vec(0, 0, 1));
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

    this->eye.look(this->body->position, averageV, *senario);

    // std::vector<double> visualInputs = {1};
    // for (const auto& ray: this->eye.rays) {
    //     visualInputs.push_back(1 - ray.k);
    // }
    // std::vector<double> dmoveTo = this->visualCortex.propagate(visualInputs);
    // angle = 0.05*pow(dmoveTo[0], 5);
    // green = green.getUnit();
    // double x = 200*fabs(dmoveTo[1])*(green.x * cos(angle) - green.y * sin(angle));
    // double y = 200*fabs(dmoveTo[1])*(green.x * sin(angle) + green.y * cos(angle));
    // this->moveTo = Vec(x + averageP.x, y + averageP.y, senario->terrain.getHeight(x + averageP.x, y + averageP.y));

    if (distanceFromGround < error) {
        const double strength = 0.6;

        std::vector<double> da = this->motorCortex.propagate(inputs);
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
            body->velocity *= (1 - 0.03 * dt);
        }
    }
    body->velocity += body->acceleration * dt;
    body->position += body->velocity * dt;
}


std::string OmniWalker::getGenomeString() const {
    std::string genomeString = Creature::getGenomeString();
    genomeString += Gene::toStringFormat(std::vector<std::string>({std::string(1, CubeGene::symbol),
        utility::numToStr<double>(body->position.x),
        utility::numToStr<double>(body->position.y),
        utility::numToStr<double>(body->position.z),
        utility::numToStr<double>(body->mass)
    }));
    return genomeString;
}




