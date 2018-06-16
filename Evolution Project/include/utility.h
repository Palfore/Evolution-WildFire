#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>  // ostringstream
#include <vector>

#include "Logger.h"

/** @brief This namespace contains functions which you might expect to be standard in cpp.
 *
 * Specifically functions in the standard libraries. If you ever ask "why isnt that function standard?"
 * then it should be here.
 *
 */
namespace utility {
    /** @brief Converts a std::string to upper case.
     *
     * @param str std::string The string to convert the case of.
     * @return std::string The string represented as lower case.
     *
     */
    std::string toUpper(std::string str);


    /** @brief Converts a std::string to lower case.
     *
     * @param str std::string The string to convert the case of.
     * @return std::string The string represented as lower case.
     *
     */
    std::string toLower(std::string str);


    /** @brief Returns a string with the desired text replaced.
     *
     * @param subject std::string The string to be returned with replacement.
     * @param search const std::string& The text to replace.
     * @param replace const std::string& The text to replace with.
     * @return std::string The replaced string
     *
     */
    std::string replaceString(std::string subject, const std::string& search, const std::string& replace);


    /** @brief Replaces a set of characters with a string.
     * @details This replacement string can often be just a character. This will the replace a set of characters with one character.
     *
     * @param subject std::string The string to be returned with replacement.
     * @param charSet const std::string& The set of characters to be replaced.
     * @param replace const std::string& The text to replace with.
     * @return std::string The string with the character set replaced.
     *
     */
    std::string replaceCharSet(std::string subject, const std::string& charSet, const std::string& replace);


    /** @brief Returns a number as its string representation.
     *
     * @tparam T The number type (eg: float, double, int, etc.)
     * @param Number The value to be converted to a string.
     * @return A string representation of a number.
     *
     */
    template <typename T> // fix to compiler bug > to_string not a member of std
    std::string numToStr (T Number) {
        std::ostringstream ss;
        ss << Number;
        return ss.str();
    }

    template<typename T>
    T strToNum(const std::string& numberAsString) {
        T valor;
        std::stringstream stream(numberAsString);
        stream >> valor;
        if (stream.fail()) {
            LOG("String conversion to number failed", LogDegree::FATAL, LogType::GENERAL);
        }
        return valor;
    }


    /** @brief Splits a string based on a supplied delimiter.
     * @details Example:
     * @code {.cpp}
     *  std::vector<std::string> = split<std::vector>(myString, myDelimiter);
     * @endcode
     * @tparam C The container the collection of strings should be returned as.
     * @param stringToBeSplit std::string The string to be split.
     * @param delimiter std::string The delimiter to split the string on.
     * @param maxSplits int The maximum number of strings to be returned. -1 means no bounds.
     * @return std::vector<std::string> A vector of the split strings.
     */
    template<template <typename, typename = std::allocator<std::string>> class C>
    C<std::string> split(std::string stringToBeSplit, std::string delimiter, int maxSplits=-1);


    /** @brief Splits a string based on a supplied delimiter.
     * @details Example:
     * @code {.cpp}
     *  std::vector<std::string> = split<std::vector>(myString, myDelimiter);
     * @endcode
     * @tparam C The container the collection of strings should be returned as.
     * @param stringToBeSplit std::string The string to be split.
     * @param delimiter char The delimiter to split the string on.
     * @param maxSplits int The maximum number of strings to be returned. -1 means no bounds.
     * @return std::vector<std::string> A vector of the split strings.
     */
    template<template <typename, typename = std::allocator<std::string>> class C>
    C<std::string> split(std::string stringToBeSplit, char delimiter, int maxSplits=-1);




    /** @brief Returns the current time.
     *
     * @return std::string The current time as a string in the format: hh:mm:ss
     *
     */
    std::string getCurrentTime();


    /** @brief Returns the current date.
     *
     * @return std::string The current date as a string in the format: mmm/dd/yyyy
     *
     */
    std::string getCurrentDate();
}

#endif // UTILITY_H
