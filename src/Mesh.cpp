#include "Mesh.h"

Vertex::Vertex() {
	Pos = Vec3();
	Color = Rgb(1);
	Normal = Vec3();
	UV = Vec2();
}

Vertex::Vertex(const Vec3& P_Pos) {
	Pos = P_Pos;
	Color = Rgb(1);
	Normal = Vec3();
	UV = Vec2();
}

Vertex::Vertex(const Vec3& P_Pos, const Vec2& P_Uv) {
	Pos = P_Pos;
	Color = Rgb(1);
	Normal = Vec3();
	UV = P_Uv;
}

Vertex::Vertex(const Vec3& P_Pos, const Rgb& P_Color) {
	Pos = P_Pos;
	Color = P_Color;
	Normal = Vec3();
	UV = Vec2();
}

Vertex::Vertex(const Vec3& P_Pos, const Vec3& P_Normal) {
	Pos = P_Pos;
	Color = Rgb(1);
	Normal = P_Normal;
	UV = Vec2();
}

Vertex::Vertex(const Vec3& P_Pos, const Rgb& P_Color, const Vec3& P_Normal) {
	Pos = P_Pos;
	Color = P_Color;
	Normal = P_Normal;
	UV = Vec2();
}

Vertex::Vertex(const Vec3& P_Pos, const Vec2& P_Uv, const Vec3& P_Normal) {
	Pos = P_Pos;
	Color = Rgb();
	Normal = P_Normal;
	UV = P_Uv;
}

Vec2 Vertex::project(const Vec3& cameraPos, const Vec3& cameraDir, const double& FOV) const {
	const Vec3 relative = Vec3(Pos.X - cameraPos.X, Pos.Y - cameraPos.Y, Pos.Z - cameraPos.Z);

	const double cosYaw = cos(cameraDir.Y);
	const double sinYaw = sin(cameraDir.Y);
	const double cosPitch = cos(cameraDir.X);
	const double sinPitch = sin(cameraDir.X);

	const double x = relative.X * cosYaw - relative.Z * sinYaw;
	const double z = relative.X * sinYaw + relative.Z * cosYaw;
	const double y = relative.Y * cosPitch - z * sinPitch;

	const double f = FOV / (y + FOV);

	return Vec2(f * x, f * z);
}

Mesh_Triangle::Mesh_Triangle(const size_t& P_I1, const size_t& P_I2, const size_t& P_I3) {
	Index1 = P_I1;
	Index2 = P_I2;
	Index3 = P_I3;

	Normal_1 = 0;
	Normal_2 = 0;
	Normal_3 = 0;

	UV_1 = 0;
	UV_2 = 0;
	UV_3 = 0;
}

Mesh::Mesh() {
	Faces = vector<Mesh_Triangle>();
	Vertex_Positions = vector<Vec3>();
	Vertex_Normals = map<string, vector<Vec3>>();
	Vertex_UV_Coords = map<string, vector<Vec2>>();
	Vertex_Colors = map<string, vector<Rgb>>();
	Vertex_Weights = map<string, map<string, double>>();

	Vertex_Output = vector<Vertex>();
}

void Mesh::f_processModelMatrix(const Vec3& P_Translate, const Vec3& P_Rotate, const Vec3& P_Scale) {
	const Matrix_4x4 translation = Matrix_4x4({
		{ 1, 0, 0, P_Translate.X },
		{ 0, 1, 0, P_Translate.Y },
		{ 0, 0, 1, P_Translate.Z },
		{ 0, 0, 0, 1             }
	});

	const double Pitch = P_Rotate.X * RAD;
	const double Yaw   = P_Rotate.Y * RAD;
	const double Roll  = P_Rotate.Z * RAD;

	const Matrix_4x4 pitchMat = Matrix_4x4({
		{ 1, 0         , 0          , 0 },
		{ 0, cos(Pitch), -sin(Pitch), 0 },
		{ 0, sin(Pitch), cos(Pitch) , 0 },
		{ 0, 0         , 0          , 1 }
	});

	const Matrix_4x4 yawMat = Matrix_4x4({
		{ cos(Yaw) , 0, sin(Yaw), 0 },
		{ 0        , 1, 0       , 0 },
		{ -sin(Yaw), 0, cos(Yaw), 0 },
		{ 0        , 0, 0       , 1 }
	});

	const Matrix_4x4 rollMat = Matrix_4x4({
		{ cos(Roll), -sin(Roll), 0, 0 },
		{ sin(Roll), cos(Roll) , 0, 0 },
		{ 0        , 0         , 1, 0 },
		{ 0        , 0         , 0, 1 }
	});

	const Matrix_4x4 scale = Matrix_4x4({
		{ P_Scale.X, 0, 0, 0 },
		{ 0, P_Scale.Y, 0, 0 },
		{ 0, 0, P_Scale.Z, 0 },
		{ 0, 0, 0        , 1 }
	});

	Matrix_4x4 rotation_matrix = pitchMat * yawMat * rollMat;
	model_matrix = translation * rotation_matrix * scale;
}

void Mesh::f_processVertexShader(const Matrix_4x4& P_Camera_Matrix, const Matrix_4x4& P_Projection_Matrix, const Matrix_4x4& P_Viewport_Matrix) {
	const Matrix_4x4 View_Matrix = P_Viewport_Matrix * P_Projection_Matrix * P_Camera_Matrix.inv() * model_matrix; // Multiplication order is important for left to right

	Vertex_Output = vector(Vertex_Positions.size(), Vertex());

	for (int i = 0; i < Vertex_Positions.size(); i++) {
		const Vec4 vertShader = Vec4(Vertex_Positions[i], 1) * View_Matrix;
		Vertex_Output[i] = Vertex(
			Vec3(
				vertShader.X / vertShader.W,
				vertShader.Y / vertShader.W,
				vertShader.Z / vertShader.W
			)
		);
	}

	if (Vertex_Normals["Normals"].size() > 0) {
		for (const Mesh_Triangle& Tri : Faces) {
			Vertex_Output[Tri.Index1].Normal = Vertex_Normals["Normals"][Tri.Normal_1];
			Vertex_Output[Tri.Index2].Normal = Vertex_Normals["Normals"][Tri.Normal_2];
			Vertex_Output[Tri.Index3].Normal = Vertex_Normals["Normals"][Tri.Normal_3];
		}
	}
	if (Vertex_UV_Coords["UV"].size() > 0) {
		for (const Mesh_Triangle& Tri : Faces) {
			Vertex_Output[Tri.Index1].UV = Vertex_UV_Coords["UV"][Tri.UV_1];
			Vertex_Output[Tri.Index2].UV = Vertex_UV_Coords["UV"][Tri.UV_2];
			Vertex_Output[Tri.Index3].UV = Vertex_UV_Coords["UV"][Tri.UV_3];
		}
	}
	if (Vertex_Colors["Color"].size() == Vertex_Positions.size()) {
		for (const Mesh_Triangle& Tri : Faces) {
			Vertex_Output[Tri.Index1].Color = Vertex_Colors["Color"][Tri.Index1];
			Vertex_Output[Tri.Index2].Color = Vertex_Colors["Color"][Tri.Index2];
			Vertex_Output[Tri.Index3].Color = Vertex_Colors["Color"][Tri.Index3];
		}
	}
}