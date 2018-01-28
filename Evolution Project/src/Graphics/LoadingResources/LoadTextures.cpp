#include "Graphics.h"
#include "Logger.h"

void Graphics::loadTextures() {
    std::string dir = "assets/Images/";
    textureMap = {
        {Appearance::GRASS, glLoadTexture(dir + "grass.jpg")},
        {Appearance::BUTTON, glLoadTexture(dir + "button.png")},
    };
    for (auto const& t : textureMap) {
        if (!t.second) LOG(LogDegree::FATAL, LogType::GRAPHICS, "Could not load texture.");
    }
}
