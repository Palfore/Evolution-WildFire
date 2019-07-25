#include "Simulation.h"
#include "Shapes.h"
#include "Objects.h"
#include "GFramework.h"
#include "Population.h"
#include <math.h>
#include "MyMath.h"
#include "Button.h"
#include "Viewer.h"

#include "StickBall.h"

#include "Factory.h"
#include "Terrain.h"
static const Terrain terrain = Terrain(0);
static const Factory creatureFactory("OmniWalker");
static const SenarioFactory senarioFactory("SenarioA", terrain, 5000);
static Population pop(200, creatureFactory);
static Viewer viewer(pop.population, creatureFactory, senarioFactory);

static void draw() {
    DrawCylinder<Appearance::BARK>(Vec(-50, 50, 0), Vec(-50, 50, 50), 8);
    DrawSphere<Appearance::TREE_TOP>(Vec(-50, 50, 50), 25);
    DrawSkybox(500);
    viewer.draw();

//    DrawRectangle<Appearance::LION>(0.6, 0.5, 1, 0.9);

    DrawRectangle<Appearance::PALFORE>(0, 0.875, 0.07, 1);
    DrawRectangle<Appearance::GAME_TITLE>(0.25, 0.05, 0.75, 0.2);
}

void Simulation::mainMenuMode() {
    viewer.update();
    draw();
    GFramework::get->camera->cinematicCamera();
}

#include "Audio.h"
#include <thread>
void Simulation::loadMainMenu() {
    std::vector<UserFunction> *userFunctions = &GFramework::get->userInput.functions;
    userFunctions->push_back(UserFunction(new UIchar(ENTER), [&](){
        this->setGameMode(GameMode::EVOLVE);
    }));
    userFunctions->push_back(UserFunction(new Button<Appearance::BUTTON>("Enter Simulation Mode", 0.1, 0.44, 0.3, 0.48, by_position()), [&](){
        GFramework::get->audio->playSound("click.wav");
        setGameMode(GameMode::EVOLVE);
    }));
    userFunctions->push_back(UserFunction(new Button<Appearance::BUTTON>("Exit", 0.1, 0.48, 0.3, 0.52, by_position()), [](){
        GFramework::get->audio->playSound("click.wav");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        NORMAL_EXIT();
    }));
}



