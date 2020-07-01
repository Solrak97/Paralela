#include "Utility.h"


int Utility::get_max_threads()
{
	int CORES = omp_get_num_procs();
	int MAX_THREADS;
	printf("Nucleos en el computador: %d\n", CORES);
	std::cout << "Hilos por nucleo?" << std::endl;
	std::cin >> MAX_THREADS;

	return MAX_THREADS * CORES;
}


int Utility::index_transform(int x, int y, int n)
{
	return (x * n) + y;
}
