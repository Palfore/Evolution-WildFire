#include "Expected.h"

namespace expected{
    std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter) {
         std::vector<std::string> splittedString;
         int startIndex = 0;
         int endIndex   = 0;
         while (static_cast<unsigned>(endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size()) {
           std::string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
           splittedString.push_back(val);
           startIndex = endIndex + delimeter.size();
         }
         if (static_cast<unsigned>(startIndex) < stringToBeSplitted.size()) {
           std::string val = stringToBeSplitted.substr(startIndex);
           splittedString.push_back(val);
         }
         return splittedString;
    }

    std::string getCurrentDate() {
        std::string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        time_t t = time(nullptr);   // get time now
        struct tm * now = localtime( & t );
        return months[now->tm_mon + 1] + '/' + numToStr(now->tm_mday) + '/' + numToStr(now->tm_year + 1900);
    }
    std::string getCurrentTime() {
        time_t t = time(nullptr);   // get time now
        struct tm * now = localtime( & t );
        return numToStr(now->tm_hour) + ':' + numToStr(now->tm_min) + ':' + numToStr(now->tm_sec);
    }

}
