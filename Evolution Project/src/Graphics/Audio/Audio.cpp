#include "Audio/Audio.h"
//Sound::Sound(std::string file) : Sound(file, 0, 0, 0) {}
//Sound::Sound(std::string file, double x, double y, double z) : Sound(file, Vec(x, y, z)) {}
//Sound::Sound(std::string file, Vec position) : buffer(), sound(), loc(position), played(false) {
//
//}
//Sound::~Sound() {
//    sound.stop();
//}

Audio::Audio() : music(), soundBuffers(), sounds()  {
    sf::Listener::setUpVector(0, 0, 1);
    if (!music.openFromFile(std::string(AUDIO_DIRECTORY) + "Sunday_Spirit.wav")) LOG(LogDegree::WARNING, LogType::AUDIO, "Failed to open music.");

    std::array<std::string, 1> soundFiles = {"gunShot.wav"};
    for (unsigned int i = 0; i < soundFiles.size(); i++) {
        soundBuffers.insert({soundFiles.at(i), sf::SoundBuffer()});
        if (!soundBuffers.at(soundFiles.at(i)).loadFromFile(std::string(AUDIO_DIRECTORY) + soundFiles[i])) {
            LOG(LogDegree::WARNING, LogType::AUDIO, "Failed to open sound.");
        }
    }
    music.play();
}

Audio::~Audio() {
    music.stop();
}

void Audio::playSound(std::string soundFile) {playSound(soundFile, 0, 0, 0);}
void Audio::playSound(std::string soundFile, double x, double y, double z) {playSound(soundFile, Vec(x, y, z));}
void Audio::playSound(std::string soundFile, Vec position) {
    sounds.push_back(sf::Sound());

    sounds[sounds.size() - 1].setMinDistance(10);
    sounds[sounds.size() - 1].setAttenuation(0.5);
    sounds[sounds.size() - 1].setPosition(position.x, position.y, position.z);
    sounds[sounds.size() - 1].setBuffer(soundBuffers.at(soundFile));
    sounds[sounds.size() - 1].play();
}
void Audio::clearStoppedSounds() {
//    sounds.erase(std::remove_if(sounds.begin(),
//                                sounds.end(),
//                                [](Sound s){
//                                    return s.played && (s.sound.getStatus() == sf::SoundSource::Status::Stopped);
//                                }),
//                 sounds.end());
}
