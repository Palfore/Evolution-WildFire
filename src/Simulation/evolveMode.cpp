#include <math.h>
#include <thread>

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
#include "MyMath.h"
#include "Creature.h"
#include "Viewer.h"


#include "OmniWalker.h"
#include "StickBall.h"
#include "Vec2.h"
#include "Factory.h"
#include "Terrain.h"
#include "ScenarioFactory.h"

#include "MultiThread.h"

static constexpr int populationSize = 1*256;

static bool drawGraph = false;
static bool drawCOMTrails = true;
static bool drawDebug = false;
static bool drawBrain = false;
static bool drawLines = false;

static constexpr int MAX_GAME_SPEED = 20;
static constexpr int MIN_GAME_SPEED = 0;  // No updating displayed creatures
static constexpr int BACKGROUND_THREADS_REQUIRED = 2;
static constexpr int MIN_THREADS = 0;  // No processing threads
static const     int MAX_THREADS = std::thread::hardware_concurrency();
static const     int DEFAULT_THREADS = (MAX_THREADS - BACKGROUND_THREADS_REQUIRED) > 0 ?
                                             MAX_THREADS - BACKGROUND_THREADS_REQUIRED : 1;
// static int numThreads = 0;
static int numThreads = DEFAULT_THREADS;

static const Terrain terrain = Terrain();
static const Factory creatureFactory("EyeWalker");
static const ScenarioFactory scenarioFactory("ScenarioA", terrain, 15'000/2);
static Population pop(populationSize, creatureFactory);
static Viewer viewer(pop.population, creatureFactory, scenarioFactory);
static Threader threader(0, scenarioFactory, pop.getBodies());


static void drawing(double fps, bool cinematic, int gameSpeed) {
    const Creature& c = viewer.getCurrent();

    /* Environment */
    DrawSkybox(1000);
    terrain.draw();


    /* Single Tree */
    double height = 100;
    double size = 50;
    auto tree = Vec(-100, 100, terrain.getHeight(-100, 100));
    DrawCylinder<Appearance::BARK>(tree, tree + Vec(0, 0, height), 8);
    DrawSphere<Appearance::TREE_TOP>(tree + Vec(0, 0, height), size);

    /* Creature Details */
    DrawSphere<Appearance::LIGHT_GREEN>(
        Vec(c.moveTo.x,
            c.moveTo.y,
            c.moveTo.z),
        5);

    viewer.draw();
    viewer.drawTrails(drawCOMTrails);

    /////////////////////////////////////////// 2D Drawing /////////////////////////////////////////
    /* Overlays */
    if (drawGraph) {
        pop.history.graph();
    }
    if (drawBrain) {
        c.drawBrain(drawLines);
    }

    if (cinematic) return;
    DrawString<Appearance::WHITE>("Frank F"+ utility::numToStr<double>(viewer.fitness), c.getTop(20));
    DrawString<Appearance::BLACK>("Generation " + utility::numToStr<int>(pop.gen),
                                 0.01, 0.03, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("Creature: " + utility::numToStr<int>(viewer.getMemberIndex()) +
                                " / " + utility::numToStr<int>(pop.population.size()) +
                                " (" + utility::numToStr<int>(pop.population.size()) + ')',
                                0.01, 0.05, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("Time " + utility::numToStr<int>(viewer.getSimStep()),
                                0.01, 0.07, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("NumThreads " + utility::numToStr<int>(numThreads) + " (" +
                                utility::numToStr<int>(threader.size()) + ")",
                                0.01, 0.09, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("GameSpeed " + utility::numToStr<int>(gameSpeed),
                                 0.01, 0.11, by_percentage(), false, false);

    DrawString<Appearance::BLACK>("NumUserFuncs: " +
                                utility::numToStr<int>(GFramework::get->userInput.functions.size()),
                                0.01, 0.13, by_percentage(), false, false);
    DrawString<Appearance::BLACK>("FPS: " + utility::numToStr<double>(fps),
                                0.01, 0.15, by_percentage(), false, false);
    double inc = 0.02;
    double place = 0.15 + inc;

    for (auto const& name : viewer.phylogeny.getPhylogenyDict()) {
        DrawString<Appearance::BLACK>(name[0] + ": " +  name[1],
            0.01, place+=inc, by_percentage(), false, false);
    }

    c.drawDebug(drawDebug);


}

void Simulation::evolveMode(double fps) {
    static int processingGeneration = 0;
    bool throttleFPS = true;

    if (this->cinematic) {
        GFramework::get->camera->cinematicCamera(Vec(
            viewer.getCurrent().getCOM().x,
            viewer.getCurrent().getCOM().y,
            20));
    }

    /* Switch cases are used to delay frames to decrease lag spikes due to
        the following expensive operations. */
    switch (processingGeneration) {
        case 10: pop.sortPop(); break;
        case 9:  pop.recordHistory(); break;
        case 8:  break;
        case 7:  pop.select(); break;
        case 6:  break;
        case 5:  pop.mutate(); pop.gen++; break;
        case 4:  break;
        case 3:
            if (pop.gen == 40) {
                numThreads = 0;
                // pop.history.writeToFile("data.out");
                // std::cout << "Finished!\n";
                // exit(0);
            }
            break;
        case 2:  break;
        case 1:
            threader.spawnThreads(pop.getBodies());
            throttleFPS = false;
            break;
        default: break;
    }

    processingGeneration = std::max(0, processingGeneration);
    if (!processingGeneration-- && threader.canProcess()) {
        pop.updateFitnesses(threader.getFitnesses(pop.population.size()));
        processingGeneration = 10;
    }
    threader.update(numThreads);
    for (int i = 0; i < gameSpeed; i++) {
        viewer.update();
    }
    if (frame == 0) {
        drawing(fps, cinematic, gameSpeed);
    }

    static auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elaspedTime = std::chrono::duration<double>(endTime - startTime).count();
    startTime = endTime;
    if (throttleFPS) {
        double FPS = 50.0;
        int x = (1.0/FPS - elaspedTime) * 1'000;
        std::this_thread::sleep_for(std::chrono::milliseconds(x));
    }
}


#include "Bisection.h"
void Simulation::loadEvolve() {
    GFramework::get->camera->reset();
    Camera *camera = GLOBAL->camera;
    auto *userFunctions = &GFramework::get->userInput.functions;

    /* Game Controls */
    userFunctions->push_back(UserFunction(new UIchar('p'), [](){
        viewer.printCurrentGenome();
    }));
    userFunctions->push_back(UserFunction(new WindowPress(), [userFunctions](){
        for (const auto& f : *userFunctions) {
            if (f.element->isClickable()) {
                if (f.element->callFunction == CALL_TYPE::ACTION) {
                    return;
                }
            }
        }
        GLdouble modelview[16];
        GLdouble projection[16];
        GLint viewport[4];

        Drawing::enable3D(true);
            glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
            glGetDoublev(GL_PROJECTION_MATRIX, projection);
            glGetIntegerv(GL_VIEWPORT, viewport);
        Drawing::enable2D();

        auto mouse = GFramework::get->mouse;
        double height = GFramework::get->windowSize.y;
        GLdouble x0, y0, z0;
        GLdouble x1, y1, z1;
        gluUnProject(
            mouse.x, height-mouse.y, 0,
            modelview, projection, viewport,
            &x0, &y0, &z0);
        gluUnProject(
            mouse.x, height-mouse.y, 1,
            modelview, projection, viewport,
            &x1, &y1, &z1);

        Vec u0 = Vec(x0, y0, z0);
        Vec u1 = Vec(x1, y1, z1);
        Vec u  = (u1 - u0).getUnit();
        double k = findRoot(
            [u0, u1, u](double r) {
                const Vec p = u0 + r*(u1 - u0);
                return p.z - viewer.getCurrent().terrain.getHeight(p.x, p.y);
            }, 0, 1
        );

        Vec s = u0 + (u1 - u0)*k;
        std::cout<<s<<'\n';
        viewer.moveTo(s);
    }));
    userFunctions->push_back(UserFunction(new UIchar('o'), [](){
        viewer.showCreature(0);
    }));
    userFunctions->push_back(UserFunction(new UIchar('b'), [](){
        GFramework::get->camera->reset();
    }));
    userFunctions->push_back(UserFunction(new UIchar('O'), [](){
        viewer.updateViewingGenomes(pop.population);
    }));
    userFunctions->push_back(UserFunction(new UIchar('[', 150), [](){
        viewer.prevCreature();
    }));
    userFunctions->push_back(UserFunction(new UIchar(' '), [](){
        viewer.resetCreature();
    }));
    userFunctions->push_back(UserFunction(new UIchar(']', 150), [](){
        viewer.nextCreature();
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
    userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_PAGE_UP, 150), [this](){
        gameSpeed += gameSpeed < MAX_GAME_SPEED ? 1 : 0;
    }));
    userFunctions->push_back(UserFunction(new UIchar(GLUT_KEY_PAGE_DOWN, 150), [this](){
        gameSpeed -= gameSpeed > MIN_GAME_SPEED ? 1 : 0;
    }));
    userFunctions->push_back(UserFunction(new UIchar('v'), [](){
        Phylogeny::scientificNames ^= true;
    }));
    userFunctions->push_back(UserFunction(new UIchar('c'), [this](){
        this->cinematic ^= true;
    }));

    userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>(
        "COM Trail", &drawCOMTrails, 0.9, 0.9, 1.0, 1, by_position()), [userFunctions](){
    }));

    userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>(
        "Graph", &drawGraph, 0.0, 0.9, 0.2, 1, by_position()), [userFunctions](){
            if (drawGraph) {
                // userFunctions->push_back(UserFunction(new Button<Appearance::GREEN>("DrawGraph",
                //     0.9, 0.1, 0.95, 0.15, by_position(), secondaryButtonIds), [](){
                // }));

                // Make this clearing more general. Infact, make radio button class.
                // Or maybe on call this on release
                for (unsigned int i = 0; i < userFunctions->size(); i++) {
                    if ((*userFunctions)[i].element->id == 2) {
                        userFunctions->erase((*userFunctions).begin() + i);
                    }
                }
            }
            drawBrain = false;
    }));
    userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>("Brain", &drawBrain,
         0.2, 0.9, 0.4, 1, by_position()), [userFunctions](){
            int secondaryButtonIds = 2;
            if (drawBrain) {
                userFunctions->push_back(UserFunction(new Checkbox<Appearance::GREEN>("Draw Axons",
                    &drawLines, 0.9, 0.1, 0.97, 0.15, by_position(), secondaryButtonIds), [](){}));
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
        [camera](){camera->mov.x = 0;}));
    userFunctions->push_back(UserFunction(new UIchar('s', -1),
        [camera](){camera->mov.x = -camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.x = 0;}));
    userFunctions->push_back(UserFunction(new UIchar('a', -1),
        [camera](){camera->mov.y = -camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.y = 0;}));
    userFunctions->push_back(UserFunction(new UIchar('d', -1),
        [camera](){camera->mov.y =  camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.y = 0;}));
    userFunctions->push_back(UserFunction(new UIchar('e', -1),
        [camera](){camera->mov.z =  camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.z = 0;}));
    userFunctions->push_back(UserFunction(new UIchar('z', -1),
        [camera](){camera->mov.z = -camera->DEFAULT_T_SPEED;},
        [camera](){camera->mov.z = 0;}));
}


