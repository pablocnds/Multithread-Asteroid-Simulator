#include "headers/Nasteroids.h"
#include <random>
#include <fstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <omp.h>

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

    /* OPENMP CONCURRENCY MECANISMS */
    forceLock = new omp_lock_t[num_asteroids];

    for (int i=0; i<num_asteroids; i++)
        omp_init_lock(&(forceLock[i]));
}

void Nasteroids::simulate()
{
    for (int i = 0; i < num_iterations; i++)
    {
        iter();
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
    ofstream file;
    file.open(path);

    file << fixed << setprecision(3);
    
    for (int i = 0; i < num_asteroids; i++)
    {
        file << asteroids[i].x << " " << asteroids[i].y << " " << asteroids[i].vx << " " <<
            asteroids[i].vy << " " << asteroids[i].m << "\n";
    }

    file.close();
}

inline void Nasteroids::iter()
{
    #pragma omp parallel
    {
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < num_asteroids; i++)
    {
        int nx_bounce = -1;
        // [omp Optimization] To minimize lock usage, add forces to the i-th asteroid locally and them add up at the end
        double forcex = 0.0;
        double forcey = 0.0;

        for (int j = i+1; j < num_asteroids; j++)
        {
            // Distance module between asteroid i and asteroid j
            double dist_ij = sqrt((asteroids[i].x-asteroids[j].x)*(asteroids[i].x-asteroids[j].x) + 
                (asteroids[i].y-asteroids[j].y)*(asteroids[i].y-asteroids[j].y));
            
            if (dist_ij > consts::dmin)
            {
                // Angle between i and j
                double pend_ij = (asteroids[i].y-asteroids[j].y) / (asteroids[i].x-asteroids[j].x);
                if (pend_ij > 1.0) pend_ij = 1.0;
                else if (pend_ij < -1.0) pend_ij = -1.0;
                double angle_ij = atan(pend_ij);

                // Calculate force between i and j
                double aux = ((consts::G * asteroids[i].m * asteroids[j].m) / (dist_ij * dist_ij));
                if (aux > 100.0) aux = 100.0;
                double fx_ij = aux * cos(angle_ij);
                double fy_ij = aux * sin(angle_ij);

                // [OPTIMIZABLE] use private var and add everything once (same with planets)
                forcey += fy_ij;
                forcex += fx_ij;

                omp_set_lock(&(forceLock[j]));
                asteroids[j].forcey -= fy_ij;
                asteroids[j].forcex -= fx_ij;
                omp_unset_lock(&(forceLock[j]));
            }
            else
            {
                /* BOUNCE */
                if (nx_bounce == -1)
                    nx_bounce = j;
            }
        }
        
        for (int p = 0; p < num_planets; p++)
        {
            double dist_ip = sqrt((asteroids[i].x-planets[p].x)*(asteroids[i].x-planets[p].x) + 
                (asteroids[i].y-planets[p].y)*(asteroids[i].y-planets[p].y));
            
            double pend_ip = (asteroids[i].y-planets[p].y) / (asteroids[i].x-planets[p].x);
            if (pend_ip > 1.0) pend_ip = 1;
            else if (pend_ip < -1.0) pend_ip = -1;
            double angle_ip = atan(pend_ip);

            double aux = ((consts::G*asteroids[i].m*planets[p].m) / (dist_ip * dist_ip));
            if (aux > 100.0) aux = 100.0;

            forcex += aux * cos(angle_ip);
            forcey += aux * sin(angle_ip);
        }

        omp_set_lock(&(forceLock[i]));
        asteroids[i].forcey += forcey;
        asteroids[i].forcex += forcex;
        omp_unset_lock(&(forceLock[i]));

        /* ASTEROID BOUNCE */
        // In-order bounce, thus only calculating the next bounce in this iteration
        if (nx_bounce != -1)
        {
            double aux;
            aux = asteroids[i].vx;
            asteroids[i].vx = asteroids[nx_bounce].vx;
            asteroids[nx_bounce].vx = aux;
            aux = asteroids[i].vy;
            asteroids[i].vy = asteroids[nx_bounce].vy;
            asteroids[nx_bounce].vy = aux;
        }

    }

    #pragma omp for
    for (int i = 0; i < num_asteroids; i++) // Divided loop for OpenMP optimization (worse cache performace)
    {

        // Calculate new position
        double ax = (1/asteroids[i].m) * asteroids[i].forcex;
        double ay = (1/asteroids[i].m) * asteroids[i].forcey;
        asteroids[i].vx += ax * consts::deltaT;
        asteroids[i].vy += ay * consts::deltaT;
        asteroids[i].x += asteroids[i].vx * consts::deltaT;
        asteroids[i].y += asteroids[i].vy * consts::deltaT;

        /* MAP BOUNDARIES */
        if (asteroids[i].x <= 0.0) { 
            asteroids[i].x = consts::dmin;
            asteroids[i].vx = -asteroids[i].vx;
        }
        else if (asteroids[i].x >= consts::dWidth) {
            asteroids[i].x = consts::dWidth - consts::dmin;
            asteroids[i].vx = -asteroids[i].vx;
        }

        if (asteroids[i].y <= 0.0) {
            asteroids[i].y = consts::dmin;
            asteroids[i].vy = -asteroids[i].vy;
        }
        else if (asteroids[i].y >= consts::dHeight) {
            asteroids[i].y = consts::dHeight - consts::dmin;
            asteroids[i].vy = -asteroids[i].vy;
        }

        // COMMENTED BLOCK FOR TESTING DIFFERENT BOUNCE CONFIGURATIONS
        /*for (int i = 0; i < num_asteroids; i++)
        {
            for (int j = i+1; j < num_asteroids; j++)
            {
                double dist_ij = sqrt((asteroids[i].x-asteroids[j].x)*(asteroids[i].x-asteroids[j].x) + 
                    (asteroids[i].y-asteroids[j].y)*(asteroids[i].y-asteroids[j].y));
                
                if (dist_ij <= 5.0)
                {
                    double aux;
                    aux = asteroids[i].vx;
                    asteroids[i].vx = asteroids[j].vx;
                    asteroids[j].vx = aux;
                    aux = asteroids[i].vy;
                    asteroids[i].vy = asteroids[j].vy;
                    asteroids[j].vy = aux;

                    break;
                }
            }
        }*/

        asteroids[i].forcex = 0;
        asteroids[i].forcey = 0;
    }
    }
}

void Nasteroids::close ()
{
    delete[] asteroids;
    delete[] planets;

    for (int i=0; i<num_asteroids; i++)
        omp_destroy_lock(&(forceLock[i]));

    delete[] forceLock;
}