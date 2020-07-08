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
}