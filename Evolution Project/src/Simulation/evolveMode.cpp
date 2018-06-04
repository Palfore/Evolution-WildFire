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

static Population pop(200);
static int gen = 0;

static int numThreads = std::thread::hardware_concurrency() - 2 > 0 ? std::thread::hardware_concurrency() - 2 : 1;
static std::vector<MultiThread*> mt;

static std::clock_t startTime;
static std::vector<double> times = {};


static void drawing() {
    DrawPlane<Appearance::GRASS>(100);
    DrawSkybox(500);
    pop.draw();
    pop.nextStep();


    DrawString<Appearance::BLACK>("Generation " + utility::numToStr<int>(gen), 0.01, 0.03, false, false);
    DrawString<Appearance::BLACK>("Creature: " + utility::numToStr<int>(pop.getMemberIndex()) +
                                " / " + utility::numToStr<int>(pop.population.size()), 0.01, 0.05, false, false);
}

void Simulation::evolveMode() {

    static bool initialized = true;
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;
    if (initialized) {
        delete GFramework::get->camera;
        GFramework::get->camera= new Camera(); // reset camera

        initialized = false;
        userFunctions->push_back(UserFunction('p', [](){
            pop.printCurrentGenome();
            GLOBAL->userInput.keyStates['p'] = false; // prevent multiple calls
        }));
        userFunctions->push_back(UserFunction('o', [](){
            pop.showCreature(0);
            GLOBAL->userInput.keyStates['o'] = false; // prevent multiple calls
        }));
        userFunctions->push_back(UserFunction('>', [](){
            static auto last_called = std::clock();
            if (last_called + 500 < std::clock()) { // If held, repeat every 0.5s This isnt flawless (yet)
                GLOBAL->userInput.keyStates['>'] = false;

                pop.population[pop.getMemberIndex()]->mutate();
                pop.showCreature(pop.getMemberIndex());

                last_called = std::clock();
            }
            GLOBAL->userInput.keyStates['>'] = true; // prevent multiple calls
        }));
        userFunctions->push_back(UserFunction('i', [](){
            pop.mutate();
            GLOBAL->userInput.keyStates['i'] = false; // prevent multiple calls
        }));

    }


    goto postThreading;

    if (mt.empty()) { // initialize
        for (int i = 0; i < numThreads; i++) {
            mt.push_back(new MultiThread());
        }
        MultiThread::spawnChildren(mt, numThreads, pop.getCreatures());
        startTime = std::clock();
    }

    if (all_of(mt.cbegin(), mt.cend(), [](MultiThread * m){return m->isFinished();})) { // done generation
        double finishedTime = (std::clock() - startTime) / (double)(CLOCKS_PER_SEC);
        times.push_back(finishedTime);
        double average = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        startTime = std::clock();

        pop.getThreadedFitnesses(mt);
        pop.sortPop();
        pop.select();
        pop.mutate();

        /// Start Next Generation
        std::cout<< "Gen: " << gen << " <f>: " << pop.getAvg() << " max(f): " << pop.population[0]->fitness << " in [" << finishedTime << "s (" << average << "s)] / "
                                        << (std::clock() - startTime) / (double)(CLOCKS_PER_SEC) << "s\n" ;
        gen++;
        MultiThread::spawnChildren(mt, numThreads, pop.getCreatures());
        pop.showCreature(0);

        startTime = std::clock();

        return; // End early on this iteration because its expensive to spawn threads.
    }

postThreading:
    drawing();
}



void Simulation::loadEvolve() {
    Camera *camera = GLOBAL->camera;
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;
    /* Setting Controls */
    userFunctions->push_back(UserFunction(GLUT_KEY_F1, [](){GFramework::get->display ^= true;}));

    /* Camera Controls */
    userFunctions->push_back(UserFunction(GLUT_KEY_LEFT,  [camera](){
        camera->del.x = +camera->DEFAULT_R_SPEED; }, [camera](){camera->del.x = 0.0;
    }));
    userFunctions->push_back(UserFunction(GLUT_KEY_RIGHT, [camera](){
        camera->del.x = -camera->DEFAULT_R_SPEED; }, [camera](){camera->del.x = 0.0;
    }));
    userFunctions->push_back(UserFunction(GLUT_KEY_UP,    [camera](){
        camera->del.z = +camera->DEFAULT_R_SPEED; }, [camera](){camera->del.z = 0.0;
    }));
    userFunctions->push_back(UserFunction(GLUT_KEY_DOWN,  [camera](){
        camera->del.z = -camera->DEFAULT_R_SPEED; }, [camera](){camera->del.z = 0.0;
    }));

    /* Position Controls */
    userFunctions->push_back(UserFunction('w', [camera](){camera->mov.x =  camera->DEFAULT_T_SPEED;}));
    userFunctions->push_back(UserFunction('s', [camera](){camera->mov.x = -camera->DEFAULT_T_SPEED;}));
    userFunctions->push_back(UserFunction('a', [camera](){camera->mov.y = -camera->DEFAULT_T_SPEED;}));
    userFunctions->push_back(UserFunction('d', [camera](){camera->mov.y =  camera->DEFAULT_T_SPEED;}));
    userFunctions->push_back(UserFunction('e', [camera](){camera->mov.z =  camera->DEFAULT_T_SPEED;}));
    userFunctions->push_back(UserFunction('z', [camera](){camera->mov.z = -camera->DEFAULT_T_SPEED;}));
}


