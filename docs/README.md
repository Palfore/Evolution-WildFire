# Evolution WildFire

Evolution WildFire is a 3D interactive evolution simulator. Creatures are created by randomly assembling a set of mechanical components. These creatures are then run through the Genetic Algorithm, where the strong survive, and the weak... well let's not worry about them. Hopes for this project include, neural network control for the activation of creature muscles, as well as creating a populated 3D world in which a user can control and manipulate their creature, evolving it as it collects resources and fights off other hungry creatures.  

![3D Terrain with creature in center shot](https://github.com/Palfore/Evolution-Project/blob/master/docs/3D%20Terrain%20w%20Creature.jpg "Evolution WildFire")

## Getting Started

A semi-major priority in this project is to keep dependencies managed. As in, you should be able to simply copy the project onto a local drive and run & it right away. Unfortunetly, some of these dependencies are too large to reasonably store along with the rest of the project.

### Prerequisites

This project is built using [Codeblocks](http://www.codeblocks.org/) as the IDE. For other IDEs feel free to try to figure it out!

### Installing

Once this is installed the only dependency that must be managed by the user is the openGL/freeglut library. The [installation of freeglut](http://wiki.codeblocks.org/index.php/Using_FreeGlut_with_Code::Blocks) is relatively simple. 

If you place CodeBlocks in a weird place, you may get some linking errors and you'll have to ensure that codeblocks links everything up properly given your paths.

Then you can just load up the Evolution Project.cbp and hit run (or rebuild then run). A nice main menu should appear. If not the assets/logger.log should tell you at what point it failed.

## Running the tests

Awkward, no automated tests yet...

## Built With

* [Codeblocks](http://www.codeblocks.org/) Integrated Devlopment Environment
* [Simple and Fast Multimedia Library (SFML)](https://www.sfml-dev.org/index.php) Audio & Networking Library
* [freeglut](https://www.transmissionzero.co.uk/software/freeglut-devel/) 3D OpenGL Graphics Library
* [Simple OpenGL Image Libary](http://www.lonesock.net/soil.html) OpenGL Image Library
* [ReactPhysics3D](http://www.reactphysics3d.com/) Physics Engine (Now just the vector and matrix algebra)

<!--
## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.
-->
## Versioning

We use CodeBlock's built-in support for auto versioning.

## Authors

* **Nawar Ismail** - *Initial work* - [Palfore](https://github.com/Palfore) - [PalforeProjects](https://palforeprojects.com)
* **Tomer Rockman** - *Getting Started*

<!--
## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
-->
## Acknowledgments

* Cary Huang (A.K.A) Carykh - Inspiration
* Matt Demers - Encouragement and facilitated an academic persual of this topic
