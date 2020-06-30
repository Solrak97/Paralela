#include "Utility.h"
#include <math.h>
#include "Individual.h"

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
	std::vector<Individual> population;
	population.resize(cpr);

	//As population indexes are unique, is possible to create population on parallel to reduce creation time
	//This time, Individual models are more comples since no network communication is required
	double threshold = cpr * 0.945;
	int patient_zeros = 0;
	int individuals = 0;

	#pragma parallel for num_threads(MAX_THREADS)
	{
		for (int i = 0; i < cpr; i++) {
			bool infection = false;
			bool age = false;

			//divides the individuals in 2 age groups
			if (i > threshold) {
				age = true;
			}
			
			//Infects the needed ammount of patients distribuited on both groups
			if (patient_zeros < poi) {
				if (age && patient_zeros > poi * 0.945) {
					infection = true;
					patient_zeros++;
				}
				else if (patient_zeros <= poi * 0.945) {
					infection = true;
					patient_zeros++;
				}
			}

			//creates the individual in a random position
			int x = Utility::discrete_uniform_random(0, tmm - 1);
			int y = Utility::discrete_uniform_random(0, tmm - 1);
			population[i] = Individual(age, prj, prm, rmj, rmm, vmj, vmm, Coordinate(x, y));
			
			//infects the individuals
			if (infection) {
				population[i].infect(dmn, dmx);
			}
			individuals++;
		}
	}

	//Init locker
	std::vector<omp_lock_t> locker = std::vector<omp_lock_t>(tmm * tmm);
	for (int i = 0; i < locker.size(); i++) {
		omp_init_lock(&locker[i]);
	}
	
	#pragma omp parallel num_threads(MAX_THREADS)
	{
		//Find a way to code this lock section!
		// and not using an array of mutexes :(
	}

	//Destroy locker
	for (int i = 0; i < locker.size(); i++) {
		omp_destroy_lock(&locker[i]);
	}

	double final_time = omp_get_wtime();
}