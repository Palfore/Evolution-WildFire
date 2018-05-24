#include "Simulation.h"
#include "GFramework.h"
#include "Drawing/Shapes.h"
#include "Drawing/Text.h"
#include "Drawing/Objects.h"
#include "Logger.h"
#include "Audio.h"
#include "utility.h" // numToStr
#include <iostream>


Simulation::Simulation() : gameMode(INITIAL_GAME_MODE), inputType(INITIAL_INPUT_TYPE) {LOG("Initialized Simulation.");}
Simulation::~Simulation() {}

#include <thread>
#include <functional>
#include "Creature.h"
#include <ctime>
#include <iostream>

#include "Genome.h"
#include "Population.h"
#include "NodeGene.h"

class MultiThread {
    public:
        std::vector<double> fitnesses;
        MultiThread() : fitnesses({}), t(), finished(false) {}

        void spawn(std::function<void(std::vector<Creature>, std::vector<double> &, bool &)> f, std::vector<Creature> creatures) {
            finished = false;
            fitnesses.clear();
            t = std::thread(f, creatures, std::ref(this->fitnesses), std::ref(this->finished));
            t.detach(); // need to return values & reinstansiate thread.
        }

        bool isFinished() const {
            return finished;
        }

        static void processCreatures(std::vector<Creature> creatures, std::vector<double> & fitnesses, bool & done) {
            for (auto creature: creatures) {
                double initialFitness = 0;
                for (int i = 0; i < 9000; i++) {
                    initialFitness = i == 1000 ? creature.getFitness() : initialFitness;
                    creature.update(i);
                }
                fitnesses.push_back(creature.getFitness() - initialFitness);
            }
            done = true;
        }

        static void spawnChildren(std::vector<MultiThread*> mt, int numThreads, std::vector<Creature> creatures) {
            auto begining = creatures.cbegin();
            const int blockSize = creatures.size() / numThreads;
            for (int i = 0; i < numThreads; i++) {
                auto e1 = begining + i     * blockSize;
                auto e2 = begining + (i+1) * blockSize;
                if (i == numThreads - 1) e2 = creatures.cend();
                mt[i]->spawn(processCreatures, std::vector<Creature>(e1, e2));
            }
        }

    private:
        std::thread t;
        bool finished;
};

#include "Shapes.h"
void Simulation::run(std::vector<UserFunction> * userFunctions, const double fps) {
    if (inputType != InputType::DEFAULT) {
        goto showMessages;
    }

    ///////////////////////////////////////////////// "Actual Simulation"
    static int numThreads = std::thread::hardware_concurrency() - 2 > 0 ? std::thread::hardware_concurrency() - 2 : 1;
    static Population<Genome> pop(1000);
    static std::vector<MultiThread*> mt;
    static int gen = 0;

    if (mt.empty()) { // initialize
        for (int i = 0; i < numThreads; i++) {
            mt.push_back(new MultiThread());
        }
        MultiThread::spawnChildren(mt, numThreads, pop.getCreatures());
    }

    if (all_of(mt.cbegin(), mt.cend(), [](MultiThread * m){return m->isFinished();})) { // done generation
        /// Get Fitnesses
        int c = 0;
        for (unsigned int i = 0; i < mt.size(); i++) {
            for (unsigned int j = 0; j < mt[i]->fitnesses.size(); j++) {
                pop.population[c++]->fitness = mt[i]->fitnesses[j];
            }
        }

        /// Sort Population
        std::sort(pop.population.begin(), pop.population.end(), [](Genome* a, Genome* b){
            return a->fitness > b->fitness; // '<' ==> worst to best
        });

        /// Select Population
        for (unsigned int i = 0; i < pop.population.size() / 2; i++) {
            delete pop.population[i + pop.population.size() / 2]; // Use smart pointers next time
            pop.population[i + pop.population.size() / 2] = new Genome(*pop.population[i]);
        }

        /// Mutate Population
        for (unsigned int i = 0; i < pop.population.size(); i++) {
            for (auto * gene: pop.population[i]->getGenes<NodeGene>()) {
                gene->mutate(*pop.population[i]);
            }
        }

        /// Start Next Generation
        std::cout<< "Finished Generation " << gen << " With average fitness " << pop.getAvg() <<
         " and max fitness of " << pop.population[0]->fitness << '\n' ;
        gen++;
        MultiThread::spawnChildren(mt, numThreads, pop.getCreatures());
        pop.showCreature(0);
    }

    DrawPlane<Appearance::GRASS>(100);
    pop.draw();
    pop.nextStep();


    DrawString<Appearance::BLACK>("Generation " + utility::numToStr<int>(gen), 0.01, 0.03, false, false);
    DrawString<Appearance::BLACK>("Population Size " + utility::numToStr<int>(pop.population.size()), 0.01, 0.05, false, false);





    //----------------------------------------------- Drawing


    ///////////////////////////////////////////////// "Other"
    /* Normal Procedure */
//    DrawPlane<Appearance::GRASS>(100);
//
//
//    DrawString<Appearance::WHITE>("Type something, then hit enter", 0.5,  0.4);
//    DrawString<Appearance::WHITE>(utility::numToStr<int>(fps), 0.1,  0.1);
//
//    /* Start and stop getting user input */
//    UserInput *userInput = &GFramework::get->userInput;
//    if (inputType == InputType::DEFAULT) {
//        if (userInput->inputString.empty()) {
//            setInputType(InputType::ALPHA_NUMERIC_INPUT);
//        } else if (userInput->isInputStringSubmitted()) {
//            userInput->drawUserString(0.5, 0.5);
//            DrawString<Appearance::WHITE>("Now you can move around", 0.5,  0.6);
//        }
//    }
//
//    /* Updates based on user input */
//    if (inputType == InputType::ALPHA_NUMERIC_INPUT) {
//        DrawString<Appearance::WHITE>(userInput->inputString, 0.5,  0.5);
//        if (userInput->isInputStringSubmitted()) {
//            setInputType(InputType::DEFAULT);
//            GFramework::get->audio->playSound("gunShot.wav");
//        }
//    }
//

    showMessages:
    if (inputType == InputType::FATAL_MESSAGE) {
        DrawRectangle<Appearance::BUTTON>(0.2, 0.2, 0.8, 0.8);
        DrawString<Appearance::BLACK>("A fatal error occured. Press ENTER to quit.", 0.5, 0.5);
    }
    if (inputType == InputType::BLOCKING_MESSAGE) {
        DrawRectangle<Appearance::BUTTON>(0.2, 0.2, 0.8, 0.8);
        DrawString<Appearance::BLACK>("A warning has been logged. Press ENTER to continue.", 0.5, 0.5);
    }


    GFramework::get->showScene();
    GFramework::get->readyDrawing();
}

void Simulation::init() {
    Simulation::loadGameModeKeyboard();
}

void Simulation::setGameMode(GameMode g) {
    gameMode = g;
    inputType = InputType::DEFAULT;
    loadGameModeKeyboard();
}

void Simulation::setInputType(InputType t) {
    inputType = t;
    setInputTypeKeyboard();
}


void Simulation::setInputTypeKeyboard() {
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;
    GFramework::get->userInput.setToDefault();
    switch(inputType) {
        case InputType::DEFAULT:
            loadGameModeKeyboard();
            break;
        case InputType::BLOCKING_MESSAGE:
            userFunctions->push_back(UserFunction(ENTER, [this](){
                this->setInputType(InputType::DEFAULT); // Unlock Input
            }));
            break;
        case InputType::FATAL_MESSAGE:
            userFunctions->push_back(UserFunction(ENTER, [](){ // Fatal ending to game
                exit(EXIT_FAILURE);
            }));
            break;
        case InputType::NUMERIC_INPUT:
            GFramework::get->userInput.keyInputIsHeld = false;
            for (char input = '0'; input <= '9'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
            }
            userFunctions->push_back(UserFunction(BACKSPACE, [](){
                if (!GFramework::get->userInput.inputString.empty()) {
                    GFramework::get->userInput.inputString.pop_back();
                }
            }));
            userFunctions->push_back(UserFunction(ENTER, [](){
                GFramework::get->userInput.submitInputString();
                GFramework::get->userInput.keyInputIsHeld = true;
            }));
            break;
        case InputType::ALPHA_NUMERIC_INPUT:
            GFramework::get->userInput.keyInputIsHeld = false;

            for (char input = '0'; input <= '9'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
            }
            for (char input = 'a'; input <= 'z'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
            }
            for (char input = 'A'; input <= 'Z'; input++) {
                userFunctions->push_back(UserFunction(input, [input](){GFramework::get->userInput.inputString += input;}));
            }
            userFunctions->push_back(UserFunction(' ', [](){GFramework::get->userInput.inputString += ' ';}));
            userFunctions->push_back(UserFunction(BACKSPACE, [](){
                if (GFramework::get->userInput.inputString.size() > 0) {
                    GFramework::get->userInput.inputString.pop_back();
                }
            }));
            userFunctions->push_back(UserFunction(ENTER, [](){
                GFramework::get->userInput.submitInputString();
                GFramework::get->userInput.keyInputIsHeld = true;
            }));
            break;
        default:
            LOG(UNKNOWN_INPUT_TYPE_MESSAGE, LogDegree::WARNING, LogType::INPUT);
            break;
    }
}

void Simulation::loadGameModeKeyboard() {
    GFramework::get->userInput.setToDefault();
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;
    switch(gameMode) {
        case GameMode::MAIN_MENU:
            break;
        case GameMode::SIMULATION: {
            Camera *camera = GFramework::get->camera;

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
            break;
        }
        default:
            LOG(UNKNOWN_GAME_MODE_MESSAGE, LogDegree::WARNING, LogType::INPUT);
            break;
    }
}



/// Timminf threadcounts
//static std::vector<Creature> creatures = {};
//    static std::vector<MultiThread*> mt;
//
//    int initThreads = 1;
//    int maxThreads = 8;
//    int initCreatures = 100;
//    int creatureIncrement = 100;
//
//    static int numThreads = initThreads;
//    static int numCreatures = initCreatures;
//    static std::clock_t start;
//
//
//    if (creatures.empty()) {
//        for (int i = 0; i < numCreatures; i++) {
//            creatures.push_back(Creature(5,6));
//        }
//    }
//
//    if (mt.empty()) { // initialize
//        for (int i = 0; i < numThreads; i++) {
//            mt.push_back(new MultiThread());
//            int e1 = i     * (numCreatures / numThreads); // must divide equally.
//            int e2 = (i+1) * (numCreatures / numThreads) - 1;
//            mt[i]->spawn(processCreatures, std::vector<Creature>(creatures.begin() + e1, creatures.begin() + e2));
//        }
//        start = std::clock();
//    }
//
//
//    if (all_of(mt.cbegin(), mt.cend(), [](MultiThread * m){return m->isFinished();})) {
//        double excecutionTime = (std::clock() - start) / (double)(CLOCKS_PER_SEC);
//
//        if (numThreads == initThreads) {
//            std::cout << numCreatures << " ";
//        }
//        std::cout << excecutionTime << " ";
//        if (numThreads == maxThreads) {
//            numThreads = initThreads - 1;
//            numCreatures += creatureIncrement;
//            std::cout << '\n';
//        }
//
//        numThreads++;
//        mt = {};
//        creatures = {};
//
//    }
