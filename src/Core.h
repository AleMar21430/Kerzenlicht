#pragma once

#include "Math.h"

enum Object_Type {
	CAMERA,
	CURVE,
	MESH,
	PIVOT
};


enum Render_Mode {
	WIREFRAME,
	POINTCLOUD,
	RAINBOW
};


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
		double Length = 1L / this->len();
		X = X * Length;
		Y = Y * Length;
		Z = Z * Length;
		return *this;
	}

	Vec3 rotate(Vec3 P_Pos, const Vec3 P_Anchor, const Vec3 P_Rotation) {
		Vec3 Result = Vec3(P_Pos);

		Result.X -= P_Anchor.X;
		Result.Y -= P_Anchor.Y;
		Result.Z -= P_Anchor.Z;

		float cosX = std::cos(P_Rotation.X);
		float sinX = std::sin(P_Rotation.X);
		float newY = Result.Y * cosX - Result.Z * sinX;
		float newZ = Result.Y * sinX + Result.Z * cosX;

		float cosY = std::cos(P_Rotation.Y);
		float sinY = std::sin(P_Rotation.Y);
		float newX = Result.X * cosY + newZ * sinY;
		newZ = -Result.X * sinY + newZ * cosY;

		float cosZ = std::cos(P_Rotation.Z);
		float sinZ = std::sin(P_Rotation.Z);
		float finalX = newX * cosZ - newY * sinZ;
		float finalY = newX * sinZ + newY * cosZ;

		Result.X = finalX + P_Anchor.X;
		Result.Y = finalY + P_Anchor.Y;
		Result.Z = newZ + P_Anchor.Z;
	}
	void rotate(const Vec3 P_Anchor, const Vec3 P_Rotation) {
		X -= P_Anchor.X;
		Y -= P_Anchor.Y;
		Z -= P_Anchor.Z;

		float cosX = std::cos(P_Rotation.X);
		float sinX = std::sin(P_Rotation.X);
		float newY = Y * cosX - Z * sinX;
		float newZ = Y * sinX + Z * cosX;

		float cosY = std::cos(P_Rotation.Y);
		float sinY = std::sin(P_Rotation.Y);
		float newX = X * cosY + newZ * sinY;
		newZ = -X * sinY + newZ * cosY;

		float cosZ = std::cos(P_Rotation.Z);
		float sinZ = std::sin(P_Rotation.Z);
		float finalX = newX * cosZ - newY * sinZ;
		float finalY = newX * sinZ + newY * cosZ;

		X = finalX + P_Anchor.X;
		Y = finalY + P_Anchor.Y;
		Z = newZ + P_Anchor.Z;
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

struct Object {
	std::string Name;
	Vec3 Pos;
	Vec3 Anchor;
	Vec3 Rot_Euler;
	Vec3 Scale;

	Object_Type Type;
	Mesh MeshData;

	Object(std::string P_Name = "Object", Object_Type P_Type = MESH) {
		Name = P_Name;
		Type = P_Type;
		Pos = Vec3();
		Anchor = Vec3();
		Rot_Euler = Vec3();
		Scale = Vec3(1, 1, 1);

		if (Type == MESH) {
			MeshData = Mesh();
		}
	}

	void loadBuffers() {
		if (Type == MESH) {
			MeshData.Vertex_Output = std::vector<Vertex>();
			for (int i = 0; i < MeshData.Vertex_Positions.size(); i++) {
				Vertex vert = Vertex(MeshData.Vertex_Positions[i]);
				MeshData.Vertex_Output.push_back(vert);
			}
		}
	}

	void scale(Vec3 P_Scale) {
		Scale = Scale + P_Scale;
	}

	void setScale(Vec3 P_Scale) {
		Scale = P_Scale;
	}

	void rotate(Vec3 P_Rot) {
		Rot_Euler = Rot_Euler + P_Rot;
	}

	void translate(Vec3 P_Pos = Vec3()) {
		Pos = Pos + P_Pos;
	}

	void processTransform() {
		if (Type == MESH) {
			for (int i = 0; i < MeshData.Vertex_Positions.size(); i++) {
				Vertex& vert = MeshData.Vertex_Output[i];
				vert.Pos = MeshData.Vertex_Positions[i] * Scale;
				vert.Pos.rotate(Anchor, Rot_Euler);
				vert.Pos = vert.Pos + Pos;
			}
		}
	}
};

struct Camera {

};