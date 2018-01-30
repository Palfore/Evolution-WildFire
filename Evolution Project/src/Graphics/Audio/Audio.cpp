#include "Audio/Audio.h"

Audio::Audio() : music(), sounds({}) {
    if (!music.openFromFile("assets/Audio/Sunday_Spirit.wav"))
        LOG(LogDegree::WARNING, LogType::AUDIO, "Failed to open music.");
    music.play();
}

Audio::~Audio() {
    music.stop();
}
