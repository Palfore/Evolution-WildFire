#ifndef EXPECTED_H
#define EXPECTED_H

#include <string>
#include <ctime>
#include <vector>
//#include <fstream>  // ofstream
//#include <iostream> // cout
#include <sstream>  // ostringstream

namespace expected {
    template <typename T> // fix to compiler bug > toString not a member of std
    std::string numToStr (T Number) {
        std::ostringstream ss;
        ss << Number;
        return ss.str();
    }
    std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter);

    std::string getCurrentTime();
    std::string getCurrentDate();


}

#endif // EXPECTED_H
