#include "GFramework.h"

/** @mainpage Evolution WildFire
 * @tableofcontents
 * @section introduction Introduction
 *    This is a 3D evolution simulator. The hope is that in future, you will be able to control evolved
 *    3D creatures in a dynamic environment. Their evolution will change their morphology and their neural
 *    network brain to accomplish the goals you set out for them. This includes fighting other creatures,
 *    navigating a complex landscape, and completing meta-challenges to collect evolution points and evolve the
 *    best creature!
 *
 * @section usage Usage
 *     The program can be run immediately once the copy has been obtained by running the .exe in the bin/debug folder.
 *     Right now, there is no game play at all, as the software is in development.
 *
 */


/** @brief Starts up the glutmainloop through GFramework startup().
 * @details The opengl framework works by running a set of disconnected functions in a loop. To do this, the functions are
 * divided and only specific variables can be passed between them. This forces a new way of programming moving away from
 * the typical gameloop constructs. Instead the use of (a attempted minimal) usage of singletons, static and global variables
 * must be implemented. It seems that the best way forward is to create a singleton instance (of the GFramework class) which encapsulates all
 * the opengl callbacks. This class effectively acts as a new main.
 *
 * @return int Exit code, 0 for normal termination, non-zero value otherwise.
 *
 */
int main() {
    GFramework::get->startup();
}















