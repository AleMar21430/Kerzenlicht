#include "R_Simulation.h"

R_Simulation::R_Simulation() {
	Name = "Simulation";
	Realtime = true;
}

void R_Simulation::read(std::string Input) {
}

std::string R_Simulation::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Simulation::size() {
	return 0;
}