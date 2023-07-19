#pragma once

#include "R_String.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <variant>
#include <cstdlib>
#include <vector>
#include <thread>
#include <future>
#include <map>

#define PI          3.1415926535897932384626433832795
#define TWO_PI      6.2831853071795864769252867665590
#define INVERTED_PI 0.3183098861837906715377675267450

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

	double dot(const Vec3& other);
	Vec3 cross(const Vec3& other);
	double len();
	Vec3 normalize();
	Vec3 rotate(Vec3 P_Pos, const Vec3 P_Anchor, const Vec3 P_Rotation);
	void rotate(const Vec3 P_Anchor, const Vec3 P_Rotation);
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

struct Math {
	static float fastInvSqrt(float number);
	static std::vector<std::string> splitString(std::string& input, std::string delimiter);
	static std::vector<std::string> splitString(std::string& input);
};