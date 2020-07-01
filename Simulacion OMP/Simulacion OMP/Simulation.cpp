#include "Simulation.h"

std::vector<Individual> Simulation::create_population(int cpr,  double prj, double prm, int poi,int dmn, int dmx, int rmj, int rmm, int vmj, int vmm, int tmm, int MAX_THREADS)
{
	//Vector of population, will be populated by Individuals and indexed by the pop_matrix
	std::vector<Individual> population;
	population.resize(cpr);

	//As population indexes are unique, is possible to create population on parallel to reduce creation time
	//This time, Individual models are more comples since no network communication is required
	double threshold = cpr * 0.945;
	int patient_zeros = 0;

#pragma omp parallel for num_threads(MAX_THREADS)
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
		int x = rand() % tmm;
		int y = rand() % tmm;

		population[i] = Individual(age, prj, prm, rmj, rmm, vmj, vmm, Coordinate(x, y));

		//infects the individuals
		if (infection) {
			population[i].infect(dmn, dmx);
		}
	}
	return population;
}

void Simulation::init_locker(std::vector<omp_lock_t> locker, int MAX_THREADS)
{
	#pragma omp parallel for num_threads(MAX_THREADS)
	for (int i = 0; i < locker.size(); i++)
	{
		omp_init_lock(&locker[i]);
	}
}

void Simulation::destroy_locker(std::vector<omp_lock_t> locker, int MAX_THREADS)
{	
	#pragma omp parallel for num_threads(MAX_THREADS)
	for (int i = 0; i < locker.size(); i++)
	{
		omp_destroy_lock(&locker[i]);
	}
}
