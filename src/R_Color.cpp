#include "R_Color.h"

R_Color::R_Color(uint16_t P_R, uint16_t P_G, uint16_t P_B, uint16_t P_A) {
	R = P_R, G = P_G, B = P_B, A = P_A;
}

void R_Color::read(std::string Input) {
}

std::string R_Color::write(uint8_t Tab) {
	R_String Result;
	Result << "Color << [ " << R << " | " << G << " | " << B << " | " << A << " ]";
	return Result.write();
}

uint64_t R_Color::size() {
	return 0;
}