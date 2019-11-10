#include "Asteroid.h"
#include "Planet.h"
#include "Constants.h"

class Nasteroids
{
public:
    Nasteroids(long n_asteroids, long n_iterations, long n_planets, long seed);
    
    void printConfig(const char* pathToFile);   // Meant to print the initial configuration

    void printState(const char* pathToFile);    // Meant to print the final state
    
    void simulate();
private:
    long num_asteroids, num_iterations, num_planets, seed, num_bodies;
    Planet* planets;
    Asteroid* asteroids;

    void iter();
};