#include <iostream>
#include <sstream>

int main(int argc, char**argv)
{
    if (argc != 5)
    {
        std::cerr << "ERROR: Invalid arguments. You should include 4 parameters:\n\t" << argv[0] << " [num_asteroids] [num_iterations] [num_planets] [seed]" << std::endl;
        return -1;
    }

    // "Parser"
    int values[4];
    for (int i = 1; i < 5; i++)
    {
        std::stringstream ss(argv[1]);
        if( ss >> values[i-1] ) {}
            // Pass num to constructor
        else 
        {
            std::cout << "ERROR: Invalid arguments." << std::endl;
            return -1;
        }
    }
    

    return 0;
}