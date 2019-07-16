#ifndef DRAW_H
#define DRAW_H

#include "Logger.h" // Logging
#include "MyMath.h" // Vec
#include "MyGlut.h" // Enable/Disable Textures

/** @brief Enums corresponding to the appearance of drawn objects.
    @details The desired drawing call is
    @code{.cpp}
        DrawObject<Appearance::STYLE>(...)
    @endcode
    To allow for this, these appearances are stored in this enum and are passed to drawing
    functions as a template parameter. I'm not sure if this is the ideal implementation method.

    To allow for both colors and textures to be applied, they are stored in the same enum despite
    being treated differently. The enums "LAST_COLOR__" and "LAST_TEXTURE__" are ONLY to be used by
    the Drawing::isColor, and Drawing::isTexture methods. Unfortunately this implementation detail
    can't be hidden with this infrastructure.
    @todo Implement drawing (color change) with alpha channel.
    @see Drawing::isColor, Drawing::isTexture.
 */
enum Appearance{CUSTOM, MAROON, DARK_RED, BROWN, FIREBRICK, CRIMSON, RED, TOMATO, CORAL, INDIAN_RED, LIGHT_CORAL, DARK_SALMON, SALMON, LIGHT_SALMON, ORANGE_RED, DARK_ORANGE, ORANGE, GOLD, DARK_GOLDEN_ROD, GOLDEN_ROD, PALE_GOLDEN_ROD, DARK_KHAKI, KHAKI, OLIVE, YELLOW, YELLOW_GREEN, DARK_OLIVE_GREEN, OLIVE_DRAB, LAWN_GREEN, CHART_REUSE, GREEN_YELLOW, DARK_GREEN, GREEN, FOREST_GREEN, LIME, LIME_GREEN, LIGHT_GREEN, PALE_GREEN, DARK_SEA_GREEN, MEDIUM_SPRING_GREEN, SPRING_GREEN, SEA_GREEN, MEDIUM_AQUA_MARINE, MEDIUM_SEA_GREEN, LIGHT_SEA_GREEN, DARK_SLATE_GRAY, TEAL, DARK_CYAN, AQUA, CYAN, LIGHT_CYAN, DARK_TURQUOISE, TURQUOISE, MEDIUM_TURQUOISE, PALE_TURQUOISE, AQUA_MARINE, POWDER_BLUE, CADET_BLUE, STEEL_BLUE, CORN_FLOWER_BLUE, DEEP_SKY_BLUE, DODGER_BLUE, LIGHT_BLUE, SKY_BLUE, LIGHT_SKY_BLUE, MIDNIGHT_BLUE, NAVY, DARK_BLUE, MEDIUM_BLUE, BLUE, ROYAL_BLUE, BLUE_VIOLET, INDIGO, DARK_SLATE_BLUE, SLATE_BLUE, MEDIUM_SLATE_BLUE, MEDIUM_PURPLE, DARK_MAGENTA, DARK_VIOLET, DARK_ORCHID, MEDIUM_ORCHID, PURPLE, THISTLE, PLUM, VIOLET, MAGENTA, ORCHID, MEDIUM_VIOLET_RED, PALE_VIOLET_RED, DEEP_PINK, HOT_PINK, LIGHT_PINK, PINK, ANTIQUE_WHITE, BEIGE, BISQUE, BLANCHED_ALMOND, WHEAT, CORN_SILK, LEMON_CHIFFON, LIGHT_GOLDEN_ROD_YELLOW, LIGHT_YELLOW, SADDLE_BROWN, SIENNA, CHOCOLATE, PERU, SANDY_BROWN, BURLY_WOOD, TAN, ROSY_BROWN, MOCCASIN, NAVAJO_WHITE, PEACH_PUFF, MISTY_ROSE, LAVENDER_BLUSH, LINEN, OLD_LACE, PAPAYA_WHIP, SEA_SHELL, MINT_CREAM, SLATE_GRAY, LIGHT_SLATE_GRAY, LIGHT_STEEL_BLUE, LAVENDER, FLORAL_WHITE, ALICE_BLUE, GHOST_WHITE, HONEYDEW, IVORY, AZURE, SNOW, BLACK, DIM_GRAY, GRAY, DARK_GRAY, SILVER, LIGHT_GRAY, GAINSBORO, WHITE_SMOKE, WHITE, LAST_COLOR__,
/* enum Textures */ BUTTON,
            SKYBOX_PX, SKYBOX_PY, SKYBOX_PZ, SKYBOX_MX, SKYBOX_MY, SKYBOX_MZ,
            LION, PALFORE, FACE,
            GRASS, TREE_TOP, BARK, FOOD,
            GAME_TITLE, LAST_TEXTURE__};

class Drawing {
    static constexpr const char * UNKNOWN_DIMENSION_MESSAGE = "Invalid number of dimensions, defaulting to 2D.";
    static constexpr const char * INVALID_DRAWING_ORDER_MESSAGE = "Cannot render 2D GFramework before 3D.";
    static constexpr const char * INVALID_COLOR_MESSAGE = "Color is not valid.";
    static constexpr const char * INVALID_TEXTURE_MESSAGE = "Texture is not valid.";
    static constexpr const char * UNKNOWN_APPROVED_COLOR_MESSAGE = "Approved color not found.";
    static constexpr const char * UNKNOWN_APPROVED_TEXTURE_MESSAGE = "Approved texture not found.";

    public:
        enum class Dimension: unsigned int {NONE=0, TWO=2, THREE=3};  ///< Specifies the dimensionality for drawing purposes.

        /** @brief Changes the Appearance::CUSTOM colors to the specified value.
         *
         * @param r The [0,1] value representing the r (red) value of the color.
         * @param g The [0,1] value representing the g (green) value of the color.
         * @param b The [0,1] value representing the b (blue) value of the color.
         * @return void
         */
        static void changeCustomColor(double r, double g, double b);

        /** @brief Changes the Appearance::CUSTOM colors to the specified value.
         *
         * @param color The vector representing the RGB values.
         * @return void
         */
        static void changeCustomColor(Vec color);

        /** @brief Prepares drawing for the specified dimension.
         *
         * @param d Dimension The desired drawing dimension.
         * @return void
         *
         */
        static void enableND(Dimension d);


        /** @brief Prepares the drawing for 2 dimensional drawing.
         *
         * @return void
         *
         */
        static void enable2D();


        /** @brief Prepares the drawing for 2 dimensional drawing.
         * @param force bool Force the conversion to 3D, supresses the FATAL warning.
         *
         * @return void
         *
         */
        static void enable3D(bool force=false);


        /** @brief Specifies the color for drawing.
         *
         * @param c Vec The (r,g,b) values for the color.
         * @return void
         *
         */
        static void changeColor(Vec c);


        /** @brief Specifies the color for drawing.
         *
         * @param C Appearance The Appearance enum for the color.
         * @return void
         * @see Appearance
         */
        static void changeColor(Appearance C);


        /** @brief Specifies the texture for drawing.
         *
         * @param textureID Tex The id corresponding to the texture to draw with.
         * @return void
         *
         */
        static void changeTexture(Tex textureID);


        /** @brief Specfies the texture for drawing.
         *
         * @param C Appearance The Appearance enum corresponding to the texture to draw with.
         * @return void
         * @see Appearance
         */
        static void changeTexture(Appearance C);


        /** @brief Determines if the specified Appearance is a color.
         *
         * @param A Appearance The Appearance to check.
         * @return bool True if the Appearance is a color. False otherwise.
         *
         */
        static bool isColor(Appearance A);


        /** @brief Determines if the specified Appearance is a texture.
         *
         * @param A Appearance The Appearance to check.
         * @return bool True if the Appearance is a texture. False otherwise.
         *
         */
        static bool isTexture(Appearance A);
};



/** @brief The parent class of items which can drawn.
 * @note This parent class ensures that all Drawing methods correctly call the required precursory methods.
 * @tparam A The appearance to be used when drawing.
 * @warning Not all children will know how to use textures.
 * This may be fine, but it might also log a warning or crash the program.
 * @todo Allow parameters to be specified as by percentage or by pixel.
 */
template <Appearance A>
class DrawItem {
    private:
        static constexpr const char * INVALID_APPEARANCE_MESSAGE = "Appearance is not valid.";

    protected:

        /** @brief Applies the appearance. Subclasses are used to draw shapes.
         * @details Constructor is used as decorator. This will apply the texture before the drawing is performed.
         * @param dim Drawing::Dimension
         *
         */
        DrawItem(Drawing::Dimension dim) {
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
        ~DrawItem() {}
};

#endif // DRAW_H
