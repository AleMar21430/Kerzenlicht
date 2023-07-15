#include "R_Mesh.h"

R_Mesh::R_Mesh() {
	Name = "Mesh";
	Vertices;
	Face_Vertex_Count;
	Face_Vertex_Indices;
	Vertex_Normals;
	Texture_Mapping;
}

void R_Mesh::read(std::string Input) {
}

std::string R_Mesh::write(uint8_t Tab){
	R_String Result;
	Result.nl(Tab, 0, 1) << Name << " {";

	Result.nl(Tab, 1, 2) << "Vertices      ⟨ " << Vertices.size() << " ⟩";
	Result.nl(Tab, 1, 1) << "Edges         ⟨ " << 0 << " ⟩";
	Result.nl(Tab, 1, 1) << "Faces         ⟨ " << Face_Vertex_Count.size() << " ⟩";
	Result.nl(Tab, 1, 1) << "UV Sets       ⟨ " << 0 << " ⟩";
	Result.nl(Tab, 1, 1) << "Vertex Groups ⟨ " << 0 << " ⟩";

	Result.nl(Tab, 1, 2) << "Vertices {";
	for (uint64_t i = 0; i < Vertices.size(); i++) {
		Result.nl(Tab, 2, 2) << "V[" << i << "] {";
		Result.nl(Tab, 2, 1) << Vertices[i].write(Tab + 3);
		Result.nl(Tab, 2, 2) << "} V[" << i << "]";
	}
	Result.nl(Tab, 1, 2) << "} Vertices";

	// Faces
	Result.nl(Tab, 1, 2) << "Faces {";
	int j = 0;
	for (uint64_t i = 0; i < Face_Vertex_Count.size(); i++) {
		Result.nl(Tab, 1, 1) << " ⟨ " << Face_Vertex_Count[i] << " ⟩ << ( ";
		while (j < Face_Vertex_Count[i]) {
			Result << Face_Vertex_Indices[j] << " | ";
			j++;
		}
		Result << "\b\b)";
	}
	Result.nl(Tab, 1, 2) << "} Faces";
	Result.nl(Tab, 0, 2) << "} " << Name;
	return Result.write();
}

uint64_t R_Mesh::size() {
	return 0;
}