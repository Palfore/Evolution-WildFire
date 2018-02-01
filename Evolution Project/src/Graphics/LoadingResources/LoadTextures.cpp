#include "Graphics.h"
#include "Logger.h"
#include "expected.h"

void Graphics::loadTextures() {
    std::string dir = "assets/Images/";
    textureMap = {
        {Appearance::GRASS, glLoadTexture(dir + "grass.jpg")},
        {Appearance::BUTTON, glLoadTexture(dir + "button.png")},
    };
    for (auto const& t : textureMap) {
        if (!t.second) LOG("Could not load texture #" + expected::numToStr<int>(t.first) + ".", LogDegree::FATAL, LogType::GRAPHICS);
    }
}
