#include "Math.h"

Vec2::Vec2() {
	X = 0;
	Y = 0;
}

Vec2::Vec2(double P_X, double P_Y) {
	X = P_X;
	Y = P_Y;
}

Vec2 Vec2::operator+(const Vec2& other)  const {
	Vec2 Result = Vec2();
	Result.X = X + other.X;
	Result.Y = Y + other.Y;
	return Result;
}

Vec2 Vec2::operator-(const Vec2& other)  const {
	Vec2 Result = Vec2();
	Result.X = X - other.X;
	Result.Y = Y - other.Y;
	return Result;
}

Vec2 Vec2::operator*(const Vec2& other)  const {
	Vec2 Result = Vec2();
	Result.X = X * other.X;
	Result.Y = Y * other.Y;
	return Result;
}

Vec2 Vec2::operator/(const Vec2& other)  const {
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

double Vec2::dot(const Vec2& other)  const {
	return X * other.X + Y * other.Y;
}

Vec2 Vec2::cross(const Vec2& other)  const {
	return Vec2(
		Y * other.Y - Y * other.Y,
		Y * other.X - X * other.Y
	);
}

double Vec2::len()  const {
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

Vec3 Vec3::operator+(const Vec3& other) const {
	Vec3 Result = Vec3();
	Result.X = X + other.X;
	Result.Y = Y + other.Y;
	Result.Z = Z + other.Z;
	return Result;
}

Vec3 Vec3::operator-(const Vec3& other) const {
	Vec3 Result = Vec3();
	Result.X = X - other.X;
	Result.Y = Y - other.Y;
	Result.Z = Z - other.Z;
	return Result;
}

Vec3 Vec3::operator*(const Vec3& other) const {
	Vec3 Result = Vec3();
	Result.X = X * other.X;
	Result.Y = Y * other.Y;
	Result.Z = Z * other.Z;
	return Result;
}

Vec3 Vec3::operator/(const Vec3& other) const {
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

Vec3 Vec3::operator*(const double& other) const {
	return Vec3(
		X * other,
		Y * other,
		Z * other
	);
}

double Vec3::dot(const Vec3& other)  const {
	return X * other.X + Y * other.Y + Z * other.Z;
}

Vec3 Vec3::cross(const Vec3& other)  const {
	return Vec3(
		Y * other.Z - Z * other.Y,
		Z * other.X - X * other.Z,
		X * other.Y - Y * other.X
	);
}

double Vec3::len()  const {
	return sqrt(X * X + Y * Y + Z * Z);
}

Vec3 Vec3::normalize() {
	double  Length = 1.0 / len();
	X = X * Length;
	Y = Y * Length;
	Z = Z * Length;
	return *this;
}

Vec3 Vec3::rotate(Vec3 P_Pos, const Vec3 P_Anchor, const Vec3 P_Rotation) const {
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

Rgb::Rgb(double P_Gray) {
	R = P_Gray;
	G = P_Gray;
	B = P_Gray;
}

Rgb::Rgb(double P_R, double P_G, double P_B) {
	R = P_R;
	G = P_G;
	B = P_B;
}

Rgb Rgb::operator+(Rgb other) const {
	Rgb Result = Rgb();
	Result.R = R + other.R;
	Result.G = G + other.G;
	Result.B = B + other.B;
	return Result;
}

Rgb Rgb::operator*(double other) const {
	Rgb Result = Rgb();
	Result.R = R * other;
	Result.G = G * other;
	Result.B = B * other;
	return Result;
}

Rgb Rgb::random() {
	return Rgb(rand() / static_cast<double>(RAND_MAX), rand() / static_cast<double>(RAND_MAX), rand() / static_cast<double>(RAND_MAX));
}

Rgba::Rgba() {
	R = 1;
	G = 1;
	B = 1;
	A = 1;
}

Rgba::Rgba(Rgb P_Color, double P_Alpha) {
	R = P_Color.R;
	G = P_Color.G;
	B = P_Color.B;
	A = P_Alpha;
}

Rgba::Rgba(double P_R, double P_G, double P_B, double P_A) {
	R = P_R;
	G = P_G;
	B = P_B;
	A = P_A;
}

Rgba& Rgba::operator*(const double& other) const {
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
	return Rgba(rand() / static_cast<double>(RAND_MAX), rand() / static_cast<double>(RAND_MAX), rand() / static_cast<double>(RAND_MAX), 1);
}

Rgba Rgba::fromRgb(Rgb P_Rgb) {
	return Rgba(P_Rgb.R, P_Rgb.G, P_Rgb.B, 1);
}

Matrix_4x4::Matrix_4x4() {
	m = vector(4, vector(4, 0.0));
}

Matrix_4x4::Matrix_4x4(vector<vector<double>> P_Input) {
	m = P_Input;
}

Matrix_4x4 Matrix_4x4::operator*(Matrix_4x4 other) const {
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

Matrix_4x4 Matrix_4x4::operator/(double other) const {
	Matrix_4x4 Result = Matrix_4x4();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			Result.m[i][j] = m[i][j] / other;
		}
	}
	return Result;
}

Matrix_4x4 Matrix_4x4::inv() const {
	return adjugate() / determinant();
}

Matrix_4x4 Matrix_4x4::adjugate() const {
	return Matrix_4x4({
		{ m[1][1] * m[2][2] * m[3][3] + m[3][1] * m[1][2] * m[2][3] + m[2][1] * m[3][2] * m[1][3] - m[1][1] * m[3][2] * m[2][3] - m[2][1] * m[1][2] * m[3][3] - m[3][1] * m[2][2] * m[1][3],
		  m[0][1] * m[3][2] * m[2][3] + m[2][1] * m[0][2] * m[3][3] + m[3][1] * m[2][2] * m[0][3] - m[3][1] * m[0][2] * m[2][3] - m[2][1] * m[3][2] * m[0][3] - m[0][1] * m[2][2] * m[3][3],
		  m[0][1] * m[1][2] * m[3][3] + m[3][1] * m[0][2] * m[1][3] + m[1][1] * m[3][2] * m[0][3] - m[0][1] * m[3][2] * m[1][3] - m[1][1] * m[0][2] * m[3][3] - m[3][1] * m[1][2] * m[0][3],
		  m[0][1] * m[2][2] * m[1][3] + m[1][1] * m[0][2] * m[2][3] + m[2][1] * m[1][2] * m[0][3] - m[0][1] * m[1][2] * m[2][3] - m[2][1] * m[0][2] * m[1][3] - m[1][1] * m[2][2] * m[0][3]},
		{ m[1][2] * m[3][3] * m[2][0] + m[2][2] * m[1][3] * m[3][0] + m[3][2] * m[2][3] * m[1][0] - m[1][2] * m[2][3] * m[3][0] - m[3][2] * m[1][3] * m[2][0] - m[2][2] * m[3][3] * m[1][0],
		  m[0][2] * m[2][3] * m[3][0] + m[3][2] * m[0][3] * m[2][0] + m[2][2] * m[3][3] * m[0][0] - m[0][2] * m[3][3] * m[2][0] - m[2][2] * m[0][3] * m[3][0] - m[3][2] * m[2][3] * m[0][0],
		  m[0][2] * m[3][3] * m[1][0] + m[1][2] * m[0][3] * m[3][0] + m[3][2] * m[1][3] * m[0][0] - m[0][2] * m[1][3] * m[3][0] - m[3][2] * m[0][3] * m[1][0] - m[1][2] * m[3][3] * m[0][0],
		  m[0][2] * m[1][3] * m[2][0] + m[2][2] * m[0][3] * m[1][0] + m[1][2] * m[2][3] * m[0][0] - m[0][2] * m[2][3] * m[1][0] - m[1][2] * m[0][3] * m[2][0] - m[2][2] * m[1][3] * m[0][0]},
		{ m[1][3] * m[2][0] * m[3][1] + m[3][3] * m[1][0] * m[2][1] + m[2][3] * m[3][0] * m[1][1] - m[1][3] * m[3][0] * m[2][1] - m[2][3] * m[1][0] * m[3][1] - m[3][3] * m[2][0] * m[1][1],
		  m[0][3] * m[3][0] * m[2][1] + m[2][3] * m[0][0] * m[3][1] + m[3][3] * m[2][0] * m[0][1] - m[0][3] * m[2][0] * m[3][1] - m[3][3] * m[0][0] * m[2][1] - m[2][3] * m[3][0] * m[0][1],
		  m[0][3] * m[1][0] * m[3][1] + m[3][3] * m[0][0] * m[1][1] + m[1][3] * m[3][0] * m[0][1] - m[0][3] * m[3][0] * m[1][1] - m[1][3] * m[0][0] * m[3][1] - m[3][3] * m[1][0] * m[0][1],
		  m[0][3] * m[2][0] * m[1][1] + m[1][3] * m[0][0] * m[2][1] + m[2][3] * m[1][0] * m[0][1] - m[0][3] * m[1][0] * m[2][1] - m[2][3] * m[0][0] * m[1][1] - m[1][3] * m[2][0] * m[0][1]},
		{ m[1][0] * m[3][1] * m[2][2] + m[2][0] * m[1][1] * m[3][2] + m[3][0] * m[2][1] * m[1][2] - m[1][0] * m[2][1] * m[3][2] - m[3][0] * m[1][1] * m[2][2] - m[2][0] * m[3][1] * m[1][2],
		  m[0][0] * m[2][1] * m[3][2] + m[3][0] * m[0][1] * m[2][2] + m[2][0] * m[3][1] * m[0][2] - m[0][0] * m[3][1] * m[2][2] - m[2][0] * m[0][1] * m[3][2] - m[3][0] * m[2][1] * m[0][2],
		  m[0][0] * m[3][1] * m[1][2] + m[1][0] * m[0][1] * m[3][2] + m[3][0] * m[1][1] * m[0][2] - m[0][0] * m[1][1] * m[3][2] - m[3][0] * m[0][1] * m[1][2] - m[1][0] * m[3][1] * m[0][2],
		  m[0][0] * m[1][1] * m[2][2] + m[2][0] * m[0][1] * m[1][2] + m[1][0] * m[2][1] * m[0][2] - m[0][0] * m[2][1] * m[1][2] - m[1][0] * m[0][1] * m[2][2] - m[2][0] * m[1][1] * m[0][2]}
		}
	);
}

double Matrix_4x4::determinant() const {
	return m[0][0] * (m[1][1] * m[2][2] * m[3][3] + m[3][1] * m[1][2] * m[2][3] + m[2][1] * m[3][2] * m[1][3] - m[1][1] * m[3][2] * m[2][3] - m[2][1] * m[1][2] * m[3][3] - m[3][1] * m[2][2] * m[1][3])
		+  m[0][1] * (m[1][2] * m[3][3] * m[2][0] + m[2][2] * m[1][3] * m[3][0] + m[3][2] * m[2][3] * m[1][0] - m[1][2] * m[2][3] * m[3][0] - m[3][2] * m[1][3] * m[2][0] - m[2][2] * m[3][3] * m[1][0])
		+  m[0][2] * (m[1][3] * m[2][0] * m[3][1] + m[3][3] * m[1][0] * m[2][1] + m[2][3] * m[3][0] * m[1][1] - m[1][3] * m[3][0] * m[2][1] - m[2][3] * m[1][0] * m[3][1] - m[3][3] * m[2][0] * m[1][1])
		+  m[0][3] * (m[1][0] * m[3][1] * m[2][2] + m[2][0] * m[1][1] * m[3][2] + m[3][0] * m[2][1] * m[1][2] - m[1][0] * m[2][1] * m[3][2] - m[3][0] * m[1][1] * m[2][2] - m[2][0] * m[3][1] * m[1][2]);
}

Vec4 Vec4::operator*(const Matrix_4x4& other) const {
	return Vec4(
		other.m[0][0] * X + other.m[0][1] * Y + other.m[0][2] * Z + other.m[0][3] * W,
		other.m[1][0] * X + other.m[1][1] * Y + other.m[1][2] * Z + other.m[1][3] * W,
		other.m[2][0] * X + other.m[2][1] * Y + other.m[2][2] * Z + other.m[2][3] * W,
		other.m[3][0] * X + other.m[3][1] * Y + other.m[3][2] * Z + other.m[3][3] * W
	);
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

double Math::fastInvSqrt(const double& number) {
	long i;
	double x2, y;
	const double threehalfs = 1.5F;
	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;							// get bits for doubleing value
	i = 0x5f3759df - (i >> 1);				// gives initial guess y0
	y = *(double*)&i;						// convert bits back to double
	y = y * (threehalfs - (x2 * y * y));	// Newton step, repeating increases accuracy
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y));
	return y;
}

vector<string> Math::splitString(const string& input, const string& delimiter) {
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
vector<string> Math::splitString(const string& input) {
	vector<string> result;
	istringstream iss(input);
	string token;
	while (iss >> token) {
		result.push_back(token);
	}
	return result;
}
string Math::strEnd(const vector<string>& P_Vec, const size_t& P_Start) {
	return accumulate(P_Vec.begin() + P_Start, P_Vec.end(), string());
}
string Math::strEndSpace(const vector<string>& P_Vec, const size_t& P_Start) {
	return accumulate(
		P_Vec.begin() + P_Start, P_Vec.end(), string(),
		[](const string& accumulator, const string& current) {
			return accumulator.empty() ? current : accumulator + " " + current;
		}
	);
}
string Math::strSpaced(const vector<size_t>& P_Vec) {
	std::ostringstream oss;
	for (size_t i = 0; i < P_Vec.size(); ++i) {
		oss << P_Vec[i];
		if (i < P_Vec.size() - 1) {
			oss << " ";
		}
	}
	return oss.str();
}

string Math::vecToStringLines(const vector<string>& P_Vec) {
	if (P_Vec.empty())
		return "";
	string result = P_Vec[0];
	for (size_t i = 1; i < P_Vec.size(); ++i) {
		result += "\n";
		result += P_Vec[i];
	}
	return result;
}
vector<string> Math::splitStringToLines(const string& P_Lines) {
	vector<string> lines;
	istringstream iss(P_Lines);
	string line;
	while (getline(iss, line)) {
		lines.push_back(line);
	}
	return lines;
}
string Math::addTabsToStr(const string& input, const int& tabs) {
	istringstream iss(input);
	ostringstream oss;
	string line;
	while (getline(iss, line)) {
		int Tab = tabs;
		while (Tab--) {
			oss << "\t";
		}
		oss << line << "\n";
	}
	return oss.str().substr(0, oss.str().length() - 1);
}

double Math::clamp(double P_Value, double P_Min, double P_Max) {
	if (P_Value >= P_Min && P_Value <= P_Max) return P_Value;
	else if (P_Value < P_Min) return P_Min;
	else return P_Max;
}