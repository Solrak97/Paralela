#include <omp.h>
#include <vector>
#include <iostream>
#include <fstream>

#include <chrono>
#include <thread> 
//std::this_thread::sleep_for(std::chrono::seconds(10));


/*
#pragma omp critical LogUpdate
			{
				// critical section where you update file
			}
*/


int main() {
	//Variables de contador
	int contador_renos = 0;
	int contador_duendes = 0;
	int total_duendes;
	int iteraciones;

	//Locks
	omp_lock_t santa_lock;
	omp_lock_t reno_lock;
	omp_lock_t duende_lock;
	omp_init_lock(&santa_lock);
	omp_init_lock(&reno_lock);
	omp_init_lock(&duende_lock);

	//Roles
	enum Rol {Santa, Reno, Duende};

	//Estructura de roles
	std::vector<Rol> roles;

	//Lectura de valores para duendes
	std::cout << "Cuantos threads de duendes desea usar?" << std::endl;
	std::cin >> total_duendes;
	std::cout << std::endl;

	//Lectura de valores para iteraciones
	std::cout << "Cuantos dias vas a iterar?" << std::endl;
	std::cin >> iteraciones;
	std::cout << std::endl;
	
	int c_threads = 10 + total_duendes;

	roles.resize(c_threads);
	roles[0] = Santa;
	
	for (int i = 1; i < 10; i++) {
		roles[i] = Reno;
	}

	for (int i = 10; i < c_threads; i++) {
		roles[i] = Duende;
	}
	
	#pragma omp parallel num_threads(c_threads)
	{
		int thread_id = omp_get_thread_num();

		if (roles[thread_id] == Santa) {
			printf_s("Santa\n");
		}

		else if (roles[thread_id] == Reno) {
			omp_set_lock(&reno_lock);
			
			contador_renos++;

			omp_unset_lock(&reno_lock);


		}

		else {
			printf_s("Duende\n");
		}
	}

	std::cout  

	//Destruccion de locks
	omp_destroy_lock(&santa_lock);
	omp_destroy_lock(&reno_lock);
	omp_destroy_lock(&duende_lock);
}