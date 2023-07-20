#pragma once

#include "Math.h"

enum Light_Type {
	Directional,
	Spot,
	Point,
	Area
};

struct R_Light {
	std::string Name;
	Light_Type Type;
	double Intensity;
	Rgb Color;
	double Radius;
	
	R_Light();
};