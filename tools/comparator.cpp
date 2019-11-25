#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

int main(int argc, char**argv)
{
    if (argc < 3)
        return -1;

    ifstream in1(argv[1]);
    if (!in1.is_open())
    {
        cout << "Error at opening " << argv[1] << "\n";
        return -1;
    }
    
    ifstream in2(argv[2]);
    if (!in2.is_open())
    {
        cout << "Error at opening " << argv[2] << "\n";
        return -1;
    }

    cout << fixed;
    cout << setprecision(3);

    int num_lines = 0;
    double sum_dif_pos = 0.0, sum_dif_vel = 0.0;
    double max_dif_pos = 0.0, max_dif_vel = 0.0;
    string line1, line2;
    while (getline(in1, line1))
    {
        if (!getline(in2, line2)) {
            cout << "WARNING: the 2 files have diferent lenght\n";
            break;
        }

        std::stringstream ss1(line1), ss2(line2);
        std::string token1, token2;
        for (int i = 0; i < 2; i++) {
            getline(ss1, token1, ' '); 
            getline(ss2, token2, ' ');
            double d = abs(stod(token1) - stod(token2));
            sum_dif_pos += d;
            //cout << d << " ";
            if (d > max_dif_pos) max_dif_pos = d;
        }
        for (int i = 0; i < 2; i++) {
            getline(ss1, token1, ' '); 
            getline(ss2, token2, ' ');
            double d = abs(stod(token1) - stod(token2));
            sum_dif_vel += d;
            //cout << d << " ";
            if (d > max_dif_vel) max_dif_vel = d;
        }
        //cout << "\n";

        num_lines++;
    }

    cout << "\n> AVG DIFERENCE IN: position = " << (double)(sum_dif_pos/((double)num_lines*2)) 
            << ", velocity = " << (double)(sum_dif_vel/((double)num_lines*2)) << "\n";
    cout << "> HIGHEST DIFERENCE IN: position = " << max_dif_pos << ", velocity = " << max_dif_vel << "\n";

    if (getline(in2, line2)) cout << "WARNING: the 2 files have diferent lenght\n";

    return 0;
}