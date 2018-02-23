#include "GFramework.h"
#include "Logger.h"
#include "utility.h"

void GFramework::loadTextures() {
    std::string dir = "assets/Images/";
    textureMap = {
        {Appearance::GRASS, glLoadTexture(dir + "grass.jpg")},
        {Appearance::BUTTON, glLoadTexture(dir + "button.png")},
    };
    for (auto const& t : textureMap) {
        if (!t.second) LOG("Could not load texture #" + utility::numToStr<int>(t.first) + ".", LogDegree::FATAL, LogType::GRAPHICS);
    }
}
