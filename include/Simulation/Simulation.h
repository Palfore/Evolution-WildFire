#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
class UserFunction;
enum class GameMode : unsigned int {MAIN_MENU, EVOLVE};
enum class InputType: unsigned int {DEFAULT, BLOCKING_MESSAGE, FATAL_MESSAGE, NUMERIC_INPUT, ALPHA_NUMERIC_INPUT};
class Simulation {
    public:
        GameMode gameMode;
        InputType inputType;
        bool cinematic;

        Simulation();
        ~Simulation();

        void run(std::vector<UserFunction> *userFunctions, const double fps);
        void init();
        void setGameMode(GameMode g);
        void setInputType(InputType t);

    private:
        static constexpr GameMode INITIAL_GAME_MODE = GameMode::MAIN_MENU;
        static constexpr InputType INITIAL_INPUT_TYPE = InputType::DEFAULT;
        static constexpr const char * UNKNOWN_INPUT_TYPE_MESSAGE = "UNKNOWN INPUT TYPE KEYBOARD SELECTED.";
        static constexpr const char * UNKNOWN_GAME_MODE_MESSAGE  = "UNKNOWN GAMEMODE SELECTED.";

        void setInputTypeKeyboard();
        void loadGameModeKeyboard();

        void loadMainMenu(); // There should be a more generic way
        void loadEvolve();

        void mainMenuMode();
        void evolveMode(double fps);
};

#endif // SIMULATION_H
