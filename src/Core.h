#pragma once

#include "Math.h"

struct Rgba {
	float R, G, B, A;

	Rgba(float P_R = 1.0f, float P_G = 1.0f, float P_B = 1.0f, float P_A = 1.0f) {
		R = P_R;
		G = P_G;
		B = P_B;
		A = P_A;
	}

	static Rgba random() {
		return Rgba(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), 1);
	}
};

struct Vec2 {
	double X, Y;

	Vec2(double P_X = 0.0f, double P_Y = 0.0f) {
		X = P_X;
		Y = P_Y;
	}

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

	double dot(const Vec2& other) {
		return X * other.X + Y * other.Y;
	}

	Vec2 cross(const Vec2& other) {
		Vec2 Result = Vec2();
		Result.X = Y * other.Y - Y * other.Y;
		Result.Y = Y * other.X - X * other.Y;
		return Result;
	}

	long double len() {
		return sqrt(X * X + Y * Y);
	}

	Vec2 normalize() {
		Vec2 Result = Vec2();
		double Length = 1L / this->len();
		X = X * Length;
		Y = Y * Length;
		return *this;
	}
};

struct Vec3 {
	double X, Y, Z;

	Vec3(double P_X = 0.0f, double P_Y = 0.0f, double P_Z = 0.0f) {
		X = P_X;
		Y = P_Y;
		Z = P_Z;
	}

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

	double dot(const Vec3& other) {
		return X * other.X + Y * other.Y + Z * other.Z;
	}

	Vec3 cross(const Vec3& other) {
		Vec3 Result = Vec3();
		Result.X = Y * other.Z - Z * other.Y;
		Result.Y = Z * other.X - X * other.Z;
		Result.Z = X * other.Y - Y * other.X;
		return Result;
	}

	long double len() {
		return sqrt(X * X + Y * Y + Z * Z);
	}

	Vec3 normalize() {
		Vec3 Result = Vec3();
		double Length = 1L / this->len();
		X = X * Length;
		Y = Y * Length;
		Z = Z * Length;
		return *this;
	}
};

struct Vertex {
	Vec3 Pos;
	Rgba Color;

	Vertex(Vec3 P_Pos = Vec3()) {
		Pos = P_Pos;
	}
};

struct Tri {
	uint32_t I1, I2, I3;

	Tri(uint32_t P_I1, uint32_t P_I2, uint32_t P_I3) {
		I1 = P_I1;
		I2 = P_I2;
		I3 = P_I3;
	}
};

struct Mesh {
	std::vector<Vertex> Vertices;
	std::vector<Tri> Triangles;

	std::vector<Vertex> Vertex_Buffer;
	std::vector<Tri> Triangle_Buffer;

	Mesh() {
		Vertices = std::vector<Vertex>();
		Triangles = std::vector<Tri>();

		Vertex_Buffer = std::vector<Vertex>();
		Triangle_Buffer = std::vector<Tri>();
	}
};

enum Object_Type {
	CAMERA,
	CURVE,
	MESH,
	PIVOT
};

struct Object {
	std::string Name;
	Vec3 Pos;
	Vec3 Rot_Euler;
	Vec3 Scale;

	Object_Type Type;
	Mesh MeshData;

	Object(std::string P_Name = "Object", Object_Type P_Type = MESH) {
		Name = P_Name;
		Type = P_Type;
		Pos = Vec3();
		Rot_Euler = Vec3();
		Scale = Vec3(1, 1, 1);

		if (Type == MESH) {
			MeshData = Mesh();
		}
	}

	void renderPass() {
		if (Type == MESH) {
			MeshData.Vertex_Buffer = MeshData.Vertices;
			MeshData.Triangle_Buffer = MeshData.Triangles;
		}
		
	}

	void scale(double P_Scale = 1) {
		if (Type == MESH) {
			for (Vertex& vert : MeshData.Vertex_Buffer) {
				vert.Pos.X *= P_Scale;
				vert.Pos.Y *= P_Scale;
				vert.Pos.Z *= P_Scale;
			}
		}
	}

	void translate(Vec3 P_Pos = Vec3()) {
		if (Type == MESH) {
			for (Vertex& vert : MeshData.Vertex_Buffer) {
				vert.Pos.X += P_Pos.X;
				vert.Pos.Y += P_Pos.Y;
				vert.Pos.Z += P_Pos.Z;
			}
		}
	}
};

struct Camera {

};