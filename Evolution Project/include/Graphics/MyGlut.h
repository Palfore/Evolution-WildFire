/** @file
 * @brief This file contains some useful extension for glut.
 * @details Specifically SOIL and texture wrappers which make certain tasks much easier.
 */

#ifdef __APPLE__
#include <GLUT/freeglut.h>
#else
#include <GL/freeglut.h>
#endif

#ifndef MYGLUT_H_INCLUDED
#define MYGLUT_H_INCLUDED

#include "MyMath.h" // vec, vec2

typedef GLuint Tex; ///< Used to refer to textures (used in the SOIL library).


/** @brief Loads a texture using SOIL and returns a reference to it.
 *
 * @param fileName std::string The name (path) of the image file to be loaded.
 * @return Tex The reference to the texture.
 *
 */
Tex glLoadTexture(std::string fileName);


/** @brief Saves the current scene as a .png and stores it in the screenshots folder.
 *
 * @return void
 *
 * @note Although it is saved as a .png, the SOIL implementation saves it as a BMP.
 * This doesn't seem to cause any problems.
 */
void saveScreenShot();


/** @brief Draws a 2D textured vertex.
 *
 * @param t Vec2 The texture coordinates (both should be [0, 1]).
 * @param v Vec2 The relative window coordinates (should be within the window dimensions).
 * @return void
 *
 * @note No bounds checking on input parameters since shapes might start out of bounds for a reason, but still be drawn in the scene.
 */
void glTexVert2f(Vec2 t, Vec2 v);


/** @brief Draws a 2D textured vertex.
 *
 * @param tx double The x texture coordinate (should be [0, 1]).
 * @param ty double The y texture coordinate (should be [0, 1]).
 * @param v Vec2 The relative window coordinates of the vertex (should be within the window dimensions).
 * @return void
 *
 * @note No bounds checking on input parameters since shapes might start out of bounds for a reason, but still be drawn in the scene.
 */
void glTexVert2f(double tx, double ty, Vec2 v);


/** @brief Draws a 2D textured vertex.
 *
 * @param t Vec2 The texture coordinates (both should be [0, 1]).
 * @param vx double The relative window x coordinate of the vertex (should be within the window width).
 * @param vy double The relative window y coordinates of the vertex (should be within the window height).
 * @return void
 *
 * @note No bounds checking on input parameters since shapes might start out of bounds for a reason, but still be drawn in the scene.
 */
void glTexVert2f(Vec2 t, double vx, double vy);


/** @brief Draws a 2D textured vertex.
 *
 * @param tx double The x texture coordinate (should be [0, 1]).
 * @param ty double The y texture coordinate (should be [0, 1]).
 * @param vx double The relative window x coordinate of the vertex (should be within the window width).
 * @param vy double The relative window y coordinates of the vertex (should be within the window height).
 * @return void
 *
 * @note No bounds checking on input parameters since shapes might start out of bounds for a reason, but still be drawn in the scene.
 */
void glTexVert2f(double tx, double ty, double vx, double vy);


/** @brief Draws a 3D textured vertex.
 *
 * @param t Vec2 The texture coordinates (both should be [0, 1]).
 * @param v Vec3 The world coordinates of the vertex.
 * @return void
 *
 * @note No bounds checking on input parameters since shapes might start out of bounds for a reason, but still be drawn in the scene.
 */
void glTexVert3f(Vec2 t, Vec v);


/** @brief Draws a 2D textured vertex.
 *
 * @param tx double The texture x coordinates (both should be [0, 1]).
 * @param ty double The texture y coordinates (both should be [0, 1]).
 * @param v Vec3 The world coordinates of the vertex.
 * @return void
 *
 * @note No bounds checking on input parameters since shapes might start out of bounds for a reason, but still be drawn in the scene.
 */
void glTexVert3f(double tx, double ty, Vec v);


/** @brief Draws a 2D textured vertex.
 *
 * @param t Vec2 The texture coordinates (both should be [0, 1]).
 * @param vx double The x world coordinate of the vertex.
 * @param vy double The y world coordinate of the vertex.
 * @param vz double The z world coordinate of the vertex.
 * @return void
 *
 * @note No bounds checking on input parameters since shapes might start out of bounds for a reason, but still be drawn in the scene.
 */
void glTexVert3f(Vec2 t, double vx, double vy, double vz);


/** @brief Draws a 2D textured vertex.
 *
 * @param tx double The x texture coordinates (should be [0, 1]).
 * @param ty double The y texture coordinates (should be [0, 1]).
 * @param vx double The x world coordinate of the vertex.
 * @param vy double The y world coordinate of the vertex.
 * @param vz double The z world coordinate of the vertex.
 * @return void
 *
 * @note No bounds checking on input parameters since shapes might start out of bounds for a reason, but still be drawn in the scene.
 */
void glTexVert3f(double tx, double ty, double vx, double vy, double vz);

#endif // MYGLUT_H_INCLUDED
