#pragma once

#include "Math.h"

struct Camera {
	std::string Name;
	double FOV;
	double Clip_Start;
	double Clip_End;
	double DOF_Focus_Distance;
	double DOF_F_Stops;
	bool   Depth_Of_Field;

	Camera();
};