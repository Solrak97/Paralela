#include <vector>
#include <omp.h>
#include "Individual.h"

#pragma once

namespace Simulation {
	//Creates a pop vector from the param list... the BIG param list
	std::vector<Individual> create_population(int cpr, double prj, double prm, int poi, int dmn, int dmx, int rmj, int rmm, int vmj, int vmm, int tmm, int MAX_THREADS);

	//Sets and unsets the locker
	void init_locker(std::vector<omp_lock_t> locker, int MAX_THREADS);
	void destroy_locker(std::vector<omp_lock_t> locker, int MAX_THREADS);
}