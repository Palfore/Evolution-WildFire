#include "Config.h"
#include "Logger.h"

#include <fstream> // ifstream
#include <algorithm> // find
#include <string> // stod

Config::Config() {

}

Config::~Config() {

}

std::string Config::loadFromFile(const std::string s) {
    std::string a, b;
    std::ifstream infile(CONFIG_FILE);
    while (infile >> a >> b) {
        if (a == s) return b;
    }
    LOG("Could not find config data.", LogDegree::FATAL, LogType::CONFIG);
    return "";
}

bool Config::loadFromFile_bool(const std::string s) {
    std::string value = loadFromFile(s);
    std::array<std::string, 5> truths = {"True", "true", "T", "t", "1"};
    std::array<std::string, 5> falses = {"False", "false", "F", "f", "0"};

    if (std::find(truths.begin(), truths.end(), value) != truths.end()) {
        return true;
    } else if (std::find(falses.begin(), falses.end(), value) != falses.end()) {
        return false;
    } else {
        LOG("Could not determine boolean.", LogDegree::FATAL, LogType::CONFIG);
        return false;
    }
}

double Config::loadFromFile_double(const std::string s) {
    std::string value = loadFromFile(s);
    try {
        return atof(value.c_str());
    } catch(std::invalid_argument e) {
        LOG("Could not parse double.", LogDegree::FATAL, LogType::CONFIG);
        return -1;
    } catch (std::out_of_range e) {
        LOG("Double overflow.", LogDegree::FATAL, LogType::CONFIG);
    }
}

int Config::loadFromFile_int(const std::string s) {
    std::string value = loadFromFile(s);
    try {
        return atof(value.c_str());
    } catch(std::invalid_argument e) {
        LOG("Could not parse int.", LogDegree::FATAL, LogType::CONFIG);
        return -1;
    } catch (std::out_of_range e) {
        LOG("Int overflow.", LogDegree::FATAL, LogType::CONFIG);
    }
}

std::string Config::loadFromFile_string(const std::string s) {
    return loadFromFile(s);
}
