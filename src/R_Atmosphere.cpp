#include "R_Atmosphere.h"

R_Atmosphere::R_Atmosphere() {
	Name = "Atmosphere";
}

void R_Atmosphere::read(std::string Input) {
}

std::string R_Atmosphere::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Atmosphere::size() {
	return 0;
}