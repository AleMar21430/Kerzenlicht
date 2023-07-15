#include "R_Vec_3.h"

R_Vec_3::R_Vec_3(long double P_X, long double P_Y, long double P_Z) {
	X = P_X, Y = P_Y, Z = P_Z;
}

void R_Vec_3::read(std::string Input) {
}

std::string R_Vec_3::write(uint8_t Tab) {
	R_String Result;
	Result << "Vec_3 << [ " << X << " | " << Y << " | " << Z << " ]";
	return Result.write();
}

uint64_t R_Vec_3::size() {
	return 0;
}

R_Vec_3 operator+(const R_Vec_3& left, const R_Vec_3& right) {
	R_Vec_3 Result = R_Vec_3();
	Result.X = left.X + right.X;
	Result.Y = left.Y + right.Y;
	Result.Z = left.Z + right.Z;
	return Result;
}

R_Vec_3 operator-(const R_Vec_3& left, const R_Vec_3& right) {
	R_Vec_3 Result = R_Vec_3();
	Result.X = left.X - right.X;
	Result.Y = left.Y - right.Y;
	Result.Z = left.Z - right.Z;
	return Result;
}

R_Vec_3 operator*(const R_Vec_3& left, const R_Vec_3& right) {
	R_Vec_3 Result = R_Vec_3();
	Result.X = left.X * right.X;
	Result.Y = left.Y * right.Y;
	Result.Z = left.Z * right.Z;
	return Result;
}

R_Vec_3 operator/(const R_Vec_3& left, const R_Vec_3& right) {
	R_Vec_3 Result = R_Vec_3();
	Result.X = left.X / right.X;
	Result.Y = left.Y / right.Y;
	Result.Z = left.Z / right.Z;
	return Result;
}

long double R_Vec_3::dot(const R_Vec_3& left, const R_Vec_3& right) {
	return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
}

R_Vec_3 R_Vec_3::cross(const R_Vec_3& left, const R_Vec_3& right) {
	R_Vec_3 Result = R_Vec_3();
	Result.X = left.Y * right.Z - left.Z * right.Y;
	Result.Y = left.Z / right.X - left.X * right.Z;
	Result.Z = left.X / right.Y - left.Y * right.X;
	return Result;
}

long double R_Vec_3::len() {
	return sqrt(X * X + Y * Y + Z * Z);
}

R_Vec_3 R_Vec_3::normalize() {
	R_Vec_3 Result = R_Vec_3();
	long double Length = 1L / this->len();
	X = X * Length;
	Y = Y * Length;
	Z = Z * Length;
	return *this;
}