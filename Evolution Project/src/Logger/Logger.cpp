#include "Logger.h"
#include "Drawing/Shapes.h" // drawing error message
#include "Graphics.h"       // affecing game state

#include <iostream> // cout
#include <stdlib.h> // EXIT_FAILURE
#include <sstream>  // ostringstream

Logger::Logger() : logs({}), logFile(LOG_FILE_TITLE, std::ios::out | std::ios::trunc) {}
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(int line, std::string file, std::string func, std::string msg, LogDegree d, LogType t) {
    std::string logType = toString(d) + " " + toString(t);
    std::string uniqueMessage = expected::numToStr<int>(line) + " in " + expected::split(file, "src\\")[1] + ":" + func + " > " + msg;
    std::string logLookup = logType + uniqueMessage;

    std::string errorString = logType + " on " + getTimeStamp() + " at line " + uniqueMessage;
    switch (d) {
        case LogDegree::DEBUG:
            logMessage(errorString, false, true);
            break;
        case LogDegree::WARNING:
            if (logs.find(logLookup) == logs.end()) { // not in set
                logs.insert(logLookup);
                logMessage(errorString);
            }
            break;
        case LogDegree::FATAL:
            logMessage(errorString);
            Graphics::get().simulation.setInputType(InputType::FATAL_MESSAGE);
            break;
        default:
            logMessage(UNKNOWN_LOG_DEGREE_MESSAGE, false, true);
            logMessage(errorString, false, true);
    }
}

void Logger::normalExit(int line, std::string file, std::string func) {
    std::string executationLocation = "line " + expected::numToStr<int>(line) + " in " + expected::split(file, "src\\")[1] + ":" + func;
    logMessage(std::string(PROGRAM_EXIT_MESSAGE) + " From " + executationLocation + " at " + getTimeStamp() + ".");
    exit(EXIT_SUCCESS);
}

std::string Logger::getTimeStamp() {
    return expected::getCurrentDate() + "-(" + expected::getCurrentTime() + ")";
}

void Logger::logMessage(std::string msg, bool showToUser, bool writeToConsole) {
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
    if (showToUser) {
        DrawRectangle<Appearance::BLUE>(10, 10, 500, 500);

        Graphics::get().simulation.setInputType(InputType::BLOCKING_MESSAGE);
        /* Log to Screen (Lock user input until they press okay, or enter) */
        // to implement later
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
        case LogType::INPUT:    return "INPUT";
        case LogType::AUDIO:    return "AUDIO";
        case LogType::CONFIG:   return "CONFIG";
        default:        return "UNKNOWN";
    }
}


