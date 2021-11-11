# Multi-Threaded Asteroid simulation

Work of paralelism with OpenMP in C++ with Bash Script.

The project consist of a simulation with a variable ammount of asteroids and planets including gravitational interactions and collisions. The objective is to implement this simulation and boost its performance with multi-thread parallelization.

Bash Script is used to automate testing and check that no precision is lost in the multi-threaded version.

## How to run the program

Choose either the concurrent version in the `par` folder or de sequential one in `seq` and compile the code inside. With g++: `g++ -fopenmp -std=c++14 -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors ./par/main.cpp ./par/Nasteroids.cpp`.

Also, the bash script tools/test_creator.sh will compile the program and run some timed tests and also give some precision metrics.
