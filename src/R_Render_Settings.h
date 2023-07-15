#pragma once

#include "R_String.h"

struct R_Render_Settings {
	uint16_t    VP_SPP,                   RD_SPP;
	bool        VP_Denoising,             RD_Denoising;
	bool        VP_Caustics,              RD_Caustics;
	uint16_t    VP_Diffuse_Bounces,       RD_Diffuse_Bounces;
	uint16_t    VP_Reflective_Bounces,    RD_Reflective_Bounces;
	uint16_t    VP_Translucent_Bounces,   RD_Translucent_Bounces;
	uint16_t    VP_SubSurface_Bounces,    RD_SubSurface_Bounces;
	uint16_t    VP_Volume_Bounces,        RD_Volume_Bounces;
	long double VP_Resolution_Percentage, RD_Resolution_Percentage;

	bool        VP_Limit_To_Camera_Mask;
	uint16_t    VP_Denoising_Start;
	uint32_t    RD_Res_X, RD_Res_Y;

	R_Render_Settings();
	
	void read(std::string Input);
	std::string write(uint8_t Tab = 0);
	uint64_t size();
};