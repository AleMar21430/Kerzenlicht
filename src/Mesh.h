#pragma once

#include "Math.h"

struct Vertex;
struct Mesh_Face;

struct Mesh {
	vector<Mesh_Face> Faces;
	vector<Vec3> Vertex_Positions;
	map<string, vector<Rgb>> Vertex_Colors;
	map<string, map<string, double>> Vertex_Weights;

	vector<Vertex> Vertex_Output;

	Mesh();
	void applyTransformMatrix(const Vec3& P_Translate, const Vec3& P_Rotate, const Vec3& P_Scale);
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
	size_t I1, I2, I3;

	Mesh_Face(size_t P_I1, size_t P_I2, size_t P_I3);
};
