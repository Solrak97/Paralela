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


	//TIC Simulation
	for (int day = 0; day < drc; day++) {
		std::vector<std::vector<std::list<int>>> tic_pop_matrix = std::vector<std::vector<std::list<int>>>(tmm, std::vector<std::list<int>>(tmm));
		std::vector<Individual> tic_population = population;

		//Iterates on population vector
		#pragma omp parallel for num_threads(MAX_THREADS)
		for (int i = 0; i < cpr; i++) {
			int infected_count = 0;
			
			std::list<int> list = pop_matrix[population[i].location.x][population[i].location.y];
			for (auto it = list.begin(); it != list.end(); it++) {
				if (population[*it].status == infected) infected_count++;
			}

			int status = tic_population[i].calculate_illness(infected_count, piv, DAYS_OF_IMMUNITY, dmn, dmx); //To-do calcular el valor de retorno
			tic_population[i].move(tmm);

			#pragma omp critical
			tic_pop_matrix[tic_population[i].location.x][tic_population[i].location.y].push_back(i);
			
			#pragma omp critical			
			{
				if (status != -1) {
					switch (status)
					{
					case 0:
						total_deceased++;
						break;

					case 1:
						total_immune++;
						break;

					case 2:
						total_infected++;
						break;
					default:
						break;
					}
				}
			}
		}
		pop_matrix = tic_pop_matrix;
		population = tic_population;

		DATA_BUFFER += std::to_string(total_deceased) + "," + std::to_string(total_infected) + "," + std::to_string(total_immune) + "\n";
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