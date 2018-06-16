#include "Audio.h"
#include "Logger.h"
#include "Vec.h"

Audio::Audio(unsigned int volume_t) : volume(volume_t), isMuted(false), music(), soundBuffers(), sounds()  {
    sf::Listener::setUpVector(0, 0, 1);
    if (!music.openFromFile(std::string(AUDIO_DIRECTORY) + "Sunday_Spirit.wav")) {
        LOG(COULD_NOT_LOAD_MUSIC_MESSAGE + std::string("Sunday_Spirit.wav") + '.', LogDegree::WARNING, LogType::AUDIO);
    }

    std::array<std::string, 2> soundFiles = {"gunShot.wav", "click.wav"};
    for (unsigned int i = 0; i < soundFiles.size(); i++) {
        soundBuffers.insert({soundFiles.at(i), sf::SoundBuffer()});
        if (!soundBuffers.at(soundFiles.at(i)).loadFromFile(std::string(AUDIO_DIRECTORY) + soundFiles[i])) {
            LOG(COULD_NOT_LOAD_MUSIC_MESSAGE + soundFiles[i] + '.', LogDegree::WARNING, LogType::AUDIO);
        }
    }
    music.setVolume(this->volume);
    music.play();
}

void Audio::volumeUp(unsigned int amount) {
    this->volume = this->volume <= MAX_VOLUME - amount ? this->volume + amount : MAX_VOLUME;
    this->updateVolume();
}

void Audio::volumeDown(unsigned int amount) {
    this->volume = this->volume >= MIN_VOLUME + amount ? this->volume - amount : MIN_VOLUME;
    this->updateVolume();
}

#include <iostream>
void Audio::toggleMute() {
    this->isMuted ^= true;
    this->updateVolume();
}

void Audio::updateVolume() {
    int level = isMuted ? 0 : this->volume;
    this->music.setVolume(level);
    for (sf::Sound &sound : this->sounds) {
        sound.setVolume(level);
    }
}

Audio::~Audio() {
    music.stop();
    soundBuffers.clear();
    sounds.clear();

}

void Audio::playSound(std::string soundFile) {playSound(soundFile, 0, 0, 0);}
void Audio::playSound(std::string soundFile, double x, double y, double z) {playSound(soundFile, Vec(x, y, z));}
void Audio::playSound(std::string soundFile, Vec position) {
    if (!this->isMuted) {
        sounds.push_back(sf::Sound());

        sounds[sounds.size() - 1].setMinDistance(10);
        sounds[sounds.size() - 1].setAttenuation(2);
        sounds[sounds.size() - 1].setPosition(position.x, position.y, position.z);
        sounds[sounds.size() - 1].setBuffer(soundBuffers.at(soundFile));
        sounds[sounds.size() - 1].play(); // sounds should be a map, with all the sounds loaded, then play is called on desired sound. (?)
                                        // Then no need to clean up, since all sounds stay loaded, but instead of adding it everytime you want to play
                                        // You just replay it.
                                        // IE a Sound should be a filename, SoundBuffer, and a Sound.
                                        // Or rather a map from a filename to a {SoundBuffer, Sound}.
    }
}
void Audio::clearStoppedSounds() {
//    sounds.erase(std::remove_if(sounds.begin(),
//                                sounds.end(),
//                                [](Sound s){
//                                    return s.played && (s.sound.getStatus() == sf::SoundSource::Status::Stopped);
//                                }),
//                 sounds.end());
}
