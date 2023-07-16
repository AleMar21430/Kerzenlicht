#pragma once

#include "R_String.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define PI          3.1415926535897932384626433832795
#define TWO_PI      6.2831853071795864769252867665590
#define INVERTED_PI 0.3183098861837906715377675267450

float fastInvSqrt(float number);

std::vector<std::string> splitString(const std::string& input, std::string delimiter);

struct R_Vec_2 {
	long double X, Y;

	R_Vec_2(long double P_X = 0L, long double P_Y = 0L) {
		X = P_X, Y = P_Y;
	}

	void read(std::string Input) {

	}

	std::string write(uint8_t Tab = 0) {
		R_String Result;
		Result << "Vec 2 << [ " << X << " | " << Y << " ]";
		return Result.write();
	}

	uint64_t size() {
		return 0;
	}

	R_Vec_2 operator+(const R_Vec_2& other) {
		R_Vec_2 Result = R_Vec_2();
		Result.X = X + other.X;
		Result.Y = Y + other.Y;
		return Result;
	}

	R_Vec_2 operator-(const R_Vec_2& other) {
		R_Vec_2 Result = R_Vec_2();
		Result.X = X - other.X;
		Result.Y = Y - other.Y;
		return Result;
	}

	R_Vec_2 operator*(const R_Vec_2& other) {
		R_Vec_2 Result = R_Vec_2();
		Result.X = X * other.X;
		Result.Y = Y * other.Y;
		return Result;
	}

	R_Vec_2 operator/(const R_Vec_2& other) {
		R_Vec_2 Result = R_Vec_2();
		Result.X = X / other.X;
		Result.Y = Y / other.Y;
		return Result;
	}

	long double dot(const R_Vec_2& other) {
		return X * other.X + Y * other.Y;
	}

	R_Vec_2 cross(const R_Vec_2& other) {
		R_Vec_2 Result = R_Vec_2();
		Result.X = Y * other.X - X * other.Y;
		Result.Y = X * other.Y - Y * other.X;
		return Result;
	}

	long double len() {
		return sqrt(X * X + Y * Y);
	}

	R_Vec_2 normalize() {
		R_Vec_2 Result = R_Vec_2();
		long double Length = 1L / this->len();
		X = X * Length;
		Y = Y * Length;
		return *this;
	}
};

struct R_Vec_3 {
	long double X, Y, Z;

	R_Vec_3(long double P_X = 0L, long double P_Y = 0L, long double P_Z = 0L) {
		X = P_X, Y = P_Y, Z = P_Z;
	}

	void read(std::string Input) {

	}

	std::string write(uint8_t Tab = 0) {
		R_String Result;
		Result << "Vec 3 << [ " << X << " | " << Y << " | " << Z << " ]";
		return Result.write();
	}

	uint64_t size() {
		return 0;
	}

	R_Vec_3 operator+(const R_Vec_3& other) {
		R_Vec_3 Result = R_Vec_3();
		Result.X = X + other.X;
		Result.Y = Y + other.Y;
		Result.Z = Z + other.Z;
		return Result;
	}

	R_Vec_3 operator-(const R_Vec_3& other) {
		R_Vec_3 Result = R_Vec_3();
		Result.X = X - other.X;
		Result.Y = Y - other.Y;
		Result.Z = Z - other.Z;
		return Result;
	}

	R_Vec_3 operator*(const R_Vec_3& other) {
		R_Vec_3 Result = R_Vec_3();
		Result.X = X * other.X;
		Result.Y = Y * other.Y;
		Result.Z = Z * other.Z;
		return Result;
	}

	R_Vec_3 operator/(const R_Vec_3 & other) {
		R_Vec_3 Result = R_Vec_3();
		Result.X = X / other.X;
		Result.Y = Y / other.Y;
		Result.Z = Z / other.Z;
		return Result;
	}

	R_Vec_3 operator*(const long double& other) {
		R_Vec_3 Result = R_Vec_3();
		Result.X = X / other;
		Result.Y = Y / other;
		Result.Z = Z / other;
		return Result;
	}

	long double dot(const R_Vec_3& other) {
		return X * other.X + Y * other.Y + Z * other.Z;
	}

	R_Vec_3 cross(const R_Vec_3& other) {
		R_Vec_3 Result = R_Vec_3();
		Result.X = Y * other.Z - Z * other.Y;
		Result.Y = Z * other.X - X * other.Z;
		Result.Z = X * other.Y - Y * other.X;
		return Result;
	}

	long double len() {
		return sqrt(X * X + Y * Y + Z * Z);
	}

	R_Vec_3 normalize() {
		R_Vec_3 Result = R_Vec_3();
		long double Length = 1L / this->len();
		X = X * Length;
		Y = Y * Length;
		Z = Z * Length;
		return *this;
	}
};

struct R_Light_Ray {
	R_Vec_3 Origin;
	R_Vec_3 Direction;

	R_Light_Ray() {
		Origin = R_Vec_3();
		Direction = R_Vec_3();
	}

	R_Vec_3 hitpoint(long double t) {
		return Origin + Direction * t;
	}
};