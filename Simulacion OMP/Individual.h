#include "Coordinate.h"


#pragma once
class Individual {
public:
	enum Health {
		healty, infected, deceased, immune
	};

	Coordinate home;
	Coordinate location;
	int probability_of_survival;
	int days_of_infection;
	int days_of_immunity;
	int velocity;
	int radius;

	Individual();
	void constructor_old();
	void constructor_young();
	int infect();
	int terinate_illness();
	void move(int tmm);

};