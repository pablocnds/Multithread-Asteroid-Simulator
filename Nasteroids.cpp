#include "headers/Nasteroids.h"
#include <random>
#include <fstream>

using namespace std;

Nasteroids::Nasteroids(long n_asteroids, long n_iterations, long n_planets, long lseed)
{
    num_asteroids = n_asteroids;
    num_iterations = n_iterations;
    num_planets = n_planets;
    seed = lseed;

    default_random_engine re{seed};
    uniform_real_distribution<double> xdist{0.0, std::nextafter(width,std :: numeric_limits<double>::max())};
    uniform_real_distribution<double> ydist{0.0, std::nextafter(height,std :: numeric_limits<double>::max())};
    normal_distribution<double> mdist{mass, sdm};

    asteroids = new Body[num_asteroids];
    planets = new Body[num_planets];

    // Assign asteroid values
    for (int i = 0; i < num_asteroids; i++) 
    {
        asteroids[i].x = xdist(re);
        asteroids[i].y = ydist(re);
        asteroids[i].m = mdist(re);
    }

    // Assign planet values
    for (int i = 0; i < num_planets; i++)
    {
        switch (i%4)
        {
        case 0:
            planets[i].x = 0;
            planets[i].y = ydist(re);
            planets[i].m = mdist(re) * 10;
            break;
        case 1:
            planets[i].x = xdist(re);
            planets[i].y = 0;
            planets[i].m = mdist(re) * 10;
            break;
        case 2:
            planets[i].x = width;
            planets[i].y = ydist(re);
            planets[i].m = mdist(re) * 10;
            break;
        case 3:
            planets[i].x = xdist(re);
            planets[i].y = height;
            planets[i].m = mdist(re) * 10;
            break;
        }
    }
}

void Nasteroids::simulate()
{

}

void Nasteroids::printConfig(const char* path)
{
    ofstream file;
    file.open(path);

    file << num_asteroids << " " << num_iterations << " " << num_planets << " " << seed << endl;

    for (int i = 0; i < num_asteroids; i++)
        file << asteroids[i].x << " " << asteroids[i].y << " " << asteroids[i].m << endl;
    
    for (int i = 0; i < num_planets; i++)
        file << planets[i].x << " " << planets[i].y << " " << planets[i].m << endl;

    file.close();
}

void Nasteroids::printState(const char* path)
{

}

void Nasteroids::iter()
{

}