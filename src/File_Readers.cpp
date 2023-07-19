#include "File_Readers.h"

std::future<Object> loadObjThread(const std::string P_File_Path) {
	std::promise<Object> promise;
	std::future<Object> future = promise.get_future();

	std::ifstream file(P_File_Path);
	std::ifstream fileTemp(P_File_Path);

	int lineCount = 0;
	std::string file_line;
	while (std::getline(fileTemp, file_line)) {
		lineCount++;
	}

	int linesRead = 0;
	const int linesPerUpdate = 128;

	Object Imported_Mesh = Object("Imported", Object_Type::MESH);

	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> Tokens = Math::splitString(line);
		if (!Tokens.empty()) {
			if (Tokens[0] == "v") {
				Vec3 Pos(
					std::stod(Tokens[1]),
					std::stod(Tokens[2]),
					std::stod(Tokens[3])
				);
				Imported_Mesh.MeshData.Vertex_Positions.push_back(Pos);
				if (Tokens.size() > 6) {
					Rgb Color(
						std::stod(Tokens[4]),
						std::stod(Tokens[5]),
						std::stod(Tokens[6])
					);
					Imported_Mesh.MeshData.Vertex_Colors["Col"].push_back(Color);
				}
			}
			else if (Tokens[0] == "f") {
				Mesh_Face triangle(
					std::stoi(Math::splitString(Tokens[1], "/")[0]) - 1,
					std::stoi(Math::splitString(Tokens[2], "/")[0]) - 1,
					std::stoi(Math::splitString(Tokens[3], "/")[0]) - 1
				);
				Imported_Mesh.MeshData.Faces.push_back(triangle);
			}
		}
	}

	file.close();
	Imported_Mesh.loadBuffers();
	promise.set_value(Imported_Mesh);

	return future;
}