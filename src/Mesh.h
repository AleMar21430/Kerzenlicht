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

	Matrix_4x4 model_matrix;
	vector<Vertex> Vertex_Output;

	Mesh();
	void f_processModelMatrix(const Vec3& P_Translate, const Vec3& P_Rotate, const Vec3& P_Scale);
	void f_processVertexShader(const Matrix_4x4& P_Camera_Matrix, const Matrix_4x4& P_Projection_Matrix, const Matrix_4x4& P_Viewport_Matrix);
};

struct Vertex {
	Vec3 Pos;
	Rgb Color;
	Vec3 Normal;
	Vec2 UV;

	Vertex();
	Vertex(const Vec3& P_Pos);
	Vertex(const Vec3& P_Pos, const Vec2& P_Uv);
	Vertex(const Vec3& P_Pos, const Rgb& P_Color);
	Vertex(const Vec3& P_Pos, const Vec3& P_Normal);
	Vertex(const Vec3& P_Pos, const Rgb& P_Color, const Vec3& P_Normal);
	Vertex(const Vec3& P_Pos, const Vec2& P_Uv, const Vec3& P_Normal);

	Vec2 project(const Vec3& cameraPos, const Vec3& cameraDir, const double& FOV) const;
};

struct Mesh_Triangle {
	size_t Index1, Index2, Index3;
	size_t Normal_1, Normal_2, Normal_3;
	size_t UV_1, UV_2, UV_3;

	Mesh_Triangle(const size_t& P_I1, const size_t& P_I2, const size_t& P_I3);
};