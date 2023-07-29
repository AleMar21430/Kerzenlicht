#pragma once

#include "Math.h"

#include "Mesh.h"

struct Object {
	std::string Name;

	Mesh MeshData;

	Vec3 Pos;
	Vec3 Anchor;
	Vec3 Rot_Euler;
	Vec3 Scale;

	Object();
	Object(std::string P_Name);

	void loadBuffers();
	void scale(Vec3 P_Scale);
	void setScale(Vec3 P_Scale);
	void rotate(Vec3 P_Rot);
	void translate(Vec3 P_Pos = Vec3());
	void processTransform();
};