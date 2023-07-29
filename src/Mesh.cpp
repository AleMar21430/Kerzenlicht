#include "Mesh.h"

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

Vec2 Vertex::project(const Vec3& cameraPos, const Vec3& cameraDir, double FOV) {
	Vec3 relative = Vec3(Pos.X - cameraPos.X, Pos.Y - cameraPos.Y, Pos.Z - cameraPos.Z);

	double cosYaw = cos(cameraDir.Y);
	double sinYaw = sin(cameraDir.Y);
	double cosPitch = cos(cameraDir.X);
	double sinPitch = sin(cameraDir.X);

	double x = relative.X * cosYaw - relative.Z * sinYaw;
	double z = relative.X * sinYaw + relative.Z * cosYaw;
	double y = relative.Y * cosPitch - z * sinPitch;

	double f = FOV / (y + FOV);

	return Vec2(f * x, f * z);
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