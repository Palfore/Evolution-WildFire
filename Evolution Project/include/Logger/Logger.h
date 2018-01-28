#ifndef LOGGER_H
#define LOGGER_H


#include "Expected.h"
#include <iostream> // cout
#include <fstream>  // ofstream
#include <stdlib.h> // EXIT_FAILURE
#include <sstream>  // ostringstream
#include <vector>   // vector
#include <unordered_set> // unordered_set -> fastest container for unique entries


#define LOG(...) Logger::get().log(__LINE__, __FILE__, __FUNCTION__, __VA_ARGS__) //if __FUNCTION__ -> "operator()" means lambda expression
#define NORMAL_EXIT() Logger::get().normalExit(__LINE__, __FILE__, __FUNCTION__)

/**
 * Logs all debugging messages, only logs unique warnings. (Note: Only 1 fatal message is ever possible).\
 *
 * Debug: Show to console, log to file
 * Warning: Show to console, log to file, display to user (blocks input until user dismisses message (enter)
 * Fatal: Show to console, log to file, display to user (EXIT_FAILURE on dismissal)
 */
enum class LogDegree : int {DEBUG, WARNING, FATAL};
enum class LogType : int {GENERAL, GRAPHICS, DISPLAY};

class Logger {
    public:
        static Logger& get() { // Singleton Pattern
            static Logger instance;
            return instance;
        }
        void log(int line, std::string file, std::string func, std::string msg);
        void log(int line, std::string file, std::string func, LogDegree d, std::string msg);
        void log(int line, std::string file, std::string func, LogDegree d, LogType e, std::string msg);

        void normalExit(int line, std::string file, std::string func);
        ~Logger();
    protected:

    private:
        static LogDegree constexpr DEFAULT_DEGREE = LogDegree::DEBUG;
        static LogType constexpr DEFAULT_TYPE  = LogType::GENERAL;
        static char constexpr* LOG_FILE_TITLE = (char*) "assets/log.txt";
        static char constexpr* PROGRAM_EXIT_MESSAGE = (char*) "The program has exited successfully.";

        Logger();
        void logMessage(std::string msg, bool writeToConsole=true, bool showToUser=true);
        std::string getTimeStamp();

        std::unordered_set<std::string> logs;
        std::ofstream logFile;
        std::string toString(LogDegree s);
        std::string toString(LogType e);
};

#endif // LOGGER_H
