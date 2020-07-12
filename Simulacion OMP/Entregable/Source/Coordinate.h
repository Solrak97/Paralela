#include <iostream>

#pragma once

//En el proyecto anterior se utilizo un poco de algebra para hacer el movimiento
//En esta ocasion se sobrecargan los operadortes con tal de minimizar y facilitar el trabajo en ese metodo
class Coordinate {
public:
	int x;
	int y;

	Coordinate();
	Coordinate(int x, int y);

	//Coord sum
	Coordinate operator + (Coordinate const &C2);

	//Coord Scalar multiplication
	Coordinate operator * (int S);

	//Is Coord C1 < Coord C2?
	bool operator < (Coordinate const& C2);

	//Is Coord C1 >= Coord C2?
	bool operator >= (Coordinate const& C2);

	//Returns aboslute value of coord
	Coordinate abs();

	//Just for testing
	friend std::ostream& operator << (std::ostream& os, const Coordinate& C);
};