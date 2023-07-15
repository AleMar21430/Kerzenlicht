#include "R_Force.h"

R_Force::R_Force() {
	Name = "Force";
	Type = Force_Type::Wind;
}

void R_Force::read(std::string Input) {
}

std::string R_Force::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Force::size() {
	return 0;
}

R_Force::R_Wind_Force::R_Wind_Force() {
	Strength = 1;
}

void R_Force::R_Wind_Force::read(std::string Input) {
}

std::string R_Force::R_Wind_Force::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Force::R_Wind_Force::size() {
	return 0;
}