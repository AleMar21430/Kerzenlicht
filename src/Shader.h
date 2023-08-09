#pragma once

#include "Math.h"

struct Texture {
	uint16_t Width, Height;
	vector<vector<Rgba>> Pixmap;

	Texture();
	
	void loadfromBitmap(const string& P_File_Path);
	Rgba getColor(const Vec2& P_Pos) const;
};

struct Shader {
	Texture Albedo;
	Shader();
};