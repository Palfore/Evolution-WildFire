#ifndef SIMULATION_H
#define SIMULATION_H

enum class GameMode{MAIN_MENU, SIMULATION};
enum class InputType{DEFAULT, BLOCKING_MESSAGE, FATAL_MESSAGE, NUMERIC_INPUT, ALPHA_NUMERIC_INPUT};
class Simulation {
    public:
        GameMode gameMode;
        InputType inputType;

        void run();
        void init();
        void setGameMode(GameMode g);
        void setInputType(InputType t);

        Simulation();
        ~Simulation();

    private:
        static constexpr GameMode INITIAL_GAME_MODE = GameMode::SIMULATION;
        static constexpr InputType INITIAL_INPUT_TYPE = InputType::DEFAULT;

        void setInputTypeKeyboard();
        void loadGameModeKeyboard();
};

#endif // SIMULATION_H
