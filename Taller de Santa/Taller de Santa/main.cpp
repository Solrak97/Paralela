#include <omp.h>
#include <queue>
#include <vector>
#include <iostream>
#include <fstream>

#include <chrono>
#include <thread> 
//std::this_thread::sleep_for(std::chrono::seconds(10));

#include "modelos.h"



int main() {

	int contador_renos, contador_duendes = 0;
	int total_duendes;
	int iteraciones;

	std::vector<Reno> calida_cabana;
	std::queue<Duende> fila_duendes;

	//Lectura de valores para duendes
	std::cout << "Cuantos threads de duendes desea usar?" << std::endl;
	std::cin >> total_duendes;
	std::cout << std::endl;

	//Lectura de valores para iteraciones
	std::cout << "Cuantos dias vas a iterar?" << std::endl;
	std::cin >> iteraciones;
	std::cout << std::endl;

	#pragma omp parallel num_threads(10 + total_duendes)
    {
		printf("id = %d, \n", omp_get_thread_num());
    }
}