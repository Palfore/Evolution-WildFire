#ifndef GLUTCALLBACKS_H
#define GLUTCALLBACKS_H

/** @namespace glutCB
 * @brief This namespace contains the glut callback functions.
 * @details These must be bound functions and therefore cannot appear inside of a class. They are used
 *          once in the GFramework class.
 *
 */
namespace glutCB {
    /** @brief Prepares the drawing state, then calls the simulation at a constant FPS.
     * @return void
     *
     * @bug The frame rate has some issues and is not consistent on some other machines.
     */
    void renderScene();

    /** @brief Updates the audio, window size, userfunctions (ie: calls them), and camera.
     *
     * @return void
     *
     * @note Kind of messy, needs clean up.
     */
    void update();


    /** @brief Updates the opengl viewport etc on window resize.
     *
     * @param w int The new width of the window.
     * @param h int The new height of the window.
     * @return void
     *
     */
    void changeSize(int w, int h);


    /** @brief Is called on mouse click.
     *
     * @details Possible buttons include GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, and GLUT_RIGHT_BUTTON.
     * @param button int Which mouse button is clicked.
     * @param state int The state of the click (release, or press).
     * @param mx int The x position of the mouse.
     * @param my int The y position of the mouse.
     * @return void
     *
     * @warning Not yet implemented.
     */
    void callMouse(int button, int state, int mx, int my);


    /** @brief Is called on mouse movement.
     *
     * @param mx int The x position of the mouse.
     * @param my int The y position of the mouse.
     * @return void
     *
     * @warning Not yet implemented.
     */
    void mouseMove(int mx, int my);

    /** @brief Continuously called to update details about the mouse.
     * @details Specifically it update the GFramework mouse, and resets the GFramework camera.del in the case that the mouse is used to
     * control the camera.
     *
     * @param mx int The x position of the mouse.
     * @param my int The y position of the mouse.
     * @return void
     *
     * @note Not fully implemented.
     */
    void passiveMouse(int mx, int my);


    /** @brief Sets the state of the UserInput keystates for the pressed key and calls its associated pressed function.
     *
     * @param key unsigned char
     * @param x int
     * @param y int
     * @return void
     *
     * @note This only works for 'normal', ascii characters. @see pressSpecialKey for handling special characters.
     */
    void keyPressed(unsigned char key, int x, int y);


    /** @brief Resets the state of the UserInput keystates for the released key and calls its associated release function.
     *
     * @param key unsigned char
     * @param x int
     * @param y int
     * @return void
     *
     * @note This only works for 'normal', ascii characters. @see pressSpecialKey for handling special characters.
     */
    void keyUp(unsigned char key, int x, int y);


    /** @brief Calls special key  (ex: Arrow keys) press functions.
     *
     * @param key int The glut key pressed.
     * @param kxx int The y position of the mouse at the time the key is pressed.
     * @param kyy int The y position of the mouse at the time the key is pressed.
     * @return void
     *
     * @see https://www.opengl.org/resources/libraries/glut/spec3/node54.html for the avaible special keys.
     */
    void pressSpecialKey(int key, int kxx, int kyy);


    /** @brief Calls the special key  (ex: Arrow keys) release functions.
     *
     * @param key int The glut key released.
     * @param kx int The x position of the mouse at the time the key is pressed.
     * @param ky int The y position of the mouse at the time the key is pressed.
     * @return void
     *
     * @see https://www.opengl.org/resources/libraries/glut/spec3/node54.html for the avaible special keys.
     */
    void releaseSpecialKey(int key, int kx, int ky);
}

#endif // GLUTCALLBACKS_H
