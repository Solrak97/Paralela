#include <omp.h>
#include "Individual.h"
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <sstream>





int main(int argc, char * argv[]) 
{
	//Lectura y guardado de los parametros a partir del archivo csv, este archivo viene como argv[1]
	std::vector<double> vec_params;
	std::fstream params;
	double param;
	std::string line;
	params.open(argv[1]);
	std::getline(params, line);
	std::istringstream ss(line);

	if(params.is_open()){
		for (std::string each; std::getline(ss, each, ','); vec_params.push_back(stof(each)));
	}
	else {
		std::cout << "Problemas para leer el archivo, terminando el programa" << std::endl;
		exit(0);
	}
	


	//			PARAMS
	int cpr = (int)vec_params[0];
	double piv = vec_params[1];
	double prj = vec_params[2];
	double prm = vec_params[3];
	int poi = (int)vec_params[4];
	double toc = vec_params[5];
	int dmn = (int)vec_params[6];
	int dmx = (int)vec_params[7];
	int rmj = (int)vec_params[8];
	int rmm = (int)vec_params[9];
	int vmj = (int)vec_params[10];
	int vmm = (int)vec_params[11];
	int drc = (int)vec_params[12];



	//La cantidad maxima de threads viene de nucleos logicos, multiplicado con  la cantidad de hilos por nucleo
	int MAX_THREADS = omp_get_num_procs();
	int tbc = 0;
	int DAYS_OF_IMMUNITY = 0;
	std::cout << "Este pc cuenta con " << MAX_THREADS << " nucleos logicos" << std::endl;
	std::cout << "Cuantos threads quiere usar por nucleo?" << std::endl;
	std::cin >> tbc;
	MAX_THREADS *= tbc;
	std::cout << "Cuantos dias de inmunidad tendran las personas: " << std::endl;
	std::cin >> DAYS_OF_IMMUNITY;

	//variables necesarias para datos de simulacion
	int tmm = floor(sqrt(cpr / toc)) + 1;
	std::string DATA_BUFFER = "";
	std::string TOTAL_DATA_BUFFER = "";



	//Definicion e inicializacion de los datos
	std::vector<std::vector<std::list<int>>> pop_matrix;
	pop_matrix.resize(tmm, std::vector<std::list<int>>(tmm));
	
	std::vector<Individual> population;
	population.resize(cpr);
 
	std::vector<std::vector<int>> tic_infected_matrix = std::vector<std::vector<int>>(tmm, std::vector<int>(tmm));
	std::vector<omp_lock_t> row_locker = std::vector<omp_lock_t>(tmm);
	


	//Ya que los indices en el vector de poblacion son unicos, es posible aprovechar el paralelismo para esta tarea
	//por lo que se utiliza para rapidamente rellenar la poblacion
	double threshold = cpr * 0.945;
	int patient_zeros = 0;

	#pragma omp parallel num_threads(MAX_THREADS)
	{
		#pragma omp for 
		for (int i = 0; i < cpr; ++i) {
			bool infection = false;
			bool age = false;

			//Se dividen los individuos en 2 grupos de edad
			if (i > threshold) {
				age = true;
			}

			// Se infecta la poblacion equitativamente, para tener el mismo porcentaje de infectados jovenes y viejos
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

			//crea una posicion random para el home
			int x = rand() % tmm;
			int y = rand() % tmm;
		
			//La indexacion en matriz no es posible de hacer de forma completamente paralela, por lo que se usa un candado
			#pragma omp critical
			{
				pop_matrix[x][y].push_back(i);
			}

			population[i] = Individual(age, prj, prm, rmj, rmm, vmj, vmm, Coordinate(x, y));

			//Se infectan los individuos seleccionados para la infeccion
			if (infection) {
				population[i].infect(dmn, dmx);
			}
		}


		#pragma omp for
		for (int i = 0; i < tmm; ++i) {
			omp_init_lock(&row_locker[i]);
		}
	}


	//Walltime
	double init_time = omp_get_wtime();


	int total_infected = 0;
	int total_immune = 0;
	int total_deceased = 0;


	//TIC Simulation
	for (int day = 0; day < drc; ++day) {
		
		int tic_infected = 0;
		int tic_immune = 0;
		int tic_deceased = 0;

		#pragma omp parallel num_threads(MAX_THREADS) 
		{
			//Se limpia la matriz de infeccion, para mantener la cantidad de infectados normalizada
			#pragma omp for collapse(2)
			for (int i = 0; i < tmm; i++) {
				for (int j = 0; j < tmm; j++) {
					tic_infected_matrix[i][j] = 0;
				}
			}


			//Conteo de infectados
			//Aunque el acceso multiple de hilos a una sola ubicacion de memoria es poco probable, es mejor
			//asegurar el acceso exclusivo, se paso de utilizar locks a la clausula atomic
			#pragma omp for
			for (int i = 0; i < cpr; ++i) {
				if (population[i].status == infected) {
					//omp_set_lock(&row_locker[population[i].x()]);
					#pragma omp atomic
					++tic_infected_matrix[population[i].x()][population[i].y()];
					//omp_unset_lock(&row_locker[population[i].x()]);
				}
			}


			//Se calcula el cambio de tic para la poblacion en el vector de poblacion
			#pragma omp for
			for (int i = 0; i < cpr; ++i) {
		
				//Calculo de las estadisticas
				int status = population[i].calculate_illness(tic_infected_matrix[population[i].x()][population[i].y()], piv, DAYS_OF_IMMUNITY, dmn, dmx);
				

				//Movimiento de la matriz
				//Esta es la parte mas lenta del codigo
				//si existiera una forma diferente de hacer los locks, se ha medido que el tiempo 
				//sin esta parte tarda unos 8 segundos.
				omp_set_lock(&row_locker[population[i].x()]);
				pop_matrix[population[i].x()][population[i].y()].remove(i);
				omp_unset_lock(&row_locker[population[i].x()]);
			
				population[i].move(tmm);
				
				omp_set_lock(&row_locker[population[i].x()]);
				pop_matrix[population[i].x()][population[i].y()].push_back(i);
				omp_unset_lock(&row_locker[population[i].x()]);

				//Como solucion a esto se puede utilizar mutex directamente sobre la ubicacion X,Y
				//un metodo aun mas sencillo que nos asegura no solo acceso unico, ademas tiempo reducido en el borrado e insertado de elementos sera usar una matriz de 3 dimensiones
				//pero esto es increiblemente pesado en memoria, se hizo la prueba y aunque es superior en cuanto a velocidad, el equipo donde
				//se realizaron las pruebas no es el mejor para este metodo.
				
				//Procesa las estadisticas que se obtienen en el calculo de enfermedad
				if (status != -1) {
					switch (status)
					{
					case 0:
						#pragma omp atomic
						++tic_deceased;
						#pragma omp atomic
						++total_deceased;
						break;

					case 1:
						#pragma omp atomic
						++tic_infected;
						#pragma omp atomic
						++total_infected;
						break;

					case 2:
						#pragma omp atomic
						++tic_immune;
						#pragma omp atomic
						++total_immune;
						break;
						
					default:
						break;
					}
				}
			}
		}
		//Se guardan los datos en buffers, inicialmente era uno solo pero se decide utilizar 2 para crear los 2 graficos a la vez en caso de confusion	
		DATA_BUFFER += std::to_string(tic_infected) + "," + std::to_string(tic_deceased) + "," + std::to_string(tic_immune) + "\n";
		TOTAL_DATA_BUFFER += std::to_string(total_infected) + "," + std::to_string(total_deceased) + "," + std::to_string(total_immune) + "\n";
	}
	

	#pragma omp parallel for num_threads(MAX_THREADS)
	for (int i = 0; i < tmm; i++) {
		omp_destroy_lock(&row_locker[i]);
	}

	double final_time = omp_get_wtime();

	std::cout << "Final time: " << final_time - init_time << std::endl;

	//Se escriben los datos finales a los 2 archivos de salida CSV y finaliza el programa.
	std::string path = argv[2];
	std::ofstream csv_data;
	path += "_partial.csv";
	csv_data.open(path);
	csv_data << DATA_BUFFER;
	csv_data.close();

	path = argv[2];
	path += "_total.csv";
	csv_data.open(path);
	csv_data << TOTAL_DATA_BUFFER;
	csv_data.close();
}