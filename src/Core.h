#pragma once

#include "Math.h"

struct Rgb {
	float R, G, B;
	Rgb() {
		R = 1;
		G = 1;
		B = 1;
	}
	Rgb(float P_R, float P_G, float P_B) {
		R = P_R;
		G = P_G;
		B = P_B;
	}

	static Rgb random() {
		return Rgb(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX));
	}
};

struct Rgba {
	float R, G, B, A;
	Rgba() {
		R = 1;
		G = 1;
		B = 1;
		A = 1;
	}
	Rgba(float P_R, float P_G, float P_B, float P_A) {
		R = P_R;
		G = P_G;
		B = P_B;
		A = P_A;
	}

	static Rgba random() {
		return Rgba(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), 1);
	}

	static Rgba fromRgb(Rgb P_Rgb) {
		return Rgba(P_Rgb.R, P_Rgb.G, P_Rgb.B, 1);
	}
};

struct Vec2 {
	double X, Y;

	Vec2() {
		X = 0;
		Y = 0;
	}
	Vec2(double P_X, double P_Y) {
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

	Vec3() {
		X = 0;
		Y = 0;
		Z = 0;
	}
	Vec3(double P_X, double P_Y, double P_Z) {
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
	Rgb Color;

	Vertex() {
		Pos = Vec3();
		Color = Rgb();
	}
	Vertex(Vec3 P_Pos) {
		Pos = P_Pos;
		Color = Rgb();
	}
	Vertex(Vec3 P_Pos, Rgb P_Color) {
		Pos = P_Pos;
		Color = P_Color;
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
	std::vector<Tri> Faces;
	std::vector<Vec3> Vertex_Positions;
	std::map<std::string, std::vector<Rgb>> Vertex_Colors;
	std::map<std::string, std::map<std::string, double>> Vertex_Weights;

	std::vector<Vertex> Vertex_Output;

	Mesh() {
		Faces = std::vector<Tri>();
		Vertex_Positions = std::vector<Vec3>();
		Vertex_Colors = std::map<std::string, std::vector<Rgb>>() ;
		Vertex_Weights = std::map<std::string, std::map<std::string, double>>() ;

		Vertex_Output = std::vector<Vertex>();
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
			MeshData.Vertex_Output = std::vector<Vertex>();
			for (const Vec3 &Pos : MeshData.Vertex_Positions) {
				MeshData.Vertex_Output.push_back(Vertex(Pos));
			}
			MeshData.Faces = MeshData.Faces;
		}
		
	}

	void scale(double P_Scale = 1) {
		if (Type == MESH) {
			for (Vertex& vert : MeshData.Vertex_Output) {
				vert.Pos.X *= P_Scale;
				vert.Pos.Y *= P_Scale;
				vert.Pos.Z *= P_Scale;
			}
		}
	}

	void translate(Vec3 P_Pos = Vec3()) {
		if (Type == MESH) {
			for (Vertex& vert : MeshData.Vertex_Output) {
				vert.Pos.X += P_Pos.X;
				vert.Pos.Y += P_Pos.Y;
				vert.Pos.Z += P_Pos.Z;
			}
		}
	}
};

struct Camera {

};