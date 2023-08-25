#pragma once

#include "R_String.h"

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <variant>
#include <cstdlib>
#include <numeric>
#include <stdio.h>
#include <vector>
#include <thread>
#include <future>
#include <math.h>
#include <array>
#include <omp.h>
#include <any>
#include <map>
#include <set>

#include "device_launch_parameters.h"
#include <cuda_runtime.h>

#define PI          3.141592653589793
#define TWO_PI      6.283185307179586
#define INVERTED_PI 0.318309886183791
#define RAD         0.017453292519943

using namespace std;

struct Vec2;
struct Vec3;
struct Matrix_3x3;
struct Matrix_4x4;

struct Vec2 {
	double X, Y;

	Vec2();
	Vec2(double P_X, double P_Y);

	Vec2 operator+(const Vec2& other) const;
	Vec2 operator-(const Vec2& other) const;
	Vec2 operator*(const Vec2& other) const;
	Vec2 operator/(const Vec2& other) const;

	Vec2& operator+=(const Vec2& other);
	Vec2& operator-=(const Vec2& other);
	Vec2& operator*=(const Vec2& other);
	Vec2& operator/=(const Vec2& other);

	double dot(const Vec2& other) const;
	Vec2 cross(const Vec2& other) const;
	double len() const;
	Vec2 normalize();
};

struct Vec3 {
	double X, Y, Z;

	Vec3();
	Vec3(double P_X, double P_Y, double P_Z);

	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	Vec3 operator*(const Vec3& other) const;
	Vec3 operator/(const Vec3& other) const;

	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);
	Vec3& operator*=(const Vec3& other);
	Vec3& operator/=(const Vec3& other);

	Vec3 operator*(const double& other) const;
	friend Vec3 operator*(const double& input, const Vec3& vec) { return vec * input; };

	double dot(const Vec3& other) const;
	Vec3 cross(const Vec3& other) const;
	double len() const;
	Vec3 normalized();
	Vec3 rotate(Vec3 P_Pos, const Vec3 P_Anchor, const Vec3 P_Rotation) const;
	void rotate(const Vec3 P_Anchor, const Vec3 P_Rotation);

	Vec3 operator-() const;

	static Vec3 normalize(const Vec3& P_i);
	static double dot(const Vec3& P_a, const Vec3& P_b);
};

struct Vec4 {
	double X, Y, Z, W;

	Vec4();
	Vec4(const Vec3& P_XYZ, double P_W);
	Vec4(double P_X, double P_Y, double P_Z, double P_W);

	Vec4 operator*(const Matrix_4x4& other) const;
};

struct Matrix_4x4 {
	vector<vector<double>> m;

	Matrix_4x4();
	Matrix_4x4(vector<vector<double>> P_Input);

	Matrix_4x4 operator*(Matrix_4x4 other) const;
	Matrix_4x4 operator/(double other) const;
	
	Matrix_4x4 inv() const;
	Matrix_4x4 adjugate() const;
	double determinant() const;
};

struct Rgb {
	double R, G, B;

	Rgb();
	Rgb(double P_Gray);
	Rgb(double P_R, double P_G, double P_B);

	Rgb operator+(Rgb other) const;

	Rgb operator*(double other) const;

	static Rgb random();
};

struct Rgba {
	double R, G, B, A;

	Rgba();
	Rgba(Rgb P_Color, double P_Alpha);
	Rgba(double P_R, double P_G, double P_B, double P_A);

	Rgba& operator*(const double& other) const;

	Rgba clamp();

	static Rgba random();
	static Rgba fromRgb(Rgb P_Rgb);
};

struct Math {
	static double fastInvSqrt(const double& number);
	static vector<string> splitString(const string& input, const string& delimiter);
	static vector<string> splitString(const string& input);
	static string strEnd(const vector<string>& P_Vec, const size_t& P_Start);
	static string strEndSpace(const vector<string>& P_Vec, const size_t& P_Start);
	static string strSpaced(const vector<size_t>& P_Vec);
	static string vecToStringLines(const vector<string>& P_Vec);
	static vector<string> splitStringToLines(const string& P_Lines);
	static string addTabsToStr(const string& input, const int& tabs);
	static double clamp(double P_Value, double P_Min, double P_Max);
};