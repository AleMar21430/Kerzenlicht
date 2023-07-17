#include "Math.h"

float Math::fastInvSqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;           // get bits for floating value
	i = 0x5f3759df - (i >> 1); // gives initial guess y0
	y = *(float*)&i;          // convert bits back to float
	y = y * (threehalfs - (x2 * y * y)); // Newton step, repeating increases accuracy
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y));
	return y;
}

std::vector<std::string> Math::splitString(std::string& input, std::string delimiter) {
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