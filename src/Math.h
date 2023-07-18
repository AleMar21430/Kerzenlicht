#pragma once

#include "R_String.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <variant>
#include <cstdlib>
#include <vector>
#include <map>

#define PI          3.1415926535897932384626433832795
#define TWO_PI      6.2831853071795864769252867665590
#define INVERTED_PI 0.3183098861837906715377675267450

struct Math {
	static float fastInvSqrt(float number) {
		long i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y;							// get bits for floating value
		i = 0x5f3759df - (i >> 1);				// gives initial guess y0
		y = *(float*)&i;						// convert bits back to float
		y = y * (threehalfs - (x2 * y * y));	// Newton step, repeating increases accuracy
		y = y * (threehalfs - (x2 * y * y));
		y = y * (threehalfs - (x2 * y * y));
		return y;
	}

	static std::vector<std::string> splitString(std::string& input, std::string delimiter) {
		std::vector<std::string> tokens;
		std::string::size_type start = 0;
		std::string::size_type end = input.find(delimiter);

		while (end != std::string::npos) {
			tokens.push_back(input.substr(start, end - start));
			start = end + 1;
			end = input.find(delimiter, start);
		}
		tokens.push_back(input.substr(start));

		return tokens;
	}

	static std::vector<std::string> splitString(std::string& input) {
		std::vector<std::string> result;
		std::istringstream iss(input);
		std::string token;

		while (iss >> token) {
			result.push_back(token);
		}

		return result;
	}
};