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

Vertex Vertex::vertShader(const Matrix_4x4& P_Model_Matrix) {
	Pos = Pos * P_Model_Matrix;
	return *this;
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

void Mesh::applyTransformMatrix(const Vec3& P_Translate, const Vec3& P_Rotate, const Vec3& P_Scale) {
	Vertex_Output = std::vector<Vertex>();

	Matrix_4x4 translation = Matrix_4x4({
		{ 1, 0, 0, P_Translate.X },
		{ 0, 1, 0, P_Translate.Y },
		{ 0, 0, 1, P_Translate.Z },
		{ 0, 0, 0, 1             }
	});

	double Yaw   = P_Rotate.X + PI / 180;
	double Pitch = P_Rotate.Y + PI / 180;
	double Roll  = P_Rotate.Z + PI / 180;

	Matrix_4x4 pitchMat = Matrix_4x4({
		{ 1, 0         , 0          , 0 },
		{ 0, cos(Pitch), -sin(Pitch), 0 },
		{ 0, sin(Pitch), cos(Pitch) , 0 },
		{ 0, 0         , 0          , 1 }
	});

	Matrix_4x4 yawMat = Matrix_4x4({
		{ cos(Yaw) , 0, sin(Yaw), 0 },
		{ 0        , 1, 0       , 0 },
		{ -sin(Yaw), 0, cos(Yaw), 0 },
		{ 0        , 0, 0       , 1 }
	});

	Matrix_4x4 rollMat = Matrix_4x4({
		{ cos(Roll), -sin(Roll), 0, 0 },
		{ sin(Roll), cos(Roll) , 0, 0 },
		{ 0        , 0         , 1, 0 },
		{ 0        , 0         , 0, 1 }
	});

	Matrix_4x4 rotation = pitchMat * yawMat * rollMat;

	Matrix_4x4 scale = Matrix_4x4({
		{ P_Scale.X, 0, 0, 0 },
		{ 0, P_Scale.Y, 0, 0 },
		{ 0, 0, P_Scale.Z, 0 },
		{ 0, 0, 0        , 1 }
	});

	Matrix_4x4 Model_Matrix = translation * rotation * scale;

	if (Vertex_Colors["Col"].size() == Vertex_Positions.size()) {
		for (int i = 0; i < Vertex_Positions.size(); i++) {
			Vertex Vert = Vertex(Vertex_Positions[i], Vertex_Colors["Col"][i]);
			Vert.Pos = Vert.Pos * Model_Matrix;
			Vertex_Output.push_back(Vert);
		}
	}
	else {
		for (int i = 0; i < Vertex_Positions.size(); i++) {
			Vertex Vert = Vertex(Vertex_Positions[i], Rgb(0,0,0));
			Vert.Pos = Vert.Pos * Model_Matrix;
			Vertex_Output.push_back(Vert);
		}
	}
}