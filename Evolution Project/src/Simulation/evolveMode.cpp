#include "Simulation.h"
#include "Population.h"
#include "Shapes.h"
#include "Objects.h"
#include "Text.h"
#include "utility.h"
#include "GFramework.h"
#include "UserFunction.h"
#include "UserInput.h"
#include "Genome.h"
#include "Button.h"
#include "MultiThread.h"

#include "Creature.h"
#include <thread>

static Population pop(4*256);
static int gameSpeed = 6;
static bool drawGraph = false;
static bool drawCOMTrails = false;
static bool drawNodeTrails = false;
static bool drawBrain = false;
static bool drawLines = false;
static bool cinematic = false;
#include "MyMath.h"

static constexpr int MIN_THREADS = 0; // No processing threads
static const     int MAX_THREADS = std::thread::hardware_concurrency();
static constexpr int BACKGROUND_THREADS_REQUIRED = 2;
static const     int DEFAULT_THREADS = MAX_THREADS - BACKGROUND_THREADS_REQUIRED > 0 ? MAX_THREADS - BACKGROUND_THREADS_REQUIRED : 1;

static constexpr int MAX_GAME_SPEED = 20;
static constexpr int MIN_GAME_SPEED = 0; // No updating displayed creatures


static int numThreads = DEFAULT_THREADS;
static Threader threader(0, pop.getBodies());

static auto startTime = std::chrono::high_resolution_clock::now();
static std::vector<double> times = {};

static void drawing(double fps) {
    const int r = 50;
    int bounds = 1000;
    for (int x = -bounds; x < bounds; x+=r) {
        for (int y = -bounds; y < bounds;y += r) {
           DrawPlane<Appearance::GRASS>(Vec(x, y, 0), r/2);
        }
    }

    DrawSphere<Appearance::SKY_BLUE>(Vec(-50,0,0), 5);
    DrawSphere<Appearance::RED>(Vec(-100,0,0), 5);
    DrawSphere<Appearance::RED>(Vec(-150,0,0), 5);
    DrawSphere<Appearance::RED>(Vec(-200,0,0), 5);
    DrawSphere<Appearance::RED>(Vec(-250,0,0), 5);
    DrawSphere<Appearance::RED>(Vec(-300,0,0), 5);
    DrawSphere<Appearance::RED>(Vec(-350,0,0), 5);


    DrawSphere<Appearance::FOOD>(Vec(pop.getActiveCreature().body.moveTo.x, pop.getActiveCreature().body.moveTo.y, 10), 10);
    DrawSphere<Appearance::FACE>(pop.getActiveCreature().getHead() + Vec(0,0,4), 5,
                                  -sgn<double>(pop.getActiveCreature().body.moveTo.x - pop.getActiveCreature().body.com.x) * 90);


    DrawSkybox(700);

    auto tree = Vec(-50, 50, 0);
    DrawCylinder<Appearance::BARK>(tree, tree + Vec(0,0,50), 8);
    DrawSphere<Appearance::TREE_TOP>(tree + Vec(0,0,50), 25);

    pop.draw();
    pop.getActiveCreature().drawTrails(drawNodeTrails, drawCOMTrails);

    /////////////////////////////////////////// 2D Drawing ///////////////////////////////////////////////////
    DrawString<Appearance::WHITE>("Frank F"+ utility::numToStr<int>(pop.getActiveCreature().fitness.getFitness(FitnessCollector::TOTAL_DISTANCE)), pop.getActiveCreature().body.getTop());
    DrawString<Appearance::BLACK>("Generation " + utility::numToStr<int>(pop.gen), 0.01, 0.03, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("Creature: " + utility::numToStr<int>(pop.getMemberIndex()) +
                                " / " + utility::numToStr<int>(pop.population.size()) +
                                " (" + utility::numToStr<int>(pop.viewingGenomes.size()) + ')', 0.01, 0.05, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("Time " + utility::numToStr<int>(pop.getSimStep()), 0.01, 0.07, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("NumThreads " + utility::numToStr<int>(numThreads) + " (" + utility::numToStr<int>(threader.size()) + ")", 0.01, 0.09, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("GameSpeed " + utility::numToStr<int>(gameSpeed), 0.01, 0.11, by_percentage(), false, false);

    DrawString<Appearance::BLACK>("NumUserFuncs: " + utility::numToStr<int>(GFramework::get->userInput.functions.size()), 0.01, 0.13, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("FPS: " + utility::numToStr<double>(fps), 0.01, 0.15, by_percentage(), false, false);
    double inc = 0.02;
    double place = 0.15 + inc;

    for (auto const& name: pop.getActiveCreature().phylogeny.getPhylogenyDict()) {
        DrawString<Appearance::BLACK>(name[0] + ": " +  name[1], 0.01, place+=inc, by_percentage(), false, false);
    }

    pop.getActiveCreature().drawNodeData();

    //// Overlays ////
    if (drawGraph) {
        pop.history.graph();
    }
    if (drawBrain) {
        pop.getActiveCreature().drawBrain(drawLines);
    }
}

void Simulation::evolveMode(double fps) {
    static int doneGeneration = 0;
    bool throttleFPS = true;

    if (cinematic) {
        GFramework::get->camera->cinematicCamera(Vec(pop.getActiveCreature().body.com.x, pop.getActiveCreature().body.com.y, 0));
    }
    switch(doneGeneration) {
        case 10: pop.sortPop(); break;
        case 9:  pop.recordHistory();break;
        case 8:  break;
        case 7:  pop.select(); break;
        case 6:  break;
        case 5:  pop.mutate(); pop.gen++; break;
        case 4:  break;
        case 3:  break;
        case 2:  break;
        case 1:  threader.spawnThreads(pop.getBodies()); throttleFPS = false; break;
        default: doneGeneration = 0;
    }

    if (doneGeneration > 0) {
        doneGeneration--;
    } else if (threader.canProcess()) {
        pop.updateFitnesses(threader.getFitnesses(pop.population.size()));
        doneGeneration = 10;
    }
    threader.update(numThreads);


    for (int i = 0; i < gameSpeed; i++) {
        pop.nextStep();
    }
    drawing(fps);


    auto endTime = std::chrono::high_resolution_clock::now();
    auto elaspedTime = std::chrono::duration<double>(endTime - startTime).count();
    startTime = endTime;
    if (throttleFPS) {
        double FPS = 50.0;
        int x = (1.0/FPS - elaspedTime) * 1'000;
        std::this_thread::sleep_for(std::chrono::milliseconds(x));
    }
}



void Simulation::loadEvolve() {
    GFramework::get->camera->reset();
    Camera *camera = GLOBAL->camera;
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;

    /* Game Controls */
    userFunctions->push_back(UserFunction(new UIchar('p'), [](){
        pop.printCurrentGenome();
    }));
    userFunctions->push_back(UserFunction(new UIchar('o'), [](){
        pop.showCreature(0);
    }));
    userFunctions->push_back(UserFunction(new UIchar('O'), [](){
        pop.updateViewingGenomes();
    }));
    userFunctions->push_back(UserFunction(new UIchar('[', 150), [](){
        pop.prevCreature();
    }));
    userFunctions->push_back(UserFunction(new UIchar(' '), [](){
        pop.resetCreature();
    }));
    userFunctions->push_back(UserFunction(new UIchar(']', 150), [](){
        pop.nextCreature();
    }));
    userFunctions->push_back(UserFunction(new UIchar(' '), [](){
        pop.history.writeToConsole();
    }));
    userFunctions->push_back(UserFunction(new UIchar('>'), [](){
        numThreads += numThreads < MAX_THREADS ? 1 : 0;
    }));
    userFunctions->push_back(UserFunction(new UIchar('<'), [](){
        numThreads -= numThreads > MIN_THREADS ? 1 : 0;
    }));
    userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_PAGE_UP, 150), [](){
        gameSpeed += gameSpeed < MAX_GAME_SPEED ? 1 : 0;
    }));
    userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_PAGE_DOWN, 150), [](){
        gameSpeed -= gameSpeed > MIN_GAME_SPEED ? 1 : 0;
    }));
    userFunctions->push_back(UserFunction(new UIchar('v'), [](){
        Phylogeny::scientificNames ^= true;
    }));
    userFunctions->push_back(UserFunction(new UIchar('c'), [](){
        cinematic ^= true;
    }));

    userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>("COM Trail", &drawCOMTrails, 0.9, 0.9, 1.0, 1, by_position()), [userFunctions](){

    }));
    userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>("Node Trails", &drawNodeTrails, 0.9, 0.8, 1.0, 0.9, by_position()), [userFunctions](){

    }));

    userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>("Graph", &drawGraph, 0.0, 0.9, 0.2, 1, by_position()), [userFunctions](){
        if (drawGraph) {
    //            userFunctions->push_back(UserFunction(new Button<Appearance::GREEN>("DrawGraph", 0.9, 0.1, 0.95, 0.15, by_position(), secondaryButtonIds), [](){
//            }));
            for (unsigned int i = 0; i < userFunctions->size(); i++) { ///< @todo Make this clearing more general. Infact, make radio button class. Or maybe on call this on release
                if ((*userFunctions)[i].element->id == 2) {
                    userFunctions->erase((*userFunctions).begin() + i);
                }
            }
        }
        drawBrain = false;
    }));
    userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>("Brain", &drawBrain, 0.2, 0.9, 0.4, 1, by_position()), [userFunctions](){
        int secondaryButtonIds = 2;
        if (drawBrain) {
            userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>("Draw Axons", &drawLines, 0.9, 0.1, 0.97, 0.15, by_position(), secondaryButtonIds), [](){

            }));
            for (unsigned int i = 0; i < userFunctions->size(); i++) {
                if ((*userFunctions)[i].element->id == 1) {
                    userFunctions->erase((*userFunctions).begin() + i);
                }
            }
        } else {
            for (unsigned int i = 0; i < userFunctions->size(); i++) {
                if ((*userFunctions)[i].element->id == secondaryButtonIds) {
                    userFunctions->erase((*userFunctions).begin() + i);
                }
            }
        }
        drawGraph = false;
    }));



    /* Camera Controls */
    userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_LEFT, -1),
        [camera](){camera->del.x = +camera->DEFAULT_R_SPEED; },
        [camera](){camera->del.x = 0.0;
    }));
    userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_RIGHT, -1),
        [camera](){camera->del.x = -camera->DEFAULT_R_SPEED; },
        [camera](){camera->del.x = 0.0;
    }));
    userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_UP, -1),
        [camera](){camera->del.z = +camera->DEFAULT_R_SPEED; },
        [camera](){camera->del.z = 0.0;
    }));
    userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_DOWN, -1),
        [camera](){camera->del.z = -camera->DEFAULT_R_SPEED; },
        [camera](){camera->del.z = 0.0;
    }));

    /* Position Controls */
    userFunctions->push_back(UserFunction(new UIchar('w', -1),
        [camera](){camera->mov.x =  camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.x = 0;}
    ));
    userFunctions->push_back(UserFunction(new UIchar('s', -1),
        [camera](){camera->mov.x = -camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.x = 0;}
    ));
    userFunctions->push_back(UserFunction(new UIchar('a', -1),
        [camera](){camera->mov.y = -camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.y = 0;}
    ));
    userFunctions->push_back(UserFunction(new UIchar('d', -1),
        [camera](){camera->mov.y =  camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.y = 0;}
    ));
    userFunctions->push_back(UserFunction(new UIchar('e', -1),
        [camera](){camera->mov.z =  camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.z = 0;}
    ));
    userFunctions->push_back(UserFunction(new UIchar('z', -1),
        [camera](){camera->mov.z = -camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.z = 0;}
    ));
}


