#ifndef EXPECTED_H
#define EXPECTED_H

#include <string>
#include <sstream>  // ostringstream
#include <vector>
namespace expected {
    std::string replaceString(std::string subject, const std::string& search, const std::string& replace);
    std::string replaceCharSet(std::string subject, const std::string& charSet, const std::string& replace);
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
