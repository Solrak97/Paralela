#include <vector>
#include <list>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <random>

namespace Utility {

	//This will calculate the number of threads
	//from user input and cores in PC
	int get_max_threads();

	//Returns a random double in range [a,b]
	double continuous_uniform_random(double a, double b);

	//Returns a random int in range [a,b]
	int discrete_uniform_random(int a, int b);

	//transforms a 2D index to a 1D index
	int index_transform(int x, int y, int n);

}