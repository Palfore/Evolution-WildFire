#include "GFramework.h"
#include "Logger.h"
#include "utility.h"

void GFramework::loadTextures() {
    std::string dir = "assets/Images/";
    textureMap = {
        {Appearance::GRASS, glLoadTexture(dir + "grass1.jpg")},
        {Appearance::BUTTON, glLoadTexture(dir + "button1.png")},
        {Appearance::SKYBOX_PX, glLoadTexture(dir + "Skybox/posx.bmp")},
        {Appearance::SKYBOX_PY, glLoadTexture(dir + "Skybox/posy.bmp")},
        {Appearance::SKYBOX_PZ, glLoadTexture(dir + "Skybox/posz.bmp")},
        {Appearance::SKYBOX_MX, glLoadTexture(dir + "Skybox/negx.bmp")},
        {Appearance::SKYBOX_MY, glLoadTexture(dir + "Skybox/negy.bmp")},
        {Appearance::SKYBOX_MZ, glLoadTexture(dir + "Skybox/negz.bmp")},
        {Appearance::LION, glLoadTexture(dir + "download.png")},
        {Appearance::GAME_TITLE, glLoadTexture(dir + "title.png")},
        {Appearance::PALFORE, glLoadTexture(dir + "PalforeLogo.png")},
        {Appearance::FACE, glLoadTexture(dir + "face.png")},
        {Appearance::TREE_TOP, glLoadTexture(dir + "treetop2.jpg")},
        {Appearance::BARK, glLoadTexture(dir + "bark.jpg")},
        {Appearance::FOOD, glLoadTexture(dir + "chocolait.jpg")},
    };
    for (auto const& t : textureMap) {
        if (!t.second) LOG("Could not load texture #" + utility::numToStr<int>(t.first) + ".", LogDegree::FATAL, LogType::GRAPHICS);
    }
}
