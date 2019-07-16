#include "GFramework.h"

/// The rgb values are stored here. At the end of this function, they are all divided by 255
/// to get the values onto [0,1], which is required by opengl.
void GFramework::loadColors() {
    this->colorMap = {
        {Appearance::CUSTOM, Vec(255, 255, 255)},
        {Appearance::MAROON, Vec(128,0,0)},
        {Appearance::DARK_RED, Vec(139,0,0)},
        {Appearance::BROWN, Vec(165,42,42)},
        {Appearance::FIREBRICK, Vec(178,34,34)},
        {Appearance::CRIMSON, Vec(220,20,60)},
        {Appearance::RED, Vec(255,0,0)},
        {Appearance::TOMATO, Vec(255,99,71)},
        {Appearance::CORAL, Vec(255,127,80)},
        {Appearance::INDIAN_RED, Vec(205,92,92)},
        {Appearance::LIGHT_CORAL, Vec(240,128,128)},
        {Appearance::DARK_SALMON, Vec(233,150,122)},
        {Appearance::SALMON, Vec(250,128,114)},
        {Appearance::LIGHT_SALMON, Vec(255,160,122)},
        {Appearance::ORANGE_RED, Vec(255,69,0)},
        {Appearance::DARK_ORANGE, Vec(255,140,0)},
        {Appearance::ORANGE, Vec(255,165,0)},
        {Appearance::GOLD, Vec(255,215,0)},
        {Appearance::DARK_GOLDEN_ROD, Vec(184,134,11)},
        {Appearance::GOLDEN_ROD, Vec(218,165,32)},
        {Appearance::PALE_GOLDEN_ROD, Vec(238,232,170)},
        {Appearance::DARK_KHAKI, Vec(189,183,107)},
        {Appearance::KHAKI, Vec(240,230,140)},
        {Appearance::OLIVE, Vec(128,128,0)},
        {Appearance::YELLOW, Vec(255,255,0)},
        {Appearance::YELLOW_GREEN, Vec(154,205,50)},
        {Appearance::DARK_OLIVE_GREEN, Vec(85,107,47)},
        {Appearance::OLIVE_DRAB, Vec(107,142,35)},
        {Appearance::LAWN_GREEN, Vec(124,252,0)},
        {Appearance::CHART_REUSE, Vec(127,255,0)},
        {Appearance::GREEN_YELLOW, Vec(173,255,47)},
        {Appearance::DARK_GREEN, Vec(0,100,0)},
        {Appearance::GREEN, Vec(0,128,0)},
        {Appearance::FOREST_GREEN, Vec(34,139,34)},
        {Appearance::LIME, Vec(0,255,0)},
        {Appearance::LIME_GREEN, Vec(50,205,50)},
        {Appearance::LIGHT_GREEN, Vec(144,238,144)},
        {Appearance::PALE_GREEN, Vec(152,251,152)},
        {Appearance::DARK_SEA_GREEN, Vec(143,188,143)},
        {Appearance::MEDIUM_SPRING_GREEN, Vec(0,250,154)},
        {Appearance::SPRING_GREEN, Vec(0,255,127)},
        {Appearance::SEA_GREEN, Vec(46,139,87)},
        {Appearance::MEDIUM_AQUA_MARINE, Vec(102,205,170)},
        {Appearance::MEDIUM_SEA_GREEN, Vec(60,179,113)},
        {Appearance::LIGHT_SEA_GREEN, Vec(32,178,170)},
        {Appearance::DARK_SLATE_GRAY, Vec(47,79,79)},
        {Appearance::TEAL, Vec(0,128,128)},
        {Appearance::DARK_CYAN, Vec(0,139,139)},
        {Appearance::AQUA, Vec(0,255,255)},
        {Appearance::CYAN, Vec(0,255,255)},
        {Appearance::LIGHT_CYAN, Vec(224,255,255)},
        {Appearance::DARK_TURQUOISE, Vec(0,206,209)},
        {Appearance::TURQUOISE, Vec(64,224,208)},
        {Appearance::MEDIUM_TURQUOISE, Vec(72,209,204)},
        {Appearance::PALE_TURQUOISE, Vec(175,238,238)},
        {Appearance::AQUA_MARINE, Vec(127,255,212)},
        {Appearance::POWDER_BLUE, Vec(176,224,230)},
        {Appearance::CADET_BLUE, Vec(95,158,160)},
        {Appearance::STEEL_BLUE, Vec(70,130,180)},
        {Appearance::CORN_FLOWER_BLUE, Vec(100,149,237)},
        {Appearance::DEEP_SKY_BLUE, Vec(0,191,255)},
        {Appearance::DODGER_BLUE, Vec(30,144,255)},
        {Appearance::LIGHT_BLUE, Vec(173,216,230)},
        {Appearance::SKY_BLUE, Vec(135,206,235)},
        {Appearance::LIGHT_SKY_BLUE, Vec(135,206,250)},
        {Appearance::MIDNIGHT_BLUE, Vec(25,25,112)},
        {Appearance::NAVY, Vec(0,0,128)},
        {Appearance::DARK_BLUE, Vec(0,0,139)},
        {Appearance::MEDIUM_BLUE, Vec(0,0,205)},
        {Appearance::BLUE, Vec(0,0,255)},
        {Appearance::ROYAL_BLUE, Vec(65,105,225)},
        {Appearance::BLUE_VIOLET, Vec(138,43,226)},
        {Appearance::INDIGO, Vec(75,0,130)},
        {Appearance::DARK_SLATE_BLUE, Vec(72,61,139)},
        {Appearance::SLATE_BLUE, Vec(106,90,205)},
        {Appearance::MEDIUM_SLATE_BLUE, Vec(123,104,238)},
        {Appearance::MEDIUM_PURPLE, Vec(147,112,219)},
        {Appearance::DARK_MAGENTA, Vec(139,0,139)},
        {Appearance::DARK_VIOLET, Vec(148,0,211)},
        {Appearance::DARK_ORCHID, Vec(153,50,204)},
        {Appearance::MEDIUM_ORCHID, Vec(186,85,211)},
        {Appearance::PURPLE, Vec(128,0,128)},
        {Appearance::THISTLE, Vec(216,191,216)},
        {Appearance::PLUM, Vec(221,160,221)},
        {Appearance::VIOLET, Vec(238,130,238)},
        {Appearance::MAGENTA, Vec(255,0,255)},
        {Appearance::ORCHID, Vec(218,112,214)},
        {Appearance::MEDIUM_VIOLET_RED, Vec(199,21,133)},
        {Appearance::PALE_VIOLET_RED, Vec(219,112,147)},
        {Appearance::DEEP_PINK, Vec(255,20,147)},
        {Appearance::HOT_PINK, Vec(255,105,180)},
        {Appearance::LIGHT_PINK, Vec(255,182,193)},
        {Appearance::PINK, Vec(255,192,203)},
        {Appearance::ANTIQUE_WHITE, Vec(250,235,215)},
        {Appearance::BEIGE, Vec(245,245,220)},
        {Appearance::BISQUE, Vec(255,228,196)},
        {Appearance::BLANCHED_ALMOND, Vec(255,235,205)},
        {Appearance::WHEAT, Vec(245,222,179)},
        {Appearance::CORN_SILK, Vec(255,248,220)},
        {Appearance::LEMON_CHIFFON, Vec(255,250,205)},
        {Appearance::LIGHT_GOLDEN_ROD_YELLOW, Vec(250,250,210)},
        {Appearance::LIGHT_YELLOW, Vec(255,255,224)},
        {Appearance::SADDLE_BROWN, Vec(139,69,19)},
        {Appearance::SIENNA, Vec(160,82,45)},
        {Appearance::CHOCOLATE, Vec(210,105,30)},
        {Appearance::PERU, Vec(205,133,63)},
        {Appearance::SANDY_BROWN, Vec(244,164,96)},
        {Appearance::BURLY_WOOD, Vec(222,184,135)},
        {Appearance::TAN, Vec(210,180,140)},
        {Appearance::ROSY_BROWN, Vec(188,143,143)},
        {Appearance::MOCCASIN, Vec(255,228,181)},
        {Appearance::NAVAJO_WHITE, Vec(255,222,173)},
        {Appearance::PEACH_PUFF, Vec(255,218,185)},
        {Appearance::MISTY_ROSE, Vec(255,228,225)},
        {Appearance::LAVENDER_BLUSH, Vec(255,240,245)},
        {Appearance::LINEN, Vec(250,240,230)},
        {Appearance::OLD_LACE, Vec(253,245,230)},
        {Appearance::PAPAYA_WHIP, Vec(255,239,213)},
        {Appearance::SEA_SHELL, Vec(255,245,238)},
        {Appearance::MINT_CREAM, Vec(245,255,250)},
        {Appearance::SLATE_GRAY, Vec(112,128,144)},
        {Appearance::LIGHT_SLATE_GRAY, Vec(119,136,153)},
        {Appearance::LIGHT_STEEL_BLUE, Vec(176,196,222)},
        {Appearance::LAVENDER, Vec(230,230,250)},
        {Appearance::FLORAL_WHITE, Vec(255,250,240)},
        {Appearance::ALICE_BLUE, Vec(240,248,255)},
        {Appearance::GHOST_WHITE, Vec(248,248,255)},
        {Appearance::HONEYDEW, Vec(240,255,240)},
        {Appearance::IVORY, Vec(255,255,240)},
        {Appearance::AZURE, Vec(240,255,255)},
        {Appearance::SNOW, Vec(255,250,250)},
        {Appearance::BLACK, Vec(0,0,0)},
        {Appearance::DIM_GRAY, Vec(105,105,105)},
        {Appearance::GRAY, Vec(128,128,128)},
        {Appearance::DARK_GRAY, Vec(169,169,169)},
        {Appearance::SILVER, Vec(192,192,192)},
        {Appearance::LIGHT_GRAY, Vec(211,211,211)},
        {Appearance::GAINSBORO, Vec(220,220,220)},
        {Appearance::WHITE_SMOKE, Vec(245,245,245)},
        {Appearance::WHITE, Vec(255,255,255)}
    };
    for (auto& c: colorMap) {
        c.second /= 255;
    }
}