#pragma once

#include "Math.h"

struct Fragment_Shader_Type {
	enum Enum {
		FRESNEL,
		POINTCLOUD,
		TEXTURED,
		SMOOTH,
		VERTEX_COLOR_DEBUG,
		WIREFRAME,
		ZBUFFER_DEBUG
	};
};

struct Texture {
	uint16_t Width, Height;
	vector<vector<Rgba>> Pixmap;

	Texture();
	
	void loadfromBitmap(const string& P_File_Path);
	Rgba getColor(const Vec2& P_Pos) const;
};

struct Shader {
	Texture Albedo;
	Fragment_Shader_Type::Enum Frag_Shader;
	Shader();
};