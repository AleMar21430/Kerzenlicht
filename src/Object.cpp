#include "Object.h"

Object::Object() {
	Name = "Default";
	Type = Object_Type::EMPTY;

	Pos = Vec3();
	Anchor = Vec3();
	Rot_Euler = Vec3();
	Scale = Vec3(1, 1, 1);

	if (Type == MESH) {
		MeshData = Mesh();
	}
}

Object::Object(std::string P_Name, Object_Type P_Type) {
	Name = P_Name;
	Type = P_Type;

	Pos = Vec3();
	Anchor = Vec3();
	Rot_Euler = Vec3();
	Scale = Vec3(1, 1, 1);

	if (Type == MESH) {
		MeshData = Mesh();
	}
}

void Object::loadBuffers() {
	if (Type == MESH) {
		MeshData.Vertex_Output = std::vector<Vertex>();
		if (MeshData.Vertex_Colors.size() > 0) {
			for (int i = 0; i < MeshData.Vertex_Positions.size(); i++) {
				Vertex vert = Vertex(MeshData.Vertex_Positions[i]);
				vert.Color = MeshData.Vertex_Colors["Col"][i];
				MeshData.Vertex_Output.push_back(vert);
			}
		}
		// TODO ///////////////////////////////////////////////////////////////////
		// Vertex Color Were not loading to buffers, will not render properly, fix.
		else {
			for (int i = 0; i < MeshData.Vertex_Positions.size(); i++) {
				Vertex vert = Vertex(MeshData.Vertex_Positions[i]);
				MeshData.Vertex_Output.push_back(vert);
			}
		}
	}
}

void Object::scale(Vec3 P_Scale) {
	Scale = Scale + P_Scale;
}

void Object::setScale(Vec3 P_Scale) {
	Scale = P_Scale;
}

void Object::rotate(Vec3 P_Rot) {
	Rot_Euler = Rot_Euler + P_Rot;
}

void Object::translate(Vec3 P_Pos) {
	Pos = Pos + P_Pos;
}

void Object::processTransform() {
	if (Type == MESH) {
		for (int i = 0; i < MeshData.Vertex_Positions.size(); i++) {
			Vertex& vert = MeshData.Vertex_Output[i];
			vert.Pos = MeshData.Vertex_Positions[i] * Scale;
			vert.Pos.rotate(Anchor, Rot_Euler);
			vert.Pos = vert.Pos + Pos;
		}
	}
}