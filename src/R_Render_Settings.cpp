#include "R_Render_Settings.h"

R_Render_Settings::R_Render_Settings() {
	VP_SPP = 16;
	VP_Denoising = true;
	VP_Caustics = false;
	VP_Diffuse_Bounces = 8;
	VP_Reflective_Bounces = 8;
	VP_Translucent_Bounces = 8;
	VP_SubSurface_Bounces = 8;
	VP_Volume_Bounces = 8;
	VP_Resolution_Percentage = 50;

	RD_SPP = 32;
	RD_Denoising = true;
	RD_Caustics = true;
	RD_Diffuse_Bounces = 16;
	RD_Reflective_Bounces = 32;
	RD_Translucent_Bounces = 32;
	RD_SubSurface_Bounces = 16;
	RD_Volume_Bounces = 32;
	RD_Resolution_Percentage = 100;

	VP_Limit_To_Camera_Mask = true;
	VP_Denoising_Start = 1;

	RD_Res_X = 3840;
	RD_Res_Y = 2160;
}

void R_Render_Settings::read(std::string Input) {
}

std::string R_Render_Settings::write(uint8_t Tab) {
	R_String Result;
	Result.nl(Tab, 0, 1) << "Render Settings {";
	Result.nl(Tab, 1, 2) << "Viewport {";
	Result.nl(Tab, 2, 1) << "Resolution          << " << VP_Resolution_Percentage << " %";
	Result.nl(Tab, 2, 1) << "Samples Per Pixel   << " << VP_SPP;
	Result.nl(Tab, 2, 1) << "Deonising           << " << VP_Denoising;
	Result.nl(Tab, 2, 1) << "Diffuse Bounces     << " << VP_Diffuse_Bounces;
	Result.nl(Tab, 2, 1) << "Reflective Bounces  << " << VP_Reflective_Bounces;
	Result.nl(Tab, 2, 1) << "Translucent Bounces << " << VP_Translucent_Bounces;
	Result.nl(Tab, 2, 1) << "Sub-Surface Bounces << " << VP_SubSurface_Bounces;
	Result.nl(Tab, 2, 1) << "Volume Bounces      << " << VP_Volume_Bounces;
	Result.nl(Tab, 1, 1) << "} Viewport";
	Result.nl(Tab, 1, 2) << "Render {";
	Result.nl(Tab, 2, 1) << "Samples Per Pixel   << " << RD_SPP;
	Result.nl(Tab, 2, 1) << "Deonising           << " << RD_Denoising;
	Result.nl(Tab, 2, 1) << "Diffuse Bounces     << " << RD_Diffuse_Bounces;
	Result.nl(Tab, 2, 1) << "Reflective Bounces  << " << RD_Reflective_Bounces;
	Result.nl(Tab, 2, 1) << "Translucent Bounces << " << RD_Translucent_Bounces;
	Result.nl(Tab, 2, 1) << "Sub-Surface Bounces << " << RD_SubSurface_Bounces;
	Result.nl(Tab, 2, 1) << "Volume Bounces      << " << RD_Volume_Bounces;
	Result.nl(Tab, 1, 1) << "} Render";
	Result.nl(Tab, 0, 2) << "} Render Settings";
	return Result.write();
}

uint64_t R_Render_Settings::size() {
	return 0;
}