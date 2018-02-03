#include "Audio/Audio.h"
#include "Config.h"

Audio::Audio() : music(), soundBuffers(), sounds()  {
    sf::Listener::setUpVector(0, 0, 1);
    if (!music.openFromFile(std::string(AUDIO_DIRECTORY) + "Sunday_Spirit.wav")) LOG(COULD_NOT_LOAD_MUSIC_MESSAGE + std::string("Sunday_Spirit.wav") + '.', LogDegree::WARNING, LogType::AUDIO);

    std::array<std::string, 1> soundFiles = {"gunShot.wav"};
    for (unsigned int i = 0; i < soundFiles.size(); i++) {
        soundBuffers.insert({soundFiles.at(i), sf::SoundBuffer()});
        if (!soundBuffers.at(soundFiles.at(i)).loadFromFile(std::string(AUDIO_DIRECTORY) + soundFiles[i])) {
            LOG(COULD_NOT_LOAD_MUSIC_MESSAGE + soundFiles[i] + '.', LogDegree::WARNING, LogType::AUDIO);
        }
    }
    if (CONFIG_AUDIO()) {
        music.play();
    }
}

Audio::~Audio() {
    music.stop();
}

void Audio::playSound(std::string soundFile) {playSound(soundFile, 0, 0, 0);}
void Audio::playSound(std::string soundFile, double x, double y, double z) {playSound(soundFile, Vec(x, y, z));}
void Audio::playSound(std::string soundFile, Vec position) {
    if (CONFIG_AUDIO()) {
        sounds.push_back(sf::Sound());

        sounds[sounds.size() - 1].setMinDistance(10);
        sounds[sounds.size() - 1].setAttenuation(0.5);
        sounds[sounds.size() - 1].setPosition(position.x, position.y, position.z);
        sounds[sounds.size() - 1].setBuffer(soundBuffers.at(soundFile));
        sounds[sounds.size() - 1].play();
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
