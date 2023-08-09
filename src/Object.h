#pragma once

#include "Math.h"

#include "Mesh.h"
#include "Shader.h"

struct Object {
	string Name;

	Mesh MeshData;
	Shader MeshShader;

	Vec3 Pos;
	Vec3 Anchor;
	Vec3 Rot_Euler;
	Vec3 Scale;

	Object();
	Object(const string& P_Name);

	void scale(const Vec3& P_Scale);
	void rotate(const Vec3& P_Rot);
	void translate(const Vec3& P_Pos = Vec3());
};