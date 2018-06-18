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

static Population pop(300);
static int gameSpeed = 6;
static bool drawGraph = false;
static bool drawDistance = false;
static bool drawBrain = true;
static bool drawLines = false;


static unsigned int numThreads = std::thread::hardware_concurrency() - 2 > 0 ? std::thread::hardware_concurrency() - 2 : 1;
static std::vector<MultiThread*> mt;

static std::clock_t startTime;
static std::vector<double> times = {};

static void drawing() {
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


    DrawSphere<Appearance::BLACK>(Vec(pop.getActiveCreature().moveTo), 2.5);
    const double a = acosh(10) / (pop.getActiveCreature().moveTo.x + 0.3); // This scaling is probably wrong, because of (moveTo.x = 0)

    double X = pop.getActiveCreature().getCOM().x;
    for (double x = -X-10; x < X +10; x += 0.3) {
        const double f = 50* tanh(0.15*(x - pop.getActiveCreature().moveTo.z)); //should be +/-(something) depending on direction => tanh
        if (f > 0) {
            DrawSphere<Appearance::SKY_BLUE>(Vec(x, 0, fabs(f)), 0.3);
        } else {
            DrawSphere<Appearance::RED>(Vec(x, 0, fabs(f)), 0.3);
        }
    }


    DrawSkybox(700);

    auto tree = Vec(-50, 50, 0);
    DrawCylinder<Appearance::BARK>(tree, tree + Vec(0,0,50), 8);
    DrawSphere<Appearance::TREE_TOP>(tree + Vec(0,0,50), 25);

    pop.draw();
    /////////////////////////////////////////// 2D Drawing ///////////////////////////////////////////////////

    DrawString<Appearance::WHITE>("Frank F"+ utility::numToStr<int>(pop.getActiveCreature().getFitness()), pop.getActiveCreature().getTop());
    DrawString<Appearance::BLACK>("Generation " + utility::numToStr<int>(pop.gen), 0.01, 0.03, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("Creature: " + utility::numToStr<int>(pop.getMemberIndex()) +
                                " / " + utility::numToStr<int>(pop.population.size()), 0.01, 0.05, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("Time " + utility::numToStr<int>(pop.getSimStep()), 0.01, 0.07, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("NumThreads " + utility::numToStr<int>(numThreads), 0.01, 0.09, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("GameSpeed " + utility::numToStr<int>(gameSpeed), 0.01, 0.11, by_percentage(), false, false);

    if (drawDistance) {

    }
    if (drawGraph) {
        pop.history.graph();
    }
    if (drawBrain) {
        pop.getActiveCreature().drawBrain(drawLines);
    }

}
#include "MyGlut.h"
void Simulation::evolveMode() {

    static bool initialized = true;
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;
    if (initialized) {
        delete GFramework::get->camera;
        GFramework::get->camera= new Camera(); // reset camera

        initialized = false;
        userFunctions->push_back(UserFunction(new UIchar('p'), [](){
            pop.printCurrentGenome();
        }));
        userFunctions->push_back(UserFunction(new UIchar('o'), [](){
            pop.showCreature(0);
        }));
        userFunctions->push_back(UserFunction(new UIchar('O'), [](){
            pop.updateViewingGenomes();
        }));
        userFunctions->push_back(UserFunction(new UIchar('[', 300), [](){
            pop.prevCreature();
        }));
        userFunctions->push_back(UserFunction(new UIchar(' '), [](){
            pop.resetCreature();
        }));
        userFunctions->push_back(UserFunction(new UIchar(']', 300), [](){
            pop.nextCreature();
        }));
        userFunctions->push_back(UserFunction(new UIchar(' '), [](){
            pop.history.writeToConsole();
        }));
        userFunctions->push_back(UserFunction(new UIchar('>'), [](){
            numThreads += (unsigned) numThreads < std::thread::hardware_concurrency() ? 1 : 0;
        }));
        userFunctions->push_back(UserFunction(new UIchar('<'), [](){
            numThreads -= numThreads > 1 ? 1 : 0;
        }));
        userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_PAGE_UP, 150), [](){
            gameSpeed++;
        }));
        userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_PAGE_DOWN, 150), [](){
            gameSpeed -= gameSpeed >= 0 ? 1 : 0;
        }));

        userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>("Graph", &drawGraph, 0.0, 0.9, 0.2, 1, by_position()), [userFunctions](){
            int secondaryButtonIds = 1;
            if (drawGraph) {
                userFunctions->push_back(UserFunction(new Button<Appearance::GREEN>("DrawGraph", 0.9, 0.1, 0.95, 0.15, by_position(), secondaryButtonIds), [](){

                }));
                for (unsigned int i = 0; i < userFunctions->size(); i++) {
                    if ((*userFunctions)[i].element->id == 2) {
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
    }


    if (0) goto postThreading;

    if (numThreads > 0 && mt.empty()) { // initialize
        for (unsigned int i = 0; i < numThreads; i++) {
            mt.push_back(new MultiThread());
        }

        try {
            MultiThread::spawnChildren(mt, pop.getCreatures());
        } catch (std::bad_alloc const& ext) {
            // With ~10000 complex creatures, memory usage spikes to over 2GB, causing a crash.
            LOG("There is not enough memory to spawn new threads.", LogDegree::FATAL);
        }
        startTime = std::clock();
    }

    if (numThreads > 0 && all_of(mt.cbegin(), mt.cend(), [](const MultiThread * m){return m->isFinished();})) { // done generation
        double finishedTime = (std::clock() - startTime) / (double)(CLOCKS_PER_SEC);
        times.push_back(finishedTime);
        double average = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        startTime = std::clock();

        pop.getThreadedFitnesses(mt);
        pop.sortPop();
        pop.recordHistory();
        pop.select(); // These two can be combined
        pop.mutate();
        pop.gen++;

        /// Start Next Generation
        std::cout<< "Gen: " << pop.gen << " <f>: " << pop.getAvg() << " max(f): " << pop.population[0]->fitness << " in [" << finishedTime << "s (" << average << "s)] "
                                        << "ProcessingTime: " << (std::clock() - startTime) / (double)(CLOCKS_PER_SEC) << "s\n" ;

        if (mt.size() != numThreads) {
            for (auto &p: mt) {
                delete p;
            }
            mt.clear();
            for (unsigned int i = 0; i < numThreads; i++) {
                mt.push_back(new MultiThread());
            }
        }
        MultiThread::spawnChildren(mt, pop.getCreatures()); /// @todo Multi-threaded spawning is a bit laggy and noticable.
        startTime = std::clock();
        if (0) goto d; // End early on this iteration because its expensive to spawn threads.
    }

postThreading:
    for (int i = 0; i < gameSpeed; i++) {
        pop.nextStep();
    }
d:
    drawing();
}



void Simulation::loadEvolve() {
    Camera *camera = GLOBAL->camera;
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;

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


