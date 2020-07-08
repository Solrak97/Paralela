#include "Coordinate.h"
#include "Utility.h"

#pragma once
enum Health {
	healty, infected, deceased, immune
};

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

	//Constructor of the class Individual
	Individual();
	Individual(bool age, double prj, double prm, int rmj, int rmm, int vmj, int vmm, Coordinate home);

	//if the individual is healty, it will get infected and return 1, else he will return 0
	int infect(int dmn, int dmx);

	//if illnes days get to 0, will calculate if individuals get imunized or die
	int calculate_illness(int number_infected, double piv, int doi, int dmn, int dmx);

	//moves the individual in a radius, if the radius is 0, he'll move freely
	void move(int tmm);



	//Just for the lazyness
	int x();
	int y();
};