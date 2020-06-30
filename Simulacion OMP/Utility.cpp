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


double Utility::continuous_uniform_random(double a, double b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(a, b);

	return  dis(gen);
}


int Utility::discrete_uniform_random(int a, int b)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(a, b);

	return dist6(rng);
}

int Utility::index_transform(int x, int y, int n)
{
	return (x * n) + y;
}
