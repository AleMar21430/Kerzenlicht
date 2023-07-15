#include "R_Pivot.h"

R_Pivot::R_Pivot() {
	Name = "Pivot";
	Pivot = "World";
	Position = R_Vec_3(0, 0, 0);
	Rotation = R_Vec_3(0, 0, 0);
	Scale = R_Vec_3(1, 1, 1);
}

void R_Pivot::read(std::string Input) {
}

std::string R_Pivot::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Pivot::size() {
	return 0;
}