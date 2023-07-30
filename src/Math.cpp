#include "Math.h"

Vec2::Vec2() {
	X = 0;
	Y = 0;
}

Vec2::Vec2(double P_X, double P_Y) {
	X = P_X;
	Y = P_Y;
}

Vec2 Vec2::operator+(const Vec2& other) {
	Vec2 Result = Vec2();
	Result.X = X + other.X;
	Result.Y = Y + other.Y;
	return Result;
}

Vec2 Vec2::operator-(const Vec2& other) {
	Vec2 Result = Vec2();
	Result.X = X - other.X;
	Result.Y = Y - other.Y;
	return Result;
}

Vec2 Vec2::operator*(const Vec2& other) {
	Vec2 Result = Vec2();
	Result.X = X * other.X;
	Result.Y = Y * other.Y;
	return Result;
}

Vec2 Vec2::operator/(const Vec2& other) {
	Vec2 Result = Vec2();
	Result.X = X / other.X;
	Result.Y = Y / other.Y;
	return Result;
}

Vec2& Vec2::operator+=(const Vec2& other) {
	X = X + other.X;
	Y = Y + other.Y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
	X = X - other.X;
	Y = Y - other.Y;
	return *this;
}

Vec2& Vec2::operator*=(const Vec2& other) {
	X = X * other.X;
	Y = Y * other.Y;
	return *this;
}

Vec2& Vec2::operator/=(const Vec2& other) {
	X = X / other.X;
	Y = Y / other.Y;
	return *this;
}

double Vec2::dot(const Vec2& other) {
	return X * other.X + Y * other.Y;
}

Vec2 Vec2::cross(const Vec2& other) {
	Vec2 Result = Vec2();
	Result.X = Y * other.Y - Y * other.Y;
	Result.Y = Y * other.X - X * other.Y;
	return Result;
}

double Vec2::len() {
	return sqrt(X * X + Y * Y);
}

Vec2 Vec2::normalize() {
	Vec2 Result = Vec2();
	double Length = 1L / this->len();
	X = X * Length;
	Y = Y * Length;
	return *this;
}

Vec3::Vec3() {
	X = 0;
	Y = 0;
	Z = 0;
}

Vec3::Vec3(double P_X, double P_Y, double P_Z) {
	X = P_X;
	Y = P_Y;
	Z = P_Z;
}

Vec3 Vec3::operator+(const Vec3& other) {
	Vec3 Result = Vec3();
	Result.X = X + other.X;
	Result.Y = Y + other.Y;
	Result.Z = Z + other.Z;
	return Result;
}

Vec3 Vec3::operator-(const Vec3& other) {
	Vec3 Result = Vec3();
	Result.X = X - other.X;
	Result.Y = Y - other.Y;
	Result.Z = Z - other.Z;
	return Result;
}

Vec3 Vec3::operator*(const Vec3& other) {
	Vec3 Result = Vec3();
	Result.X = X * other.X;
	Result.Y = Y * other.Y;
	Result.Z = Z * other.Z;
	return Result;
}

Vec3 Vec3::operator/(const Vec3& other) {
	Vec3 Result = Vec3();
	Result.X = X / other.X;
	Result.Y = Y / other.Y;
	Result.Z = Z / other.Z;
	return Result;
}

Vec3& Vec3::operator+=(const Vec3& other) {
	X = X + other.X;
	Y = Y + other.Y;
	Z = Z + other.Z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& other) {
	X = X - other.X;
	Y = Y - other.Y;
	Z = Z - other.Z;
	return *this;
}

Vec3& Vec3::operator*=(const Vec3& other) {
	X = X * other.X;
	Y = Y * other.Y;
	Z = Z * other.Z;
	return *this;
}

Vec3& Vec3::operator/=(const Vec3& other) {
	X = X / other.X;
	Y = Y / other.Y;
	Z = Z / other.Z;
	return *this;
}

Vec3& Vec3::operator*(const double& other) {
	Vec3 Result = Vec3();
	Result.X = X * other;
	Result.Y = Y * other;
	Result.Z = Z * other;
	return Result;
}

double Vec3::dot(const Vec3& other) {
	return X * other.X + Y * other.Y + Z * other.Z;
}

Vec3 Vec3::cross(const Vec3& other) {
	Vec3 Result = Vec3();
	Result.X = Y * other.Z - Z * other.Y;
	Result.Y = Z * other.X - X * other.Z;
	Result.Z = X * other.Y - Y * other.X;
	return Result;
}

double Vec3::len() {
	return sqrt(X * X + Y * Y + Z * Z);
}

Vec3 Vec3::normalize() {
	double Length = 1L / this->len();
	X = X * Length;
	Y = Y * Length;
	Z = Z * Length;
	return *this;
}

Vec3 Vec3::rotate(Vec3 P_Pos, const Vec3 P_Anchor, const Vec3 P_Rotation) {
	Vec3 Result = Vec3(P_Pos);

	Result.X -= P_Anchor.X;
	Result.Y -= P_Anchor.Y;
	Result.Z -= P_Anchor.Z;

	double cosX = cos(P_Rotation.X);
	double sinX = sin(P_Rotation.X);
	double newY = Result.Y * cosX - Result.Z * sinX;
	double newZ = Result.Y * sinX + Result.Z * cosX;

	double cosY = cos(P_Rotation.Y);
	double sinY = sin(P_Rotation.Y);
	double newX = Result.X * cosY + newZ * sinY;
	newZ = -Result.X * sinY + newZ * cosY;

	double cosZ = cos(P_Rotation.Z);
	double sinZ = sin(P_Rotation.Z);
	double finalX = newX * cosZ - newY * sinZ;
	double finalY = newX * sinZ + newY * cosZ;

	Result.X = finalX + P_Anchor.X;
	Result.Y = finalY + P_Anchor.Y;
	Result.Z = newZ + P_Anchor.Z;
	return Result;
}

void Vec3::rotate(const Vec3 P_Anchor, const Vec3 P_Rotation) {
	X -= P_Anchor.X;
	Y -= P_Anchor.Y;
	Z -= P_Anchor.Z;

	double cosX = cos(P_Rotation.X);
	double sinX = sin(P_Rotation.X);
	double newY = Y * cosX - Z * sinX;
	double newZ = Y * sinX + Z * cosX;

	double cosY = cos(P_Rotation.Y);
	double sinY = sin(P_Rotation.Y);
	double newX = X * cosY + newZ * sinY;
	newZ = -X * sinY + newZ * cosY;

	double cosZ = cos(P_Rotation.Z);
	double sinZ = sin(P_Rotation.Z);
	double finalX = newX * cosZ - newY * sinZ;
	double finalY = newX * sinZ + newY * cosZ;

	X = finalX + P_Anchor.X;
	Y = finalY + P_Anchor.Y;
	Z = newZ + P_Anchor.Z;
}

Rgb::Rgb() {
	R = 1;
	G = 1;
	B = 1;
}

Rgb::Rgb(float P_R, float P_G, float P_B) {
	R = P_R;
	G = P_G;
	B = P_B;
}

Rgb Rgb::operator+(Rgb other) {
	Rgb Result = Rgb();
	Result.R = R + other.R;
	Result.G = G + other.G;
	Result.B = B + other.B;
	return Result;
}

Rgb Rgb::operator*(float other) {
	Rgb Result = Rgb();
	Result.R = R * other;
	Result.G = G * other;
	Result.B = B * other;
	return Result;
}

Rgb Rgb::random() {
	return Rgb(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX));
}

Rgba::Rgba() {
	R = 1;
	G = 1;
	B = 1;
	A = 1;
}

Rgba::Rgba(Rgb P_Color, float P_Alpha) {
	R = P_Color.R;
	G = P_Color.G;
	B = P_Color.B;
	A = P_Alpha;
}

Rgba::Rgba(float P_R, float P_G, float P_B, float P_A) {
	R = P_R;
	G = P_G;
	B = P_B;
	A = P_A;
}

Rgba& Rgba::operator*(const float& other) {
	Rgba Result = Rgba();
	Result.R = R * other;
	Result.G = G * other;
	Result.B = B * other;
	Result.A = A * other;
	return Result;
}

Rgba Rgba::clamp() {
	R = R < 0 ? 0.0 : R > 1.0 ? 1.0 : R;
	G = G < 0 ? 0.0 : G > 1.0 ? 1.0 : G;
	B = B < 0 ? 0.0 : B > 1.0 ? 1.0 : B;
	A = A < 0 ? 0.0 : A > 1.0 ? 1.0 : A;
	return *this;
}

Rgba Rgba::random() {
	return Rgba(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), 1);
}

Rgba Rgba::fromRgb(Rgb P_Rgb) {
	return Rgba(P_Rgb.R, P_Rgb.G, P_Rgb.B, 1);
}

float Math::fastInvSqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;							// get bits for floating value
	i = 0x5f3759df - (i >> 1);				// gives initial guess y0
	y = *(float*)&i;						// convert bits back to float
	y = y * (threehalfs - (x2 * y * y));	// Newton step, repeating increases accuracy
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y));
	return y;
}

vector<string> Math::splitString(string& input, string delimiter) {
	vector<string> tokens;
	string::size_type start = 0;
	string::size_type end = input.find(delimiter);

	while (end != string::npos) {
		tokens.push_back(input.substr(start, end - start));
		start = end + 1;
		end = input.find(delimiter, start);
	}
	tokens.push_back(input.substr(start));

	return tokens;
}

vector<string> Math::splitString(string& input) {
	vector<string> result;
	istringstream iss(input);
	string token;

	while (iss >> token) {
		result.push_back(token);
	}

	return result;
}

void Math::centerPointsToOrigin(vector<pair<int, int>>& points) {
	// Find the minimum and maximum x and y coordinates
	int minX = points[0].first;
	int maxX = points[0].first;
	int minY = points[0].second;
	int maxY = points[0].second;

	for (const auto& point : points) {
		minX = min(minX, point.first);
		maxX = max(maxX, point.first);
		minY = min(minY, point.second);
		maxY = max(maxY, point.second);
	}

	// Calculate the center of the bounding box
	int centerX = (minX + maxX) / 2;
	int centerY = (minY + maxY) / 2;

	// Shift all points to center them around the origin
	for (auto& point : points) {
		point.first -= centerX;
		point.second -= centerY;
	}
}

Matrix_3x3 Matrix_3x3::operator*(Matrix_3x3 other) {
	Matrix_3x3 Result = Matrix_3x3();
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			Result.matrix[i][j] =
			matrix[i][0] * other.matrix[0][j] +
			matrix[i][1] * other.matrix[1][j] +
			matrix[i][2] * other.matrix[2][j] ;
		}
	}
	return Result;
}

Matrix_4x4::Matrix_4x4() {
	m = vector(4, vector(4, 0.0));
}

Matrix_4x4::Matrix_4x4(vector<vector<double>> P_Input) {
	m = P_Input;
}

Matrix_4x4 Matrix_4x4::operator*(Matrix_4x4 other) {
	Matrix_4x4 Result = Matrix_4x4();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				Result.m[i][j] += m[i][k] * other.m[k][j];
			}
		}
	}
	return Result;
}

Vec4& Vec4::operator*(const Matrix_4x4& other) {
	Vec4 Result = Vec4();
	Result.X = other.m[0][0] * X + other.m[0][1] * Y + other.m[0][2] * Z + other.m[0][3] * W;
	Result.Y = other.m[1][0] * X + other.m[1][1] * Y + other.m[1][2] * Z + other.m[1][3] * W;
	Result.Z = other.m[2][0] * X + other.m[2][1] * Y + other.m[2][2] * Z + other.m[2][3] * W;
	Result.W = other.m[3][0] * X + other.m[3][1] * Y + other.m[3][2] * Z + other.m[3][3] * W;
	return Result;
}

Vec4::Vec4() {
	X = 0;
	Y = 0;
	Z = 0;
	W = 0;
}

Vec4::Vec4(const Vec3& P_XYZ, double P_W) {
	X = P_XYZ.X;
	Y = P_XYZ.Y;
	Z = P_XYZ.Z;
	W = P_W;
}

Vec4::Vec4(double P_X, double P_Y, double P_Z, double P_W) {
	X = P_X;
	Y = P_Y;
	Z = P_Z;
	W = P_W;
}