#include "headers/Nasteroids.h"
#include <random>
#include <fstream>
#include <iomanip>

using namespace std;

Nasteroids::Nasteroids(long n_asteroids, long n_iterations, long n_planets, long lseed)
{
    num_asteroids = n_asteroids;
    num_iterations = n_iterations;
    num_planets = n_planets;
    seed = lseed;

    default_random_engine re{seed};
    uniform_real_distribution<double> xdist{0.0, std::nextafter(consts::width,std :: numeric_limits<double>::max())};
    uniform_real_distribution<double> ydist{0.0, std::nextafter(consts::height,std :: numeric_limits<double>::max())};
    normal_distribution<double> mdist{consts::mass, consts::sdm};

    asteroids = new Asteroid[num_asteroids];
    planets = new Planet[num_planets];

    num_bodies = num_asteroids + num_planets;

    // Assign asteroid values
    for (int i = 0; i < num_asteroids; i++) 
    {
        asteroids[i].x = xdist(re);
        asteroids[i].y = ydist(re);
        asteroids[i].m = mdist(re);
        asteroids[i].vx = 0.0;
        asteroids[i].vy = 0.0;

        // Arrays to store the forces for every iteration
        asteroids[i].forcesx = new double[num_bodies-1];
        asteroids[i].forcesy = new double[num_bodies-1];
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
            planets[i].x = consts::width;
            planets[i].y = ydist(re);
            planets[i].m = mdist(re) * 10;
            break;
        case 3:
            planets[i].x = xdist(re);
            planets[i].y = consts::height;
            planets[i].m = mdist(re) * 10;
            break;
        }
    }
}

void Nasteroids::simulate()
{
    /* CALCULATE FORCES */
    // [optimization] Its not necessary to calculate the last iteration (their values have been already calculated at that point)
    long last_ast = num_asteroids-1;
    for (int i = 0; i < last_ast; i++)
    {
        // [optimization] The previous asteroids to i have been calculated with j already, and i=j doestn make sense
        for (int j = i+1; j < num_asteroids; j++)
        {
            // Distance module between asteroid i and asteroid j:
            double dist_ij = sqrt((asteroids[i].x-asteroids[j].x)*(asteroids[i].x-asteroids[j].x) + 
                (asteroids[i].y-asteroids[j].y)*(asteroids[i].y-asteroids[j].y));
            
            if (dist_ij > consts::dmin) //Case: asteroids dont collide
            {
                // Angle between i and j
                double pend_ij = (asteroids[i].y-asteroids[j].y) / (asteroids[i].x-asteroids[j].x);
                if (pend_ij > 1.0) pend_ij = 1;
                else if (pend_ij < -1.0) pend_ij = -1;
                double angle_ij = atan(pend_ij);

                // Calculate force between i and j
                double aux = ((consts::G*asteroids[i].m*asteroids[i].m) / dist_ij);
                double fx_ij = aux * cos(angle_ij);
                asteroids[i].forcesx[j-1] = fx_ij;
                asteroids[j].forcesx[i] = fx_ij;
                double fy_ij = aux * sin(angle_ij);
                asteroids[i].forcesy[j-1] = fy_ij;
                asteroids[j].forcesy[i] = fy_ij;
            }
            else    // Case: asteroids collide
            {

            }
        }
        for (int k = 0; k < num_planets; k++)
        {
            
        }
    }

    /* CALCULATE NEW POSITIONS */
    for (int i = 0; i < num_asteroids; i++)
    {

    }
}

void Nasteroids::printConfig(const char* path)
{
    ofstream file;
    file.open(path);

    file << fixed << setprecision(3);
    file << num_asteroids << " " << num_iterations << " " << num_planets << " " << seed << "\n";

    for (int i = 0; i < num_asteroids; i++)
        file << asteroids[i].x << " " << asteroids[i].y << " " << asteroids[i].m << "\n";
    
    for (int i = 0; i < num_planets; i++)
        file << planets[i].x << " " << planets[i].y << " " << planets[i].m << "\n";

    file.close();
}

void Nasteroids::printState(const char* path)
{

}

void Nasteroids::iter()
{

}