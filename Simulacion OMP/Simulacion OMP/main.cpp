#include "Utility.h"
#include "Individual.h"
#include <string>



#define PARAMS_C 13
#define DAYS_OF_IMMUNITY 40



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



	//Simulation Variables
	int tmm = floor(sqrt(cpr / toc)) + 1;

	int total_infected = 0;
	int total_immune = 0;
	int total_deceased = 0;

	std::string DATA_BUFFER = "";


	//Data definition and initialization
	std::vector<std::vector<std::list<int>>> pop_matrix;
	pop_matrix.resize(tmm, std::vector<std::list<int>>(tmm));
	
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
		
		//At matrix indexation is the only mutual exclution access it'll need
		#pragma omp critical
		{
			pop_matrix[x][y].push_back(i);
		}

		population[i] = Individual(age, prj, prm, rmj, rmm, vmj, vmm, Coordinate(x, y));

		//infects the individuals
		if (infection) {
			population[i].infect(dmn, dmx);
		}
	}



	std::vector<std::vector<int>> tic_infected_matrix = std::vector<std::vector<int>>(tmm, std::vector<int>(tmm));
	std::vector<omp_lock_t> row_locker = std::vector<omp_lock_t>(tmm);

	#pragma omp parallel for num_threads(MAX_THREADS)
	for (int i = 0; i < tmm; i++) {
		omp_init_lock(&row_locker[i]);
	}

	
	//TIC Simulation
	#pragma parallel num_threads(MAX_THREADS)
	for (int day = 0; day < drc; day++) {

		//Infected cleanup     North Korea much?
		#pragma omp parallel for collapse(2) num_threads(MAX_THREADS)
		for (int i = 0; i < tmm; i++) {
			for (int j = 0; j < tmm; j++) {
				tic_infected_matrix[i][j] = 0;
			}
		}


		//Infected count
		//Probability of memory rewritte is very low, yet is better protected so im using a row locker
		#pragma omp parallel for num_threads(MAX_THREADS)
		for (int i = 0; i < cpr; i++) {
			if (population[i].status == infected) {
				omp_set_lock(&row_locker[population[i].x()]);
				tic_infected_matrix[population[i].x()][population[i].y()]++;
				omp_unset_lock(&row_locker[population[i].x()]);
			}
		}


		//Makes calcualtions over population vector
		#pragma omp parallel for num_threads(MAX_THREADS)
		for (int i = 0; i < cpr; i++) {
			//Status calculation
			int status = population[i].calculate_illness(tic_infected_matrix[population[i].x()][population[i].y()], piv, DAYS_OF_IMMUNITY, dmn, dmx);


			//Movement on matrix
			omp_set_lock(&row_locker[population[i].x()]);
			pop_matrix[population[i].x()][population[i].y()].remove(i);
			omp_unset_lock(&row_locker[population[i].x()]);
			
			population[i].move(tmm);
			
			omp_set_lock(&row_locker[population[i].x()]);
			pop_matrix[population[i].x()][population[i].y()].push_back(i);
			omp_unset_lock(&row_locker[population[i].x()]);


			//Process stats
			if (status != -1) {
				switch (status)
				{
				case 0:
					#pragma omp critical (deceased)
					total_deceased++;
					break;

				case 1:
					#pragma omp critical (immune)
					total_immune++;
					break;

				case 2:
					#pragma omp critical (infected)
					total_infected++;
					break;

				default:
					break;
				}
			}
		}

		//Data writting into buffer, as it is outside of the paralell region, it doesnt need guards		
		DATA_BUFFER += std::to_string(total_deceased) + "," + std::to_string(total_infected) + "," + std::to_string(total_immune) + "\n";
	}
	

	#pragma omp parallel for num_threads(MAX_THREADS)
	for (int i = 0; i < tmm; i++) {
		omp_destroy_lock(&row_locker[i]);
	}

	double final_time = omp_get_wtime();

	//Final CSV data descriptor
	std::ofstream csv_data;
	csv_data.open("data.csv");
	csv_data << DATA_BUFFER;
	csv_data.close();
	std::cout << DATA_BUFFER;


	std::cout << "Final time: " << final_time - init_time << std::endl;

}