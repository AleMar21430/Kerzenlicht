#pragma once

#include "R_String.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <variant>
#include <cstdlib>
#include <vector>

#define PI          3.1415926535897932384626433832795
#define TWO_PI      6.2831853071795864769252867665590
#define INVERTED_PI 0.3183098861837906715377675267450


struct Math {
	static float fastInvSqrt(float number);
	static std::vector<std::string> splitString(std::string& input, std::string delimiter);
};
