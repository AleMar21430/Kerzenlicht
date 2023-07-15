#include "R_Light.h"

R_Light::R_Light() {
	Name = "Light";
	Type = Light_Type::Point;
}

void R_Light::read(std::string Input) {
}

std::string R_Light::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Light::size() {
	return 0;
}

R_Light::R_Point_Light::R_Point_Light() {
	Intensity = 10;
	Color = R_Color(2000, 560, 100);
	Radius = 0.25L;
}

void R_Light::R_Point_Light::read(std::string Input) {
}

std::string R_Light::R_Point_Light::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Light::R_Point_Light::size() {
	return 0;
}