#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include "Logger.h"
#include "MyMath.h"

/** @brief The audio engine.
 * @details This class is incomplete. It needs more options! Mono vs Stereo, disable spatialization, loop, attenuation, min distance, CLEAR WHEN DONE.
 *
 * @todo This class does not clear sounds when they are done. This means that the sounds vector will grow unbounded.
 * This is wastefull and should be resolved.
 * @todo Protect / warn against using stero sounds in 3D. (not allowed according to SFML).
 */
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

        /** @brief
         *
         * @param soundFile std::string
         * @param x double
         * @param y double
         * @param z double
         * @return void
         *
         * @warning Sterio sounds will not be rendered in 3D space.
         */
        void playSound(std::string soundFile, double x, double y, double z);
        void playSound(std::string soundFile, Vec position);
        void playSound(std::string soundFile);
        void clearStoppedSounds();
};

#endif // AUDIO_H
