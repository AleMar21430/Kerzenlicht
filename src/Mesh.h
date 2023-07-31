#pragma once

#include "Math.h"

struct Vertex;
struct Mesh_Triangle;

struct Mesh {
	vector<Mesh_Triangle> Faces;
	vector<Vec3> Vertex_Positions;
	map<string, vector<Vec3>> Vertex_Normals;
	map<string, vector<Vec2>> Vertex_UV_Coords;
	map<string, vector<Rgb>> Vertex_Colors;
	map<string, map<string, double>> Vertex_Weights;

	vector<Vertex> Vertex_Output;

	Mesh();
	void applyTransformMatrix(const Vec3& P_Translate, const Vec3& P_Rotate, const Vec3& P_Scale);
};

struct Vertex {
	Vec3 Pos;
	Rgb Color;
	Vec3 Normal;
	Vec2 UV;

	Vertex();
	Vertex(Vec3 P_Pos);
	Vertex(Vec3 P_Pos, Rgb P_Color);

	Vec2 project(const Vec3& cameraPos, const Vec3& cameraDir, double FOV);
};

struct Mesh_Triangle {
	size_t Index1, Index2, Index3;
	size_t Normal_1, Normal_2, Normal_3;
	size_t UV_1, UV_2, UV_3;

	Mesh_Triangle(size_t P_I1, size_t P_I2, size_t P_I3);
};
