#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <type_traits>
#include "Logger.h"

/** @brief This namespace contains the functions which convert values to their
 *      respective type.
 * @details In the Config class, a key is specified which is looked up in the
 * Config file. In these functions, the value is converted from a std::string to
 * the specified template type.
 * @warning This should not be used outside of the Config class.
 * @note Ideally these functions would be in the config class, however
 * for templating reaons, it must be in a seperate namespace.
 * @see Config
 *
 */
namespace ConfigValueConverters {
        /** @brief Templated function to return the value as the specified type T.
         *
         * @param value const std::string The value to convert.
         * @return bool The value as a boolean.
         * @see Config::getFileValue
         * @note This should only be called by Config::getValue.
         */
        template <typename T> T getValue_(std::string value);

        /** @brief Template specialization, returns value as boolean.
         *
         * @param value const std::string The value to convert.
         * @return bool The value as a boolean.
         * @see Config::getFileValue, ConfigValueConverters::getValue_
         * @note This should only be called by Config::getValue.
         */
        template <> bool getValue_<bool>(std::string value);

        /** @brief Template specialization, returns value as boolean.
         *
         * @param value const std::string The value to convert.
         * @return int The value as an integer.
         * @see Config::getFileValue, ConfigValueConverters::getValue_
         * @note This should only be called by Config::getValue.
         */
        template <> int getValue_<int>(std::string value);

        /** @brief Template specialization, returns value as boolean.
         *
         * @param value const std::string The value to convert.
         * @return double The value as a double.
         * @see Config::getFileValue, ConfigValueConverters::getValue_
         * @note This should only be called by Config::getValue.
         */
        template <> double getValue_<double>(std::string value);

        /** @brief Template specialization, returns value as boolean.
         *
         * @param value const std::string The value to convert.
         * @return std::string The value as a std::string.
         * @see Config::getFileValue, ConfigValueConverters::getValue_
         * @note This should only be called by Config::getValue.
         */
        template <> std::string getValue_<std::string>(std::string value);
}


/** @brief This class allows variables to be loaded from a file.
 * @details This is very useful to set the state of the program on start up.
 * To add a new config parameter, just append it to the config file with an associated value.
 * Values can be parsed as boolean, int, double, std::string.
 * Then the value can be retrieved from the key as:
 * @code {.cpp}
 *     std::string value = Config::getValue<std::string>("STRING_TEST")
 * @endcode
 * @todo Allow changes to config file based on program state.
 */
class Config {
    private:
        static constexpr const char * CONFIG_FILE = "assets/config.config";  ///< The file containing the configuration data.


        /** @brief Returns the corresponding value to the key provided from the config file.
         *
         * @param key std::string The string to be found in the configuration file.
         * @return std::string The value of the configuration variable.
         *
         */
        static std::string getFileValue(std::string key);

    public:
        /** @brief Returns the value associated with the key as the specified type.
         *
         * @param  key std::string The key to be found.
         * @return T templated parameter can be one of bool, int, double, std::string.
         * @warning Fatal error if key cannot be found since configuration parameters are required.
         *
         */
        template <typename T>
        static T getValue(std::string key) {
            return ConfigValueConverters::getValue_<T>(getFileValue(key));
        }

        ~Config();  ///< Empty destructor.
    protected:
        Config();   ///< Empty constructor.
};

#endif // CONFIG_H
