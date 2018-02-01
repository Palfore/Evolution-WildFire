#ifndef LOGGER_H
#define LOGGER_H

#include "Expected.h"

#include <unordered_set> // unordered_set -> fastest container for unique entries
#include <fstream>  // ofstream

#define LOG(...) Logger::get().log(__LINE__, __FILE__, __FUNCTION__, __VA_ARGS__) // __FUNCTION__ printing "operator()" means lambda expression
#define NORMAL_EXIT() Logger::get().normalExit(__LINE__, __FILE__, __FUNCTION__)

enum class LogDegree : unsigned int {DEBUG, WARNING, FATAL};
enum class LogType   : unsigned int {GENERAL, GRAPHICS, DISPLAY, INPUT, AUDIO};

/**
 * Logs all debugging messages, only logs unique warnings. (Note: Only 1 fatal message is ever possible).
 *
 * Debug: Show to console, log to file
 * Warning: Show to console, log to file, display to user (blocks input until user dismisses message
 * Fatal: Show to console, log to file, display to user (EXIT_FAILURE on dismissal)
 */
class Logger {
    private:
        static constexpr const LogDegree DEFAULT_DEGREE = LogDegree::DEBUG;
        static constexpr const LogType   DEFAULT_TYPE   = LogType::GENERAL;

    public:
        static Logger& get() { // Singleton Pattern
            static Logger instance;
            return instance;
        }

        void log(int line, std::string file, std::string func, std::string msg, LogDegree d=DEFAULT_DEGREE, LogType t=DEFAULT_TYPE);
        void normalExit(int line, std::string file, std::string func);

        ~Logger();
        Logger(Logger const&)          = delete;
        void operator=(Logger const&)  = delete;

    private:
        static constexpr const char * LOG_FILE_TITLE = "assets/log.txt";
        static constexpr const char * PROGRAM_EXIT_MESSAGE = "The program has exited successfully.";
        static constexpr const char * OPENED_LOG_AFTER_FAILURE_MESSAGE = "Initialization could not open logging file. Succeeded in reopening.\n";
        static constexpr const char * FAILED_TO_OPEN_AFTER_FAILURE_MESSAGE = "Initialization could not open logging file. Failed in reopening.\n";
        static constexpr const char * UNKNOWN_LOG_DEGREE_MESSAGE = "Invalid Log Degree Specified.\n";

        Logger();

        void logMessage(std::string msg, bool writeToConsole=true, bool showToUser=true);
        std::string getTimeStamp();
        std::unordered_set<std::string> logs;
        std::ofstream logFile;
        std::string toString(LogDegree s);
        std::string toString(LogType e);
};

#endif // LOGGER_H
