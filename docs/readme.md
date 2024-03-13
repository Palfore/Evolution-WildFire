# Evolution WildFire

Evolution WildFire is a 3D robotics simulator that uses reinforcement learning to train mechanical creatures controlled by neural-networks. It is written in C++/OpenGL and uses multi-threading to evolve the creatures in the background as you explore the real-time environment with your pets! The strong survive, and the weak... well let's not worry about them. The creatures are evolved one by one as they modify their morphologies and their brains to learn how to control their bodies well enough to make it to the next generation.

![3D Terrain with creature in center shot](https://github.com/Palfore/Evolution-Project/blob/master/docs/3D%20Terrain%20w%20Creature.jpg "Evolution WildFire")

More specifically, a creature is made up of muscles (pistons), bones (rods), and contact nodes that all contribute to the creature's structure and function. However for complex motion they need a sophisticated control mechanism that can evolve along with their bodies. Introduce the Neural Network. Their ability to interact with the world is shaped according to the Genetic Algorithm driving improvements in fitness, as creatures compete to  accomplish their goal.

This project has gone through several stages of development. With it's 2D incarnation written in C, this 3D simulator was developed to explore more complex behavior. First, the creatures learned how to move in 2D, then they they were trained to jump up in the air, or to walk up or down hills. Their muscles started as sinusoidal pistons with a speed and off-set parameter. But for more complex behavior neural networks were required. The locomotion in 3D proves very difficult, and a new morphology was implemented to avoid this. The latest form of creature, the Eye Walker, has two neural networks that control it's motion: one for its eye, and one for its brain. With the advent of eyesight, creatures could chase food but their complex piston-powered bodies were replaced with a simple car-like motion. Now, creature compete to collect food.


### Running

Run `bin/Debug/Evolution-Project.exe` from the top-level directory. On windows, you can simply run `launch.bat`.

### Installing

To run:

- Windows is already compiled. Mac you will have to compile it yourself.
- It has to be run from the directory with bin, assets, src, etc in it (rather than just click on the exe).
- Open up the command prompt and cd into the "Evolution Project" directory and type bin\Debug\"Evolution Project.exe"

To develop/compile:

- From https://solarianprogrammer.com/2017/11/22/install-codeblocks-gcc-windows/
	- Go to http://www.codeblocks.org/downloads/26 and download codeblocks-17.12-setup.exe
	- Go to https://nuwen.net/mingw.html and download mingw-16.1.exe to C:\ (not downloads)
- Then you can just load up the Evolution Project.cbp and hit run (or rebuild then run).

### Working with the Code

- evolveMode.cpp is the main driving file.
- The Population constructor defines the default creature generation.
- Creature::update and MultiThread::processCreatures define how the creatures are evolved (The ones the user sees and the ones in the background).
- The FitnessCollector fitbit map defines the possible evolution types (Work could be done here to figure out the best way reward system to evolve goal seeking)
- NodeGene has a setting for twoD creatures.
- NeuralNetwork.cpp (somewhat obviously) defines the (basic) neural network. A library should probably be used for more sophisticated networks.


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
