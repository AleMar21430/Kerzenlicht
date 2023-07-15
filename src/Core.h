#pragma once

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

	Tri() {
		I1 = 0;
		I2 = 0;
		I3 = 0;
	}
};