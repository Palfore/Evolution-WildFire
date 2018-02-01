#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include "Logger.h"
#include "MyMath.h"

//class Sound {
//    public:
//        Sound(std::string file);
//        Sound(std::string file, double x, double y, double z);
//        Sound(std::string file, Vec position);
//        ~Sound();
//
//        sf::SoundBuffer buffer;
//        sf::Sound sound;
//        Vec loc;
//        bool played;
//};

/* Needs more options! Mono vs Stereo, disable spatialization, loop, attenuation, min distance, clear when done */
class Audio {
    private:
        static char constexpr* AUDIO_DIRECTORY = (char*) "assets/Audio/";
    public:
        sf::Music music;
        std::map<std::string, sf::SoundBuffer> soundBuffers; // Loaded at init
        std::vector<sf::Sound> sounds;

        Audio();
        ~Audio();
        void playSound(std::string soundFile, double x, double y, double z);
        void playSound(std::string soundFile, Vec position);
        void playSound(std::string soundFile);
        void clearStoppedSounds();
};

#endif // AUDIO_H
