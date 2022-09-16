#include <random>
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <chrono>
using namespace std;

//#define elementos 1000

int main()
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 999);//numeros entre 1 y 100
    std::uniform_int_distribution<> punto(10, 600);


    // declarar el vector
    // vec [elementos][3]
    //std::cout<<sizeof(int**)<<std::endl;
    // int **vec = new int*[elementos];
    vector<vector<int>> vec(50000, vector<int>(3));
    for(int i = 0; i < 50000; ++i)
        vec[i].resize(3);


    int dimension= 3;
    double result = 0;

    int elementos[3]={1000, 10000, 50000};

    for(int elem = 0; elem < 3; ++elem) {
        result = 0;
        // llenar vector hasta la dimension deseada
        for (int i = 0; i < elementos[elem]; ++i)
            for (int j = 0; j < dimension; ++j)
                vec[i][j] = dis(gen);
        // begin time
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();
        // calcular distancia
        int i1 = punto(gen);
        for (int i2 = 0; i2 < elementos[elem]; ++i2) {
            if (i1 != i2)
                for (int j = 0; j < 3; ++j)
                    result += pow(vec[i1][j] - vec[i2][j], 2);
            result = std::round(sqrt(result));
        }
        // end time
        end = std::chrono::high_resolution_clock::now();

        int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "Tiempo con " << elementos[elem] <<" elementos de dimension 3 "<< ": " << duration << "ns" <<"\n";
    }
    std::cout << '\n';

    return 0;
}