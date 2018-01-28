#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>


enum GameMode{MAIN_MENU, SIMULATION, BLOCKING_MESSAGE, FATAL_MESSAGE};
class Simulation {
    public:
        GameMode gameMode;

        void run();
        void init();
        void changeGameMode(GameMode g);

        Simulation();
        ~Simulation();

    protected:

    private:
        static constexpr GameMode INITIAL_GAME_MODE = GameMode::SIMULATION;
        void loadKeyboard();
};

#endif // SIMULATION_H
