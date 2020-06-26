#include <omp.h>
#include <vector>
#include <iostream>
#include <fstream>

#include <ctime>
#include <random>

//Generacion de numeros aleatorios
double getRandom()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);

	return  dis(gen);
}

// al no usar semaforos es dificil utilizar a santa como thread, sin emargo se intentara hacer una version 2
void santa(bool estado, int& duendes, bool& navidad) {
	if (estado) {
		printf_s("\nSanta sale a preparar el trineo\nFeliz Navidad!\n");
		navidad = true;
	}

	else {
		printf_s("\nSanta sale a ayudar un grupo de duendes\n");
		duendes = 0;
	}
}


int main() {
	//Variables de contador
	int contador_renos = 0;
	int contador_duendes = 0;
	int total_duendes;
	bool navidad = false;

	double prob_reno = 0.2;
	double prob_duende = 0.5;

	//Locks
	omp_lock_t oficina_lock;

	omp_init_lock(&oficina_lock);

	//Roles
	enum Rol { Santa, Reno, Reno_Descanso, Duende, Consulta };

	//Estructura de roles
	std::vector<Rol> roles;

	//Lectura de valores para duendes
	std::cout << "Cuantos threads de duendes desea usar?" << std::endl;
	std::cin >> total_duendes;
	std::cout << std::endl;

	//Lectura de valores para iteraciones
	std::cout << "Cuantos dias vas a iterar?" << std::endl;

	int c_threads = 10 + total_duendes;


	//Rellenado de roles
	roles.resize(c_threads);
	roles[0] = Santa;

	for (int i = 1; i < 10; i++) {
		roles[i] = Reno;
	}

	for (int i = 10; i < c_threads; i++) {
		roles[i] = Duende;
	}



	//Simulacion de los tics
	while(!navidad) {
		printf_s("\nNuevo dia de simulacion\n");
		#pragma omp parallel num_threads(c_threads)
		{
			int thread_id = omp_get_thread_num();

			//Threads de renos
			if (roles[thread_id] == Reno) {
				if (getRandom() < prob_reno) {
					#pragma omp critical (Renos)
					{
						contador_renos++;
						printf_s("\nEl reno %d vuelve a la cabana\n", thread_id - 1);
					}
					if (contador_renos == 9) {
						//Se llama la funcion de santa para controlar a los renos
						santa(true, contador_duendes, navidad);
					}
					roles[thread_id] = Reno_Descanso;
				}
			}
			

			//Thread del duende
			if (roles[thread_id] == Duende) {
				#pragma omp critical (Duendes)
				{
					//Se intenta que no lleguen duendes luego de navidad, pero a veces pasa
					if (getRandom() < prob_duende && !navidad) {
						contador_duendes++;
						printf_s("\nEl duende %d de une a un grupo para ver a Santa\n", thread_id - 10);
					}
					if (contador_duendes == 3) {
						//Se llaman los duendes para recibir ayuda
						santa(false, contador_duendes, navidad);
					}
				}
			}

		}
	}

	//Destruccion de locks
	omp_destroy_lock(&oficina_lock);
}
