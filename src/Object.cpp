#include "Object.h"

Object::Object() {
	Name = "Default";

	Pos = Vec3();
	Anchor = Vec3();
	Rot_Euler = Vec3();
	Scale = Vec3(1, 1, 1);
	MeshData = Mesh();
}

Object::Object(std::string P_Name) {
	Name = P_Name;

	Pos = Vec3();
	Anchor = Vec3();
	Rot_Euler = Vec3();
	Scale = Vec3(1, 1, 1);
	MeshData = Mesh();
}

void Object::scale(Vec3 P_Scale) {
	Scale = Scale + P_Scale;
}

void Object::rotate(Vec3 P_Rot) {
	Rot_Euler = Rot_Euler + P_Rot;
}

void Object::translate(Vec3 P_Pos) {
	Pos = Pos + P_Pos;
}