#pragma once

//as the code in the python version used a little bit of tricky math
//We'll overload the operators to do so, thats why we needed a coord class

class Coordinate {
	int x;
	int y;

public:
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
};