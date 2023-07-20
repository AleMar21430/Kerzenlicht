#pragma once

#include "Math.h"

enum Force_Type {
	Wind,
	Vortex,
	Point
};

struct R_Force {
	std::string Name;
	Force_Type Type;
	double Strength;
	
	R_Force();
};