#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_FULLSCREEN() Config::loadFromFile_bool("CONFIG_FULLSCREEN")
#define CONFIG_AUDIO() Config::loadFromFile_bool("CONFIG_AUDIO")

#include <iostream>
class Config final {
    private:
        static constexpr const char * CONFIG_FILE = "assets/config.config";

        static std::string loadFromFile(const std::string s);
    public:
        static bool loadFromFile_bool(const std::string s);
        static double loadFromFile_double(const std::string s);
        static int loadFromFile_int(const std::string s);
        static std::string loadFromFile_string(const std::string s);
        ~Config();
    protected:
        Config();
};

#endif // CONFIG_H
