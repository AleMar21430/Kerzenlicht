#include "R_Camera.h"

R_Camera::R_Camera() {
	Name = "Camera";
	FOV = 90;
	Clip_Start = 0.01L;
	Clip_End = 10000;
	DOF_Focus_Distance = 10;
	DOF_F_Stops = 2.8L;
	Depth_Of_Field = false;
}

void R_Camera::read(std::string  Input) {
}

std::string R_Camera::write(uint8_t Tab) {
	R_String Result;
	return Result.write();
}

uint64_t R_Camera::size() {
	return 0;
}