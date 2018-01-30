#include "Logger.h"
#include "Drawing/Shapes.h" // drawing error message
#include "Graphics.h"       // affecing game state

Logger::Logger() : logs({}), logFile(LOG_FILE_TITLE, std::ios::out | std::ios::trunc) {}
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(int line, std::string file, std::string func, std::string msg) {log(line, file, func, DEFAULT_DEGREE, msg);}
void Logger::log(int line, std::string file, std::string func, LogDegree d, std::string msg) {log(line, file, func, d, DEFAULT_TYPE, msg);}
void Logger::log(int line, std::string file, std::string func, LogDegree d, LogType e, std::string msg) {
    std::string logType = toString(d) + " " + toString(e);
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
            logMessage("Invalid Log Degree Specified.", false, true);
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
        std::cout << "Unable to open logging file.\n";
        /* Every log after failure, should try to reopent the file */
        logFile.open(LOG_FILE_TITLE, std::ios::out | std::ios::trunc);
        if (logFile.is_open()) {
            logFile << msg << '\n';
        }
    }

    /* Log to Stream */
    if (writeToConsole) {
        std::cout << msg << '\n'; // print to console
    }
    if (showToUser) {
        DrawRectangle<Appearance::BLUE>(10, 10, 500, 500);

        Graphics::get().simulation.setInputType(InputType::BLOCKING_MESSAGE);
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

std::string Logger::toString(LogType e) {
    switch (e) {
        case LogType::GENERAL:  return "GENERAL";
        case LogType::GRAPHICS: return "GRAPHICS";
        case LogType::DISPLAY:  return "DISPLAY";
        case LogType::INPUT:    return "INPUT";
        case LogType::AUDIO:    return "AUDIO";
        default:        return "UNKNOWN";
    }
}


