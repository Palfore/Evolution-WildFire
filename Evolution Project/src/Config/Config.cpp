#include "Config.h"
#include "Logger.h"
#include "utility.h"

#include <fstream> // ifstream
#include <algorithm> // find, transform
#include <string> // stod

namespace ConfigValueConverters {
        template <> bool getValue_<bool>(std::string value) {
            std::array<std::string, 6> truths = {"TRUE", "True", "true", "T", "t", "1"};
            std::array<std::string, 6> falses = {"FALSE", "False", "false", "F", "f", "0"};

            if (std::find(truths.begin(), truths.end(), value) != truths.end()) {
                return true;
            } else if (std::find(falses.begin(), falses.end(), value) != falses.end()) {
                return false;
            } else {
                LOG("Could not determine boolean for " + value + ".", LogDegree::FATAL, LogType::CONFIG);
                return false;
            }
        }

        template <> int getValue_<int>(std::string value) {
            try {
                return atof(value.c_str());
            } catch(std::invalid_argument e) {
                LOG("Could not parse int for " + value + ".", LogDegree::FATAL, LogType::CONFIG);
                return -1;
            } catch (std::out_of_range e) {
                LOG("Int overflow for " + value + ".", LogDegree::FATAL, LogType::CONFIG);
            }
        }

        template <> double getValue_<double>(std::string value) {
            try {
                return atof(value.c_str());
            } catch(std::invalid_argument e) {
                LOG("Could not parse double for " + value + ".", LogDegree::FATAL, LogType::CONFIG);
                return -1;
            } catch (std::out_of_range e) {
                LOG("Double overflow for " + value + ".", LogDegree::FATAL, LogType::CONFIG);
            }
        }

        template <> std::string getValue_<std::string>(std::string value) {
            return value;
        }
}


Config::Config() {

}

Config::~Config() {

}

std::string Config::getFileValue(std::string key) {
    std::string line;
    std::ifstream infile(CONFIG_FILE);
    key = utility::toUpper(key);
    while (std::getline(infile, line)) {
        auto tokens = utility::split(line, " ");

        int numTokens = tokens.size();
        switch(numTokens) {
            case 0: // Empty Line
                continue;
            case 1: // Unpaired key
                LOG("Config file is improperly formatted.", LogDegree::FATAL, LogType::CONFIG);
                break;
            case 2: // Simple key-value pair
                if (utility::toUpper(tokens[0]) == key) {
                    return tokens[1]; // Key's associated value.
                }
                break;
            default: // key-[set of words] > "key" "many words as valueString"
                if (utility::toUpper(tokens[0]) == key) {
                    std::string valueString = "";
                    for (int i = 1; i < numTokens; i++) {
                        valueString += tokens[i];
                        if (i != numTokens - 1) {
                            valueString += ' ';
                        }
                    }
                    return valueString;
                }
                break;
        }
    }
    LOG("Could not find " + key + " in config file.", LogDegree::FATAL, LogType::CONFIG);
    return "";
}





