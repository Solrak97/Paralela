#include "Coordinate.h"

Coordinate::Coordinate(int x, int y)
{
	this->x = x;
	this->y = y;
}

//Coord sum
Coordinate Coordinate :: operator + (Coordinate const& C2)
{
	return Coordinate(this->x + C2.x, this->y + C2.y);
}

//Coord Scalar multiplication
Coordinate Coordinate :: operator * (int S)
{
	return Coordinate(this->x * S, this->y * S);
}

//Is Coord C1 < Coord C2?
bool Coordinate :: operator < (Coordinate const& C2)
{
	return (this->x < C2.x) && (this->y < C2.y);
}

//Returns absolute value of Coord
Coordinate Coordinate::abs()
{
	return Coordinate(this->x > 0 ? x : x * -1, this->y > 0 ? y : y * -1);
}