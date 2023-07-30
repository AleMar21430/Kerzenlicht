#pragma once

#include "Math.h"

#include "Mesh.h"

struct Object {
	string Name;

	Mesh MeshData;

	Vec3 Pos;
	Vec3 Anchor;
	Vec3 Rot_Euler;
	Vec3 Scale;

	Object();
	Object(string P_Name);

	void scale(Vec3 P_Scale);
	void rotate(Vec3 P_Rot);
	void translate(Vec3 P_Pos = Vec3());
};