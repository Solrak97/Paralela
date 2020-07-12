#include "Coordinate.h"


#pragma once
//Enum para representar el estado de salud de un individuo
enum Health {
	healty, infected, deceased, immune
};


//Clase individuo, representa a un solo individuo de la poblacion
//Su estado de salud, edad, movimiento, hogar y locacion
class Individual {
public:
	Health status;
	double probability_of_recuperation;
	int days_of_infection;
	int days_of_immunity;
	int velocity;
	int radius;
	Coordinate home;
	Coordinate location;

	//Constructores para la clase individuo, el constructor default es necesario para la instanciacion en templates como std::vector
	Individual();
	Individual(bool age, double prj, double prm, int rmj, int rmm, int vmj, int vmm, Coordinate home);

	//Metodo de infeccion, recibe como parametros la cantidad minima y maxima de dias en los que se puede infectar un individuo
	int infect(int dmn, int dmx);

	//Este metodo actualiza el estado de enfermedad
	//Cada TIC disminuye el tiempo de inmunidad, tiempo de enfermedad
	//calcula muertes, nuevas infecciones, o en caso de sobrevivir calcula la inmunidad
	//En caso de que el individuo no cuente para las estadisticas retorna un -1
	int calculate_illness(int number_infected, double piv, int doi, int dmn, int dmx);

	//Mueve al individuo en la mtariz calculando su nueva posicion dentro de un rango dado
	//en caso de que el rango sea 0, se movera libremente
	void move(int tmm);



	//N3ecesitaba un acceso rapido a las variables x,y de la locacion
	int x();
	int y();
};