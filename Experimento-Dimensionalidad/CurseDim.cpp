#include <random>
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <chrono>
#include <map>

int main()
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(1, 100);//numeros entre 1 y 100
	std::uniform_int_distribution<> punto(0, 19999);


	std::map<double, int> histogram[19];/* = new std::map<double, int>;*/


	int** vec = new int* [20000];
	for (int i = 0; i < 20000; ++i)
		vec[i] = new int[100];

	int dimensiones[19] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

	double result = 0;

	int suma;

	for (int dim = 0; dim < 19; ++dim) {

		result = 0;
		// llenar vector hasta la dimension deseada podria sacarlo
		for (int i = 0; i < 20000; ++i)//20k elementos
			for (int j = 0; j < dimensiones[dim]; ++j)
				vec[i][j] = dis(gen);

		// calcular distancia
		std::cout << "dimension: " << dimensiones[dim] << '\n';
		int i1 = punto(gen);
		for (int i2 = 0; i2 < 20000; ++i2) {
			if (i1 != i2)
				for (int j = 0; j < dimensiones[dim]; ++j)
					result += pow(vec[i1][j] - vec[i2][j], 2);
			result = std::round(sqrt(result));
			if (i1 != i2)
				++histogram[dim][result];
		}

		for (const auto& x : histogram[dim]) {
			std::cout << x.first << "     " << x.second << "\n";
		}
	}

	std::cout << '\n';

	return 0;
}