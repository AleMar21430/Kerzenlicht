#include "R_Shader.h"

R_Shader::R_Shader() {
	Name = "Shader";
	Roughness = 0L;
}

void R_Shader::read(std::string Input) {
}

std::string R_Shader::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Shader::size() {
	return 0;
}