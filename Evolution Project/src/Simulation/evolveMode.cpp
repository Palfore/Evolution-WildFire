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

static Population pop(1000);
static int gameSpeed = 1;

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

    DrawSkybox(700);

    auto tree = Vec(-50, 50, 0);
    DrawCylinder<Appearance::BARK>(tree, tree + Vec(0,0,50), 8);
    DrawSphere<Appearance::TREE_TOP>(tree + Vec(0,0,50), 25);

    pop.draw();

    DrawString<Appearance::WHITE>("Frank F"+ utility::numToStr<int>(pop.getActiveCreature().getFitness()), pop.getActiveCreature().getTop());
    DrawString<Appearance::BLACK>("Generation " + utility::numToStr<int>(pop.gen), 0.01, 0.03, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("Creature: " + utility::numToStr<int>(pop.getMemberIndex()) +
                                " / " + utility::numToStr<int>(pop.population.size()), 0.01, 0.05, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("Time " + utility::numToStr<int>(pop.getSimStep()), 0.01, 0.07, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("NumThreads " + utility::numToStr<int>(numThreads), 0.01, 0.09, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("GameSpeed " + utility::numToStr<int>(gameSpeed), 0.01, 0.11, by_percentage(), false, false);
}

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
    }


    if (0) goto postThreading;

    if (mt.empty()) { // initialize
        for (unsigned int i = 0; i < numThreads; i++) {
            mt.push_back(new MultiThread());
        }
        MultiThread::spawnChildren(mt, pop.getCreatures());
        startTime = std::clock();
    }

    if (all_of(mt.cbegin(), mt.cend(), [](const MultiThread * m){return m->isFinished();})) { // done generation
        double finishedTime = (std::clock() - startTime) / (double)(CLOCKS_PER_SEC);
        times.push_back(finishedTime);
        double average = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        startTime = std::clock();

        pop.getThreadedFitnesses(mt);
        pop.sortPop(); // These three can be combined
        pop.recordHistory();
        pop.select();
        pop.mutate();
        pop.gen++;

        /// Start Next Generation
//        std::cout<< "Gen: " << gen << " <f>: " << pop.getAvg() << " max(f): " << pop.population[0]->fitness << " in [" << finishedTime << "s (" << average << "s)] "
//                                        << "ProcessingTime: " << (std::clock() - startTime) / (double)(CLOCKS_PER_SEC) << "s\n" ;

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
//        goto d; // End early on this iteration because its expensive to spawn threads.
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


