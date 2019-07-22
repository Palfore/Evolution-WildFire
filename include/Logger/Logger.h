#ifndef LOGGER_H
#define LOGGER_H

#include <unordered_set> // unordered_set -> fastest container for unique entries
#include <fstream>       // ofstream


/** @brief Used to specify different levels of logging severity.
 * @see Logger::log
 */
enum class LogDegree : unsigned int {DEBUG, WARNING, FATAL};


/** @brief Used to specify different types of logging message (ie: the reason for the log).
 * @see Logger::log
 */
enum class LogType   : unsigned int {GENERAL, GRAPHICS, DISPLAY, AUDIO, CONFIG, GENETIC};


/** @brief This class houses the logging functionality.
 * @details It allows you to log messages of various severity (degree) and provides detailed output which
 * is very useful for debugging.
 * @todo Right now, there isn't a way to propogate warning and error messages to the screen.
 *      Maybe this could be a variable "lastLogged" or "lastError" or "lastWarning" OR "lastSignificantLog".
 */
class Logger {
    /**
     * @brief This macro passes the line, file, and function to the logger, along with a message and optional parameters.
     * @details It effectively acts like a decorator and allows more detailed logging information to be passed to the logger.
     * Should be called like so (the last two parameters are optional):
     * @code {.cpp}
     *     LOG("This is my message.", LogDegree::DEBUG, LogType::GENERAL);
     * @endcode
     *
     * @param msg (Required) The message to be printed with the log.
     * @param logDegree (Optional) The message to be printed with the log.
     * @param logType (Optional) The message to be printed with the log.
     *
     * @return void
     * @warning Type and argument checking is lacking due to macro implementation. It may be difficult to determine the incorrect call
     * to this macro if incorrect parameters are passed. This is because the compiler redirects you to this definition instead of the culprite.
     * @see Logger::log
     * @note Although the input is '...' you a required to specify a message. This is to prevent 'c99 requires rest arguments to be used' error.
     * @note If the function is printed as "operator()" this means a lambda expression.
     */
    #define LOG(...) Logger::get().log(__LINE__, __FILE__, __PRETTY_FUNCTION__, __VA_ARGS__)


    /** @brief Exits the program normally, but also logs the line, file and function where the exiting call came from.
     *
     * @return void
     * @warning Type and argument checking is lacking due to macro implementation. It may be difficult to determine the incorrect call
     * to this macro if incorrect parameters are passed. This is because the compiler redirects you to this definition instead of the culprite.
     * @see Logger::normalExit
     */
    #define NORMAL_EXIT() Logger::get().normalExit(__LINE__, __FILE__, __PRETTY_FUNCTION__)

    private:
        static constexpr const LogDegree DEFAULT_DEGREE = LogDegree::DEBUG; ///< The LogDegree to use if none is specified.
        static constexpr const LogType   DEFAULT_TYPE   = LogType::GENERAL; ///< The LogType to use if none is specified.

    public:

        /** @brief Returns the singleton instance.
         *
         * @return Logger& Reference to the Logger singleton instance.
         *
         */
        static Logger& get();

        /** @brief Logs a descriptive message to the appropriate stream.
         * @details The logged string is in the format [msg] [line] [file] [function] [date] [time]
         * in a human readable format. The LogDegree specifies where and how the message should logged.
         *
         * \arg \c \b Debug: Show to console, log to file
         * \arg \c \b Warning: Show to console, log to file, display to user (blocks input until user dismisses message).
         *              Only logs unique warnings.
         * \arg \c \b Fatal: Show to console, log to file, display to user (EXIT_FAILURE on dismissal)
         *
         *
         * @param line int The line that called the logger.
         * @param file std::string The file that called the logger.
         * @param func std::string The function that called the logger.
         * @param msg std::string The message to be logged.
         * @param d LogDegree The specified LogDegree.
         * @param t LogType The specified LogType.
         * @return void
         *
         * @warning This function is not meant to be called manually. It should be called through the @ref LOG macro.
         * @see #LOG
         */
        void log(int line, std::string file, std::string func, std::string msg, LogDegree d=DEFAULT_DEGREE, LogType t=DEFAULT_TYPE);


        /** @brief Logs a descriptive normal exit message.
         * @details The logged string is in the format [msg] [line] [file] [function] [date] [time]
         * in a human readable format. Normal exit is logged as debug message.
         *
         *
         * @param line int The line that called the logger.
         * @param file std::string The file that called the logger.
         * @param func std::string The function that called the logger.
         * @return void
         *
         * @warning This function is not meant to be called manually. It should be called through the @ref NORMAL_EXIT macro.
         * @see #NORMAL_EXIT
         */
        void normalExit(int line, std::string file, std::string func);


         /** @brief Cleans up the logger
          * @details Closes the log file.
          *
          */
         ~Logger();
        Logger(Logger const&)          = delete;  ///< Deleted for singleton pattern.
        Logger& operator=(Logger const&)  = delete;  ///< Deleted for singleton pattern.

    private:
        static constexpr const char * LOG_FILE_TITLE = "assets/logger.log";
        static constexpr const char * PROGRAM_EXIT_MESSAGE = "The program has exited successfully.";
        static constexpr const char * OPENED_LOG_AFTER_FAILURE_MESSAGE = "Initialization could not open logging file. Succeeded in reopening.\n";
        static constexpr const char * FAILED_TO_OPEN_AFTER_FAILURE_MESSAGE = "Initialization could not open logging file. Failed in reopening.\n";
        static constexpr const char * UNKNOWN_LOG_DEGREE_MESSAGE = "Invalid Log Degree Specified.\n";

        Logger();  ///< Private constructor for singleton pattern.


        /** @brief Logs the message to the approriate stream.
         * @details Tries to open the log file for writting. If it is unsucessful, every next log attempt will try to reopen the file.
         * Warnings and Errors will only be logged once. Errors should alert the user and terminate the program. Warnings should just prompt the user.
         * This handling is implemented in the simulation.
         * Debug messages are written to the console based on @ref Config file parameter "DEBUG_LOG_TO_CONSOLE", while Warnings and Errors are.
         *
         * @param msg std::string The message to be logged.
         * @param writeToConsole bool Should the message print to the console.
         * @return void
         *
         * @note Default arguments are not obvious. I always have to come back here. Maybe fix this?
         */
        void logMessage(std::string msg, bool writeToConsole=true);


        /** @brief Returns the current time.
         *
         * @return std::string Time in the format MMM/dd/yyyy-(hh:mm:ss)
         *
         */
        std::string getTimeStamp();


        /** @brief Stores the unique warning log messages.
         * @details This is used to prevent duplicate warnings which would likely lead to spam.
         */
        std::unordered_set<std::string> logs;
        std::ofstream logFile;              ///< The ofsteam corresponding to the logging file.
        std::string toString(LogDegree s);  ///< Converts a LogDegree to a string (Verbatim)
        std::string toString(LogType e);    ///< Converts a LogType to a string (Verbatim)
};

#endif // LOGGER_H
