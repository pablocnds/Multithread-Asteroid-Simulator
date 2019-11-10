#include <iostream>
#include <sstream>
#include "./headers/Nasteroids.h"

using namespace std;

void parseError();

int main(int argc, char**argv)
{
    if (argc != 5)
    {
        parseError();
        return -1;
    }

    // "Parser"
    long values[4];
    for (int i = 1; i < 5; i++)
    {
        std::stringstream ss(argv[i]);
        if( ss >> values[i-1] && values[i-1] >= 0 ) {}
        else 
        {
            parseError();
            return -1;
        }
    }

    Nasteroids* simulation = new Nasteroids(values[0], values[1], values[2], values[3]);
    simulation->printConfig("init_conf.txt");
    //simulation->simulate();
    //simulation->printState("out.txt");

    return 0;
}

void parseError()
{
    std::cerr << "nasteroids-seq" << ": Wrong arguments.\nCorrect use:\n" << "nasteroids-seq" << " num_asteroides num_iteraciones num_planetas semilla" << std::endl;
}