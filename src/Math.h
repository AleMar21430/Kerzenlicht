#pragma once

#include "R_String.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <variant>
#include <cstdlib>
#include <utility>
#include <vector>
#include <thread>
#include <future>
#include <map>
#include <omp.h>

#define PI          3.141592653589793
#define TWO_PI      6.283185307179586
#define INVERTED_PI 0.318309886183791

using namespace std;

struct Vec2;
struct Vec3;
struct Matrix_3x3;
struct Matrix_4x4;

struct Vec2 {
	double X, Y;

	Vec2();
	Vec2(double P_X, double P_Y);

	Vec2 operator+(const Vec2& other);
	Vec2 operator-(const Vec2& other);
	Vec2 operator*(const Vec2& other);
	Vec2 operator/(const Vec2& other);
	Vec2& operator+=(const Vec2& other);
	Vec2& operator-=(const Vec2& other);
	Vec2& operator*=(const Vec2& other);
	Vec2& operator/=(const Vec2& other);

	double dot(const Vec2& other);
	Vec2 cross(const Vec2& other);
	double len();
	Vec2 normalize();
};

struct Vec3 {
	double X, Y, Z;

	Vec3();
	Vec3(double P_X, double P_Y, double P_Z);

	Vec3 operator+(const Vec3& other);
	Vec3 operator-(const Vec3& other);
	Vec3 operator*(const Vec3& other);
	Vec3 operator/(const Vec3& other);
	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);
	Vec3& operator*=(const Vec3& other);
	Vec3& operator/=(const Vec3& other);

	Vec3& operator*(const double& other);

	double dot(const Vec3& other);
	Vec3 cross(const Vec3& other);
	double len();
	Vec3 normalize();
	Vec3 rotate(Vec3 P_Pos, const Vec3 P_Anchor, const Vec3 P_Rotation);
	void rotate(const Vec3 P_Anchor, const Vec3 P_Rotation);
};

struct Matrix_3x3 {
	double matrix[3][3];

	Matrix_3x3 operator*(Matrix_3x3 other);
};

struct Vec4 {
	double X, Y, Z, W;

	Vec4();
	Vec4(const Vec3& P_XYZ, double P_W);
	Vec4(double P_X, double P_Y, double P_Z, double P_W);

	Vec4& operator*(const Matrix_4x4& other);
};

struct Matrix_4x4 {
	vector<vector<double>> m;

	Matrix_4x4();
	Matrix_4x4(vector<vector<double>> P_Input);

	Matrix_4x4 operator*(Matrix_4x4 other);
};

struct Rgb {
	float R, G, B;

	Rgb();
	Rgb(float P_R, float P_G, float P_B);

	Rgb operator+(Rgb other);

	Rgb operator*(float other);

	static Rgb random();
};

struct Rgba {
	float R, G, B, A;

	Rgba();
	Rgba(Rgb P_Color, float P_Alpha);
	Rgba(float P_R, float P_G, float P_B, float P_A);

	Rgba& operator*(const float& other);

	Rgba clamp();

	static Rgba random();
	static Rgba fromRgb(Rgb P_Rgb);
};

struct Math {
	static float fastInvSqrt(float number);
	static std::vector<std::string> splitString(std::string& input, std::string delimiter);
	static std::vector<std::string> splitString(std::string& input);
	static void centerPointsToOrigin(std::vector<std::pair<int, int>>& points);
};