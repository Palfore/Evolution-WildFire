#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include "Logger.h"
#include "MyMath.h"

/* Needs more options! Mono vs Stereo, disable spatialization, loop, attenuation, min distance, clear when done */
class Audio {
    private:
        static char constexpr const * AUDIO_DIRECTORY = "assets/Audio/";
        static char constexpr const * COULD_NOT_LOAD_MUSIC_MESSAGE = "Failed to open music file: ";
        static char constexpr const * COULD_NOT_LOAD_SOUND_MESSAGE = "Failed to open sound file: ";
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
