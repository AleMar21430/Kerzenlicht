#pragma once

#include "Math.h"

struct Vertex;
struct Mesh_Face;

struct Mesh {
	std::string Name;
	std::vector<Mesh_Face> Faces;
	std::vector<Vec3> Vertex_Positions;
	std::map<std::string, std::vector<Rgb>> Vertex_Colors;
	std::map<std::string, std::map<std::string, double>> Vertex_Weights;

	std::vector<Vertex> Vertex_Output;

	Mesh();
};

struct Vertex {
	Vec3 Pos;
	Rgb Color;

	Vertex();
	Vertex(Vec3 P_Pos);
	Vertex(Vec3 P_Pos, Rgb P_Color);

	Vec2 project(const Vec3& cameraPos, const Vec3& cameraDir, double FOV);
};

struct Mesh_Face {
	uint32_t I1, I2, I3;

	Mesh_Face(uint32_t P_I1, uint32_t P_I2, uint32_t P_I3);
};
