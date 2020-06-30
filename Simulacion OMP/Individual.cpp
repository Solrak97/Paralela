#include "Individual.h"

#include <fstream>


Individual::Individual()
{
}

Individual::Individual(bool age, double prj, double prm, int rmj, int rmm, int vmj, int vmm, Coordinate home)
{
	//AGE == true : Over 65
	if (age) {
		this->probability_of_recuperation = prm;
		this->velocity = vmm;
		this->radius = rmm;
	}
	else {
		this->probability_of_recuperation = prj;
		this->velocity = vmj;
		this->radius = rmj;
	}
	this->days_of_immunity = -1;
	this->days_of_infection = -1;
	this->home = home;
	this->location = home;
}


int Individual::infect(int dmn, int dmx)
{
	if(this->status == healty){
		this->status = infected;
		this->days_of_infection = Utility::discrete_uniform_random(dmn, dmx);
		return 1;
	}
	return 0;
}

int Individual::terminate_illness()
{
	return 0;
}

void Individual::move(int tmm)
{
	Coordinate base = Coordinate();
	Coordinate top = Coordinate(tmm, tmm);
	int index = Utility::discrete_uniform_random(0, 8);
	int final_index = index + 8;
	Coordinate direction[8] = { Coordinate(-1, 0), Coordinate(0, -1), Coordinate(1, 0), Coordinate(-1, 0), Coordinate(-1, -1),
		Coordinate(1, -1), Coordinate(-1, 1), Coordinate(1, 1) };

	for (int i = 0; i < final_index; i++) {
		Coordinate destination = this->location + direction[i % 8] * this->velocity;
		if (destination >= base && destination < top) {
			Coordinate home_radius = this->home + direction[i % 8] * this->radius;
			if (destination.abs() < home_radius.abs() || radius == 0) {
				this->location = destination;
				break;
			}
		}
	}
}
