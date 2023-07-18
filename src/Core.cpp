#include "Core.h"

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

Rgb Rgb::random() {
	return Rgb(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX));
}

Rgba::Rgba() {
	R = 1;
	G = 1;
	B = 1;
	A = 1;
}

Rgba::Rgba(float P_R, float P_G, float P_B, float P_A) {
	R = P_R;
	G = P_G;
	B = P_B;
	A = P_A;
}

Rgba Rgba::random() {
	return Rgba(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), 1);
}

Rgba Rgba::fromRgb(Rgb P_Rgb) {
	return Rgba(P_Rgb.R, P_Rgb.G, P_Rgb.B, 1);
}

Vec2::Vec2() {
	X = 0;
	Y = 0;
}

Vec2::Vec2(double P_X, double P_Y) {
	X = P_X;
	Y = P_Y;
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

	double cosX = std::cos(P_Rotation.X);
	double sinX = std::sin(P_Rotation.X);
	double newY = Result.Y * cosX - Result.Z * sinX;
	double newZ = Result.Y * sinX + Result.Z * cosX;

	double cosY = std::cos(P_Rotation.Y);
	double sinY = std::sin(P_Rotation.Y);
	double newX = Result.X * cosY + newZ * sinY;
	newZ = -Result.X * sinY + newZ * cosY;

	double cosZ = std::cos(P_Rotation.Z);
	double sinZ = std::sin(P_Rotation.Z);
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

	double cosX = std::cos(P_Rotation.X);
	double sinX = std::sin(P_Rotation.X);
	double newY = Y * cosX - Z * sinX;
	double newZ = Y * sinX + Z * cosX;

	double cosY = std::cos(P_Rotation.Y);
	double sinY = std::sin(P_Rotation.Y);
	double newX = X * cosY + newZ * sinY;
	newZ = -X * sinY + newZ * cosY;

	double cosZ = std::cos(P_Rotation.Z);
	double sinZ = std::sin(P_Rotation.Z);
	double finalX = newX * cosZ - newY * sinZ;
	double finalY = newX * sinZ + newY * cosZ;

	X = finalX + P_Anchor.X;
	Y = finalY + P_Anchor.Y;
	Z = newZ + P_Anchor.Z;
}

Vertex::Vertex() {
	Pos = Vec3();
	Color = Rgb();
}

Vertex::Vertex(Vec3 P_Pos) {
	Pos = P_Pos;
	Color = Rgb();
}

Vertex::Vertex(Vec3 P_Pos, Rgb P_Color) {
	Pos = P_Pos;
	Color = P_Color;
}

Mesh_Face::Mesh_Face(uint32_t P_I1, uint32_t P_I2, uint32_t P_I3) {
	I1 = P_I1;
	I2 = P_I2;
	I3 = P_I3;
}

Mesh::Mesh() {
	Faces = std::vector<Mesh_Face>();
	Vertex_Positions = std::vector<Vec3>();
	Vertex_Colors = std::map<std::string, std::vector<Rgb>>();
	Vertex_Weights = std::map<std::string, std::map<std::string, double>>();

	Vertex_Output = std::vector<Vertex>();
}

Object::Object() {
	Name = "Default";
	Type = Object_Type::EMPTY;

	Pos = Vec3();
	Anchor = Vec3();
	Rot_Euler = Vec3();
	Scale = Vec3(1, 1, 1);

	if (Type == MESH) {
		MeshData = Mesh();
	}
}

Object::Object(std::string P_Name, Object_Type P_Type) {
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

void Object::loadBuffers() {
	if (Type == MESH) {
		MeshData.Vertex_Output = std::vector<Vertex>();
		for (int i = 0; i < MeshData.Vertex_Positions.size(); i++) {
			Vertex vert = Vertex(MeshData.Vertex_Positions[i]);
			MeshData.Vertex_Output.push_back(vert);
		}
	}
}

void Object::scale(Vec3 P_Scale) {
	Scale = Scale + P_Scale;
}

void Object::setScale(Vec3 P_Scale) {
	Scale = P_Scale;
}

void Object::rotate(Vec3 P_Rot) {
	Rot_Euler = Rot_Euler + P_Rot;
}

void Object::translate(Vec3 P_Pos) {
	Pos = Pos + P_Pos;
}

void Object::processTransform() {
	if (Type == MESH) {
		for (int i = 0; i < MeshData.Vertex_Positions.size(); i++) {
			Vertex& vert = MeshData.Vertex_Output[i];
			vert.Pos = MeshData.Vertex_Positions[i] * Scale;
			vert.Pos.rotate(Anchor, Rot_Euler);
			vert.Pos = vert.Pos + Pos;
		}
	}
}