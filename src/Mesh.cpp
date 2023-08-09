#include "Mesh.h"

Vertex::Vertex() {
	Pos = Vec3();
	Color = Rgb();
	Normal = Vec3();
	UV = Vec2();
}

Vertex::Vertex(Vec3 P_Pos) {
	Pos = P_Pos;
	Color = Rgb();
	Normal = Vec3();
	UV = Vec2();
}

Vertex::Vertex(Vec3 P_Pos, Rgb P_Color) {
	Pos = P_Pos;
	Color = P_Color;
	Normal = Vec3();
	UV = Vec2();
}

Vec2 Vertex::project(const Vec3& cameraPos, const Vec3& cameraDir, double FOV) const {
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

Mesh_Triangle::Mesh_Triangle(size_t P_I1, size_t P_I2, size_t P_I3) {
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
	Matrix_4x4 translation = Matrix_4x4({
		{ 1, 0, 0, P_Translate.X },
		{ 0, 1, 0, P_Translate.Y },
		{ 0, 0, 1, P_Translate.Z },
		{ 0, 0, 0, 1             }
	});

	double Yaw   = P_Rotate.X * RAD;
	double Pitch = P_Rotate.Y * RAD;
	double Roll  = P_Rotate.Z * RAD;

	Matrix_4x4 yawMat = Matrix_4x4({
		{ cos(Yaw) , 0, sin(Yaw), 0 },
		{ 0        , 1, 0       , 0 },
		{ -sin(Yaw), 0, cos(Yaw), 0 },
		{ 0        , 0, 0       , 1 }
	});

	Matrix_4x4 pitchMat = Matrix_4x4({
		{ 1, 0         , 0          , 0 },
		{ 0, cos(Pitch), -sin(Pitch), 0 },
		{ 0, sin(Pitch), cos(Pitch) , 0 },
		{ 0, 0         , 0          , 1 }
	});

	Matrix_4x4 rollMat = Matrix_4x4({
		{ cos(Roll), -sin(Roll), 0, 0 },
		{ sin(Roll), cos(Roll) , 0, 0 },
		{ 0        , 0         , 1, 0 },
		{ 0        , 0         , 0, 1 }
	});

	Matrix_4x4 scale = Matrix_4x4({
		{ P_Scale.X, 0, 0, 0 },
		{ 0, P_Scale.Y, 0, 0 },
		{ 0, 0, P_Scale.Z, 0 },
		{ 0, 0, 0        , 1 }
	});

	model_matrix = translation * (pitchMat * yawMat * rollMat) * scale;
}

void Mesh::f_processVertexShader(Matrix_4x4& P_Camera_Matrix, const Matrix_4x4& P_Projection_Matrix, const Matrix_4x4& P_Viewport_Matrix) {
	Matrix_4x4 View_Matrix = model_matrix * P_Viewport_Matrix * P_Projection_Matrix * P_Camera_Matrix.inv(); // Multiplication order is important

	Vertex_Output = vector(Vertex_Positions.size(), Vertex());

	if (Vertex_UV_Coords["UV"].size() > 0) {
		for (const Mesh_Triangle& Tri : Faces) {
			Vertex Vert1 = Vertex(Vertex_Positions[Tri.Index1]);
			Vertex Vert2 = Vertex(Vertex_Positions[Tri.Index2]);
			Vertex Vert3 = Vertex(Vertex_Positions[Tri.Index3]);

			Vert1.UV = Vertex_UV_Coords["UV"][Tri.UV_1];
			Vert2.UV = Vertex_UV_Coords["UV"][Tri.UV_2];
			Vert3.UV = Vertex_UV_Coords["UV"][Tri.UV_3];

			Vec4 vertShader1 = Vec4(Vert1.Pos, 1) * View_Matrix;
			Vec4 vertShader2 = Vec4(Vert2.Pos, 1) * View_Matrix;
			Vec4 vertShader3 = Vec4(Vert3.Pos, 1) * View_Matrix;

			Vert1.Pos = Vec3(
				vertShader1.X / vertShader1.W,
				vertShader1.Y / vertShader1.W,
				vertShader1.Z / vertShader1.W
			);
			Vert2.Pos = Vec3(
				vertShader2.X / vertShader2.W,
				vertShader2.Y / vertShader2.W,
				vertShader2.Z / vertShader2.W
			);
			Vert3.Pos = Vec3(
				vertShader3.X / vertShader3.W,
				vertShader3.Y / vertShader3.W,
				vertShader3.Z / vertShader3.W
			);

			Vertex_Output[Tri.Index1] = Vert1;
			Vertex_Output[Tri.Index2] = Vert2;
			Vertex_Output[Tri.Index3] = Vert3;
		}
	}
	else if (Vertex_Colors["Color"].size() > 0) {
		for (int i = 0; i < Vertex_Positions.size(); i++) {
			Vertex Vert = Vertex(Vertex_Positions[i], Vertex_Colors["Color"][i]);
			Vec4 vertShader = Vec4(Vert.Pos, 1) * View_Matrix;
			Vert.Pos = Vec3(
				vertShader.X / vertShader.W,
				vertShader.Y / vertShader.W,
				vertShader.Z / vertShader.W
			);
			Vertex_Output[i] = Vert;
		}
	}
	else {
		for (int i = 0; i < Vertex_Positions.size(); i++) {
			Vertex Vert = Vertex(Vertex_Positions[i], Rgb(1, 1, 1));
			Vec4 vertShader = Vec4(Vert.Pos, 1) * View_Matrix;
			Vert.Pos = Vec3(
				vertShader.X / vertShader.W,
				vertShader.Y / vertShader.W,
				vertShader.Z / vertShader.W
			);
			Vertex_Output[i] = Vert;
		}
	}
}