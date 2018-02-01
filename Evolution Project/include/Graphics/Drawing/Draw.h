#ifndef DRAW_H
#define DRAW_H

#include "Logger.h" // Logging
#include "MyMath.h" // Vec
#include "MyGlut.h" // Enable/Disable Textures

enum class Dimension: unsigned int {NONE=0, TWO=2, THREE=3};
enum Appearance{MAROON, DARK_RED, BROWN, FIREBRICK, CRIMSON, RED, TOMATO, CORAL, INDIAN_RED, LIGHT_CORAL, DARK_SALMON, SALMON, LIGHT_SALMON, ORANGE_RED, DARK_ORANGE, ORANGE, GOLD, DARK_GOLDEN_ROD, GOLDEN_ROD, PALE_GOLDEN_ROD, DARK_KHAKI, KHAKI, OLIVE, YELLOW, YELLOW_GREEN, DARK_OLIVE_GREEN, OLIVE_DRAB, LAWN_GREEN, CHART_REUSE, GREEN_YELLOW, DARK_GREEN, GREEN, FOREST_GREEN, LIME, LIME_GREEN, LIGHT_GREEN, PALE_GREEN, DARK_SEA_GREEN, MEDIUM_SPRING_GREEN, SPRING_GREEN, SEA_GREEN, MEDIUM_AQUA_MARINE, MEDIUM_SEA_GREEN, LIGHT_SEA_GREEN, DARK_SLATE_GRAY, TEAL, DARK_CYAN, AQUA, CYAN, LIGHT_CYAN, DARK_TURQUOISE, TURQUOISE, MEDIUM_TURQUOISE, PALE_TURQUOISE, AQUA_MARINE, POWDER_BLUE, CADET_BLUE, STEEL_BLUE, CORN_FLOWER_BLUE, DEEP_SKY_BLUE, DODGER_BLUE, LIGHT_BLUE, SKY_BLUE, LIGHT_SKY_BLUE, MIDNIGHT_BLUE, NAVY, DARK_BLUE, MEDIUM_BLUE, BLUE, ROYAL_BLUE, BLUE_VIOLET, INDIGO, DARK_SLATE_BLUE, SLATE_BLUE, MEDIUM_SLATE_BLUE, MEDIUM_PURPLE, DARK_MAGENTA, DARK_VIOLET, DARK_ORCHID, MEDIUM_ORCHID, PURPLE, THISTLE, PLUM, VIOLET, MAGENTA, ORCHID, MEDIUM_VIOLET_RED, PALE_VIOLET_RED, DEEP_PINK, HOT_PINK, LIGHT_PINK, PINK, ANTIQUE_WHITE, BEIGE, BISQUE, BLANCHED_ALMOND, WHEAT, CORN_SILK, LEMON_CHIFFON, LIGHT_GOLDEN_ROD_YELLOW, LIGHT_YELLOW, SADDLE_BROWN, SIENNA, CHOCOLATE, PERU, SANDY_BROWN, BURLY_WOOD, TAN, ROSY_BROWN, MOCCASIN, NAVAJO_WHITE, PEACH_PUFF, MISTY_ROSE, LAVENDER_BLUSH, LINEN, OLD_LACE, PAPAYA_WHIP, SEA_SHELL, MINT_CREAM, SLATE_GRAY, LIGHT_SLATE_GRAY, LIGHT_STEEL_BLUE, LAVENDER, FLORAL_WHITE, ALICE_BLUE, GHOST_WHITE, HONEYDEW, IVORY, AZURE, SNOW, BLACK, DIM_GRAY, GRAY, DARK_GRAY, SILVER, LIGHT_GRAY, GAINSBORO, WHITE_SMOKE, WHITE, LAST_COLOR__,
/* enum Textures */ GRASS, BUTTON, LAST_TEXTURE__};

struct Drawing {
    static constexpr const char * UNKNOWN_DIMENSION_MESSAGE = "Invalid number of dimensions, defaulting to 2D.";
    static constexpr const char * INVALID_DRAWING_ORDER_MESSAGE = "Cannot render 2D graphics before 3D.";
    static constexpr const char * INVALID_COLOR_MESSAGE = "Color is not valid.";
    static constexpr const char * INVALID_TEXTURE_MESSAGE = "Texture is not valid.";
    static constexpr const char * UNKNOWN_APPROVED_COLOR_MESSAGE = "Approved color not found.";
    static constexpr const char * UNKNOWN_APPROVED_TEXTURE_MESSAGE = "Approved texture not found.";

    static void enableND(Dimension d);
    static void enable2D();
    static void enable3D();
    static void changeColor(Vec c);
    static void changeColor(Appearance C);
    static void changeTexture(Tex textureID);
    static void changeTexture(Appearance C);
    static bool isColor(Appearance A);
    static bool isTexture(Appearance A);
};

template <Appearance A>
class DrawItem {
    static constexpr const char * INVALID_APPEARANCE_MESSAGE = "Appearance is not valid.";

    public:
    DrawItem(Dimension dim) {
        Drawing::enableND(dim);

        /* Apply Appearance */
        if (Drawing::isTexture(A)) {
            Drawing::changeColor(Appearance::WHITE);
            glEnable(GL_TEXTURE_2D);
            Drawing::changeTexture(A);
        } else if (Drawing::isColor(A)) {
            glDisable(GL_TEXTURE_2D);
            Drawing::changeColor(A);
        } else {
            LOG(INVALID_APPEARANCE_MESSAGE, LogDegree::FATAL, LogType::GRAPHICS);
        }
    }
};

#endif // DRAW_H
