#include "R_Curve.h"

R_Curve::R_Curve() {
	Name = "Curve";
}

void R_Curve::read(std::string Input) {
}

std::string R_Curve::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Curve::size() {
	return 0;
}