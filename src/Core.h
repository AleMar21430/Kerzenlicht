#pragma once

#include "Math.h"

enum Object_Type {
	CAMERA,
	CURVE,
	EMPTY,
	MESH,
	PIVOT
};

enum Render_Mode {
	WIREFRAME,
	POINTCLOUD,
	VISUALIZER
};

struct Rgb {
	float R, G, B;

	Rgb();
	Rgb(float P_R, float P_G, float P_B);

	static Rgb random();
};

struct Rgba {
	float R, G, B, A;

	Rgba();
	Rgba(float P_R, float P_G, float P_B, float P_A);

	static Rgba random();
	static Rgba fromRgb(Rgb P_Rgb);
};

struct Vec2 {
	double X, Y;

	Vec2();
	Vec2(double P_X, double P_Y);

	Vec2 operator+(const Vec2& other) {
		Vec2 Result = Vec2();
		Result.X = X + other.X;
		Result.Y = Y + other.Y;
		return Result;
	}
	Vec2 operator-(const Vec2& other) {
		Vec2 Result = Vec2();
		Result.X = X - other.X;
		Result.Y = Y - other.Y;
		return Result;
	}
	Vec2 operator*(const Vec2& other) {
		Vec2 Result = Vec2();
		Result.X = X * other.X;
		Result.Y = Y * other.Y;
		return Result;
	}
	Vec2 operator/(const Vec2& other) {
		Vec2 Result = Vec2();
		Result.X = X / other.X;
		Result.Y = Y / other.Y;
		return Result;
	}
	Vec2 operator*(const double& other) {
		Vec2 Result = Vec2();
		Result.X = X / other;
		Result.Y = Y / other;
		return Result;
	}

	double dot(const Vec2& other);
	Vec2 cross(const Vec2& other);
	double len();
	Vec2 normalize();
};

struct Vec3 {
	double X, Y, Z;

	Vec3();
	Vec3(double P_X, double P_Y, double P_Z);

	Vec3 operator+(const Vec3& other) {
		Vec3 Result = Vec3();
		Result.X = X + other.X;
		Result.Y = Y + other.Y;
		Result.Z = Z + other.Z;
		return Result;
	}
	Vec3 operator-(const Vec3& other) {
		Vec3 Result = Vec3();
		Result.X = X - other.X;
		Result.Y = Y - other.Y;
		Result.Z = Z - other.Z;
		return Result;
	}
	Vec3 operator*(const Vec3& other) {
		Vec3 Result = Vec3();
		Result.X = X * other.X;
		Result.Y = Y * other.Y;
		Result.Z = Z * other.Z;
		return Result;
	}
	Vec3 operator/(const Vec3& other) {
		Vec3 Result = Vec3();
		Result.X = X / other.X;
		Result.Y = Y / other.Y;
		Result.Z = Z / other.Z;
		return Result;
	}
	Vec3 operator*(const double& other) {
		Vec3 Result = Vec3();
		Result.X = X / other;
		Result.Y = Y / other;
		Result.Z = Z / other;
		return Result;
	}

	double dot(const Vec3& other);
	Vec3 cross(const Vec3& other);
	double len();
	Vec3 normalize();
	Vec3 rotate(Vec3 P_Pos, const Vec3 P_Anchor, const Vec3 P_Rotation);
	void rotate(const Vec3 P_Anchor, const Vec3 P_Rotation);
};

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
	std::vector<Mesh_Face> Faces;
	std::vector<Vec3> Vertex_Positions;
	std::map<std::string, std::vector<Rgb>> Vertex_Colors;
	std::map<std::string, std::map<std::string, double>> Vertex_Weights;

	std::vector<Vertex> Vertex_Output;

	Mesh();
};

struct Object {
	std::string Name;
	Object_Type Type;

	Mesh MeshData;

	Vec3 Pos;
	Vec3 Anchor;
	Vec3 Rot_Euler;
	Vec3 Scale;

	Object();
	Object(std::string P_Name, Object_Type P_Type);

	void loadBuffers();
	void scale(Vec3 P_Scale);
	void setScale(Vec3 P_Scale);
	void rotate(Vec3 P_Rot);
	void translate(Vec3 P_Pos = Vec3());
	void processTransform();
};

struct Camera {

};