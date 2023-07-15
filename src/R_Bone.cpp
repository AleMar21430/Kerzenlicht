#include "R_Bone.h"

R_Bone::R_Bone() {
	Name = "Bone";
}

void R_Bone::read(std::string Input) {
}

std::string R_Bone::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Bone::size() {
	return 0;
}