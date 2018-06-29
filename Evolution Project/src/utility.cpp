#include "utility.h"
#include <ctime>
#include <vector>
#include <deque>

#include <iostream>
#include <algorithm>
namespace utility {
    std::string toTitle(std::string str) {
        return std::string(1, toupper(str[0])) + str.substr(1);
    }

    std::string toUpper(std::string str) {
        std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        return str;
    }

    std::string toLower(std::string str) {
        std::transform(str.begin(), str.end(),str.begin(), ::tolower);
        return str;
    }

    std::string replaceString(std::string subject, const std::string& search, const std::string& replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
             subject.replace(pos, search.length(), replace);
             pos += replace.length();
        }
        return subject;
    }

    std::string replaceCharSet(std::string subject, const std::string& charSet, const std::string& replace) {
        for (char c : charSet) {
            subject = replaceString(subject, std::string(1, c), replace);
        }
        return subject;
    }

    template<template <typename, typename = std::allocator<std::string>> class C>
    C<std::string> split(std::string stringToBeSplit, std::string delimeter, int maxSplits) {
        C<std::string> splitString;
        int numSplits = 0;
        int startIndex = 0;
        int endIndex   = 0;
        while (static_cast<unsigned>(endIndex = stringToBeSplit.find(delimeter, startIndex)) < stringToBeSplit.size()) {
            std::string val = stringToBeSplit.substr(startIndex, endIndex - startIndex);
            splitString.push_back(val);
            startIndex = endIndex + delimeter.size();

            if ((maxSplits != -1) && (++numSplits >= maxSplits)) {
                break;
            }
        }
        if (static_cast<unsigned>(startIndex) < stringToBeSplit.size()) {
            std::string val = stringToBeSplit.substr(startIndex);
            splitString.push_back(val);
        }
        return splitString;
    }
    template std::vector<std::string, std::allocator<std::string>> split<std::vector>(std::string s, std::string delim, int maxSplits);
    template std::deque<std::string, std::allocator<std::string>> split<std::deque>(std::string s, std::string delim, int maxSplits);

    template<template <typename, typename = std::allocator<std::string>> class C>
    C<std::string> split(std::string stringToBeSplit, char delimiter, int maxSplits) {
        return split<C>(stringToBeSplit, std::string(1, delimiter), maxSplits);
    }
    template std::vector<std::string, std::allocator<std::string>> split<std::vector>(std::string s, char delim, int maxSplits);
    template std::deque<std::string, std::allocator<std::string>> split<std::deque>(std::string s, char delim, int maxSplits);

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
