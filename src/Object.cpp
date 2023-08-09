#include "Object.h"

Object::Object() {
	Name = "Default";

	Pos = Vec3();
	Anchor = Vec3();
	Rot_Euler = Vec3();
	Scale = Vec3(1, 1, 1);
	MeshData = Mesh();
	MeshShader = Shader();
}

Object::Object(const string& P_Name) {
	Name = P_Name;

	Pos = Vec3();
	Anchor = Vec3();
	Rot_Euler = Vec3();
	Scale = Vec3(1, 1, 1);
	MeshData = Mesh();
	MeshShader = Shader();
}

void Object::scale(const Vec3& P_Scale) {
	Scale += P_Scale;
}

void Object::rotate(const Vec3& P_Rot) {
	Rot_Euler += P_Rot;
}

void Object::translate(const Vec3& P_Pos) {
	Pos += P_Pos;
}