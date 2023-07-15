#include "R_Vertex.h"

R_Vertex::R_Vertex() {
	Color = R_Color(0, 0, 0, 2055);
	Location = R_Vec_3(0, 0, 0);
	Transform_Weights;
	Shader_Weights;
	Custom_Weights;
}

void R_Vertex::read(std::string Input) {
}
std::string R_Vertex::write(uint8_t Tab) {
	R_String Result;
	Result.nl(Tab, 0, 1) << "Transform << " << Location.write();
	Result.nl(Tab, 0, 1) << "Color     << " << Color.write();
	Result.nl(Tab, 0, 1) << "TW        ⟨ " << Transform_Weights.size() << " ⟩";
	Result.nl(Tab, 0, 1) << "SW        ⟨ " << Shader_Weights.size() << " ⟩";
	Result.nl(Tab, 0, 1) << "CW        ⟨ " << Custom_Weights.size() << " ⟩";

	Result.nl(Tab, 0, 2) << "Transform Weights {";
	Result.nl(Tab, 0, 2) << "} Transform Weights";
	Result.nl(Tab, 0, 2) << "Shader Weights {";
	Result.nl(Tab, 0, 2) << "} Shader Weights";
	Result.nl(Tab, 0, 2) << "Custom Weights {";
	Result.nl(Tab, 0, 2) << "} Custom Weights";
	return Result.write();
}

uint64_t R_Vertex::size() {
	return 1;
}