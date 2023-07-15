#include "R_Vec_2.h"

R_Vec_2::R_Vec_2(long double P_X, long double P_Y) {
	X = P_X, Y = P_Y;
}

void R_Vec_2::read(std::string Input) {
}

std::string R_Vec_2::write(uint8_t Tab) {
	R_String Result;
	Result << "Vec_2 << [ " << X << " | " << Y << " ]";
	return Result.write();
}

uint64_t R_Vec_2::size() {
	return 0;
}