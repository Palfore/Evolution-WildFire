# Evolution WildFire

Evolution WildFire is an real-time interactive 3D evolution simulator. Mechanical, animal-like creatures evolve to be the best. The strong survive, and the weak... well let's not worry about them. The creatures are evolved one by one as they modify their morphologies and their brains to learn how to control their bodies well enough to make it to the next generation.

![3D Terrain with creature in center shot](https://github.com/Palfore/Evolution-Project/blob/master/docs/3D%20Terrain%20w%20Creature.jpg "Evolution WildFire")

More specifically, a creature is made up of muscles, bones, and contact nodes that all contribute to the creature's structure and function. However for complex motion they need a sophisticated control mechanism that can evolve along with their bodies. Introduce the Neural Network. This handy structure evolves, using the Genetic Algorithm, along with the body and acts as its brain, actuating their muscles to acomplish their goal.

Now evolving random structures to walk around and chase after things sounds awesome (and it is) but it's also quite a difficult task. Neural Networks are a massive field and figuring out the right way to implement one, and the right way to train the creatures is actually a pretty difficult task. Thankfully, this walking problem has been solved in two dimensions fairly easily, but in three dimensions not so much. Walking in 3D isn't that bad but the idea of goal seeking, being able to turn around and move to different targets, well that is still in the works.

The real-time aspect of this simulator makes use of multi-threading. You will play around in one instance which lets you explore all the different creatures, while all the evolution takes place in the background.

### Installing

A semi-major priority in this project is to keep dependencies managed. As in, you should be able to simply copy the project onto a local drive and run it right away.

To run:
	Windows is already compiled. Mac you will have to compile it yourself.
	It has to be run from the directory with bin, assets, src, etc in it (rather than just click on the exe).
	Open up the command prompt and cd into the "Evolution Project" directory and type bin\Debug\"Evolution Project.exe"

To develop/compile:
	From https://solarianprogrammer.com/2017/11/22/install-codeblocks-gcc-windows/
	1) Go to http://www.codeblocks.org/downloads/26 and download codeblocks-17.12-setup.exe
	2) Go to https://nuwen.net/mingw.html and download mingw-16.1.exe to C:\ (not downloads)
	Then you can just load up the Evolution Project.cbp and hit run (or rebuild then run).

### Working with the Code

evolveMode.cpp is the main driving file.
The Population constructor defines the default creature generation.
Creature::update and MultiThread::processCreatures define how the creatures are evolved (The ones the user sees and the ones in the background).
The FitnessCollector fitbit map defines the possible evolution types (Work could be done here to figure out the best way reward system to evolve goal seeking)
NodeGene has a setting for twoD creatures.
NeuralNetwork.cpp (somewhat obviously) defines the (basic) neural network. A library should probably be used for more sophisticated networks.


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

* **Nawar Ismail** - [Palfore Github](https://github.com/Palfore) - [PalforeProjects Website](https://palforeprojects.com)

<!--
## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
-->
## Acknowledgments

* Cary Huang (A.K.A) [Carykh](https://www.youtube.com/user/carykh) - Inspiration