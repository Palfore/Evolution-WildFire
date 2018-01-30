#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include "Logger.h"
class Audio {
    public:
        sf::Music music;
        std::vector<sf::Sound> sounds;
        Audio();
        virtual ~Audio();

    protected:

    private:
};

#endif // AUDIO_H
