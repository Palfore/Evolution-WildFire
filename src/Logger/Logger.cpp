#include "Logger.h"
#include "Drawing/Shapes.h" // drawing error message
#include "GFramework.h"       // affecing game state
#include "utility.h"
#include "Config.h"
#include "Simulation.h"

#include <iostream> // cout
#include <stdlib.h> // EXIT_FAILURE
#include <sstream>  // ostringstream

Logger::Logger() : logs({}), logFile(LOG_FILE_TITLE, std::ios::out | std::ios::trunc) {}
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::get() {
    static Logger instance;
    return instance;
}

void Logger::log(int line, std::string file, std::string func, std::string msg, LogDegree d, LogType t) {
    std::string logDetails = toString(d) + " " + toString(t);
    std::string relativeFilePath = ""; // Alternatively, you could split at "Evolution Project\\" but no need to show src or include since .h or .cpp shows that.
    if (file.find("src\\") != std::string::npos) {
        relativeFilePath = utility::split<std::vector>(file, "src\\")[1];
    } else if (file.find("include\\") != std::string::npos) {
        relativeFilePath = utility::split<std::vector>(file, "include\\")[1];
    } else {
        relativeFilePath = file;
    }
    std::string uniqueMessage = utility::numToStr<int>(line) + " in " + relativeFilePath + ":" + func + " > " + msg;
    std::string logLookup = logDetails + uniqueMessage;
    std::string errorString = logDetails + " on " + getTimeStamp() + " at line " + uniqueMessage;
    switch (d) {
        case LogDegree::DEBUG:
            logMessage(errorString, Config::getValue<bool>("DEBUG_LOG_TO_CONSOLE"));
            break;
        case LogDegree::WARNING:
            if (logs.find(logLookup) == logs.end()) {
                logs.insert(logLookup);
                logMessage(errorString);
                GFramework::get->simulation->setInputType(InputType::BLOCKING_MESSAGE);
            }
            break;
        case LogDegree::FATAL:
            if (logs.find(logLookup) == logs.end()) {
                logs.insert(logLookup);
                logMessage(errorString);
            }
            GFramework::get->simulation->setInputType(InputType::FATAL_MESSAGE);
            break;
        default:
            logMessage(UNKNOWN_LOG_DEGREE_MESSAGE, false);
            logMessage(errorString, false);
            break;
    }
}

void Logger::normalExit(int line, std::string file, std::string func) {
    log(line, file, func, PROGRAM_EXIT_MESSAGE, LogDegree::DEBUG, LogType::GENERAL);
    glutLeaveMainLoop();
}

std::string Logger::getTimeStamp() {
    return utility::getCurrentDate() + "-(" + utility::getCurrentTime() + ")";
}

void Logger::logMessage(std::string msg, bool writeToConsole) {
    /* Log to File */
    if (logFile.is_open()) {
        logFile << msg << '\n';
    } else {
        /* Every log after failure, should try to reopen the file */
        logFile.open(LOG_FILE_TITLE, std::ios::out | std::ios::trunc);
        if (logFile.is_open()) {
            std::cout << OPENED_LOG_AFTER_FAILURE_MESSAGE;
            logFile << msg + '\n';
        } else {
            std::cout << FAILED_TO_OPEN_AFTER_FAILURE_MESSAGE;
        }
    }

    /* Log to Stream */
    if (writeToConsole) {
        std::cout << msg << '\n'; // print to console
    }
}

std::string Logger::toString(LogDegree s) {
    switch (s) {
        case LogDegree::DEBUG:   return "DEBUG";
        case LogDegree::WARNING: return "WARNING";
        case LogDegree::FATAL:   return "FATAL";
        default:      return "UNKOWN";
    }
}

std::string Logger::toString(LogType t) {
    switch (t) {
        case LogType::GENERAL:  return "GENERAL";
        case LogType::GRAPHICS: return "GRAPHICS";
        case LogType::DISPLAY:  return "DISPLAY";
        case LogType::AUDIO:    return "AUDIO";
        case LogType::CONFIG:   return "CONFIG";
        case LogType::GENETIC:   return "GENETIC";
        default:        return "UNKNOWN";
    }
}


