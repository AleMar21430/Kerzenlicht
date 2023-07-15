#include "R_Object.h"

R_Object::R_Object() {
	Name  = "Object";
	Pivot = "World";
	Position = R_Vec_3(0, 0, 0);
	Rotation = R_Vec_3(0, 0, 0);
	Scale = R_Vec_3(1, 1, 1);
}

void R_Object::read(std::string Input) {
}

std::string R_Object::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Object::size() {
	return 0;
}