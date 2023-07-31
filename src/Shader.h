#pragma once

#include "Math.h"

struct Shader {
};

struct Texture {
	uint16_t Width, Height;
	vector<vector<Rgba>> Pixmap;

	Texture();
	
	void loadfromBitmap(string P_File_Path);
};