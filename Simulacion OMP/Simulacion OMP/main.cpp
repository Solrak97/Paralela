#include "Utility.h"
#include "Individual.h"
#include "Simulation.h"

#include <math.h>
#include <ctime>


#define PARAMS_C 13

int main(int argc, char * argv[]) 
{
	//Param "error handler"
	if (argc - 1 != PARAMS_C) {
		std::cout << "La cantidad de parametros no es correcta\n" << 
		"asegurese de utilizar el siguiente formato\n" <<
		"cpr piv prj prm poi toc dmn dmx rmj rmm vmj vmm drc" << std::endl;
		exit(0);
	}

	//			PARAMS
	int cpr = atoi(argv[1]);
	double piv = atof(argv[2]);
	double prj = atof(argv[3]);
	double prm = atof(argv[4]);
	int poi = atoi(argv[5]);
	double toc = atof(argv[6]);
	int dmn = atoi(argv[7]);
	int dmx = atoi(argv[8]);
	int rmj = atoi(argv[9]);
	int rmm = atoi(argv[10]);
	int vmj = atoi(argv[11]);
	int vmm = atoi(argv[12]);
	int drc = atoi(argv[13]);

	//MAX THREAD FIXED FROM CORES AND USER INPUT
	int MAX_THREADS = Utility::get_max_threads();
	
	//Walltime
	double init_time = omp_get_wtime();

	//TMM Calc
	int tmm = floor(sqrt(cpr / toc)) + 1;
	
	//Population Matrix definition
	std::vector<std::vector<std::list<int>>> pop_matrix;
	pop_matrix.resize(tmm, std::vector<std::list<int>>(tmm));
	
 
	//Vector of population, will be populated by Individuals and indexed by the pop_matrix
	std::vector<Individual> population = Simulation::create_population(cpr, prj, prm, poi, dmn, dmx, rmj, rmm, vmj, vmm, tmm, MAX_THREADS);
	
	double final_time = omp_get_wtime();
	std::cout << "Final time: " << final_time - init_time << std::endl;

	//Locker creation
	std::vector<omp_lock_t> locker = std::vector<omp_lock_t>(tmm * tmm);
	
	//Init locker
	void init_locker(std::vector<omp_lock_t> locker, int MAX_THREADS);

	//TIC SIMULATION
	for (int day = 0; day < drc; day++) {

	}


	//Destroy locker
	void destroy_locker(std::vector<omp_lock_t> locker, int MAX_THREADS);

	//double final_time = omp_get_wtime();
}