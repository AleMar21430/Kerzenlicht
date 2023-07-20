#pragma once

#include "Math.h"

struct Vertex {
	Vec3 Pos;
	Rgb Color;

	Vertex();
	Vertex(Vec3 P_Pos);
	Vertex(Vec3 P_Pos, Rgb P_Color);
};

struct Mesh_Face {
	uint32_t I1, I2, I3;

	Mesh_Face(uint32_t P_I1, uint32_t P_I2, uint32_t P_I3);
};

struct Mesh {
	std::string Name;
	std::vector<Mesh_Face> Faces;
	std::vector<Vec3> Vertex_Positions;
	std::map<std::string, std::vector<Rgb>> Vertex_Colors;
	std::map<std::string, std::map<std::string, double>> Vertex_Weights;

	std::vector<Vertex> Vertex_Output;

	Mesh();
};