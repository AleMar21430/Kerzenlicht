#pragma once

#include "Math.h"

#include "Mesh.h"

enum Object_Type {
	EMPTY,
	MESH,
};

struct Object {
	std::string Name;
	Object_Type Type;

	Mesh MeshData;

	Vec3 Pos;
	Vec3 Anchor;
	Vec3 Rot_Euler;
	Vec3 Scale;

	Object();
	Object(std::string P_Name, Object_Type P_Type);

	void loadBuffers();
	void scale(Vec3 P_Scale);
	void setScale(Vec3 P_Scale);
	void rotate(Vec3 P_Rot);
	void translate(Vec3 P_Pos = Vec3());
	void processTransform();
};