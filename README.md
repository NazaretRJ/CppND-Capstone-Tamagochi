# CPPND: Tamagochi (final project of Udacity C++ Nanodegree Program)

Tamagochi is a pet simulation game. You can feed and sleep your pet, moreover you can play with it!

![structure of the project](/images/threads.png)
In the picture we can see the structure of the program.
On one hand, we have the main thread, which reads data from the command that the user enters on the screen. This command is added to a message queue that will then be read by one of the threads of the Tamagochi class.

Tamagochi class has two threads. One (readThread) to read the user commands and update the status, and another (simulationThread) that updates the status of the tamagochi after a certain time has elapsed.

After some time has passed, the tamagochi may become hungry or sleepy, which is what simulationThreads does. It updated the levels of hunger or sleep and also notify the user.

readThread will update the levels of hunger or sleep when the user decides it.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Tamagochi`.

## Rubric.

The project meets the following criteria:

### Loops, Functions, I/O
- The project demonstrates an understanding of C++ functions and control structures.
- The project accepts user input and processes the input.

### Object Oriented Programming
- The project uses Object Oriented Programming techniques.
- Classes use appropriate access specifiers for class members.
- Class constructors utilize member initialization lists.
- Classes encapsulate behavior. (I just have a class but It encapsulate the behaviour of the tamagochi.)
- Templates generalize functions in the project.

### Memory Management
- The project uses smart pointers instead of raw pointers.

### Concurrency
- The project uses multithreading.
- A mutex or lock is used in the project.
- A condition variable is used in the project.

## Output
When you play with your tamagochi, you will see something like:
\
![playing](/images/playing.png)
\
You can choose any of the actions by typing the corresponding number.
The tamagochi will say a message responding to that action.
The tamagiochi will periodically report its status.
if you hit 4, you will stop playing.