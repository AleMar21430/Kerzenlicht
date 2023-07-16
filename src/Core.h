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
};

struct Vec3 {
	double X, Y, Z;

	Vec3(double P_X = 0.0f, double P_Y = 0.0f, double P_Z = 0.0f) {
		X = P_X;
		Y = P_Y;
		Z = P_Z;
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

struct Object {
	std::string Name;
	Vec3 Pos;
	Vec3 Rot_Euler;
	Vec3 Scale;

	std::vector<Vertex> Vertex_Buffer;
	std::vector<Tri> Triangle_Buffer;

	Object(std::string P_Name = "Object") {
		Name = P_Name;
		Pos = Vec3();
		Rot_Euler = Vec3();
		Scale = Vec3(1, 1, 1);

		Vertex_Buffer = std::vector<Vertex>();
		Triangle_Buffer = std::vector<Tri>();
	}
};