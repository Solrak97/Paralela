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

	//transforms a 2D index to a 1D index
	int index_transform(int x, int y, int n);
}