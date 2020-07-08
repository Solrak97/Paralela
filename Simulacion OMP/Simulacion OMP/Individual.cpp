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

	this->status = healty;
	this->days_of_immunity = -1;
	this->days_of_infection = -1;
	this->home = home;
	this->location = home;
}


int Individual::infect(int dmn, int dmx)
{
	if(this->status == healty){
		this->status = infected;
		this->days_of_infection = rand() % (dmx - dmn) + dmn;
		return 1;
	}
	return 0;
}

int Individual::calculate_illness(int number_infected, double piv, int doi, int dmn, int dmx)
{

	//returns - 1 if not necessary on calcs
	//returns 0 if dies
	//returns 1 if gets immunized
	//return 2 if infected

	switch (this->status)
	{
		case healty:
			for (int i = 0; i < number_infected; i++) {
				if (((float)rand()) / (float)RAND_MAX <= piv) {
					//std::cout << "Infects" << std::endl;
					return infect(dmn, dmx);
				}
			}
			break;

		case infected:
			if (this->days_of_infection > 0) {
				this->days_of_infection--;
			}
			else {
				double surv_roll = ((float)rand()) / (float)RAND_MAX;
				if (surv_roll < 1 - this->probability_of_recuperation) {
					this->status = deceased;
					//std::cout << "Dies" << std::endl;
					return 0;
				}
				else {
					this->status = immune;
					this->days_of_immunity = doi;
					//std::cout << "Immune" << std::endl;
					return 2;
				}
			}
			break;

		case deceased:
			return -1;

		case immune:
			if (this->days_of_immunity > 0) {
				this->days_of_immunity--;
			}
			else {
				this->status = healty;
			}
			break;
	}
	return -1;
}

void Individual::move(int tmm)
{
	Coordinate base = Coordinate();
	Coordinate top = Coordinate(tmm, tmm);
	int index = rand() % 8;
	int final_index = index + 8;
	Coordinate direction[8] = { Coordinate(-1, 0), Coordinate(0, -1), Coordinate(1, 0), Coordinate(0, 1), Coordinate(-1, -1),
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
