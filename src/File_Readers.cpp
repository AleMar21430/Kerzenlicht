#include "File_Readers.h"

Obj_File_Loader::Obj_File_Loader(QObject* parent, std::string P_File_Path) : QThread(parent) {
	File_Path = P_File_Path;
}

void Obj_File_Loader::run() {
	emit updateProgress_Signal(0);
	std::ifstream file(File_Path);
	std::ifstream fileTemp(File_Path);

	int lineCount = 0;
	std::string file_line;
	while (std::getline(fileTemp, file_line)) {
		lineCount++;
	}

	int linesRead = 0;
	int Temp = 0;
	if (static_cast<int>(lineCount / 512) > 2) {
		Temp = static_cast<int>(lineCount / 512);
	}
	else {
		Temp = 2;
	}
	const int linesPerUpdate = Temp;

	Object Imported_Mesh = Object("Imported");

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

		// Update progress every linesPerUpdate lines read
		if (++linesRead == linesPerUpdate) {
			emit updateProgress_Signal(static_cast<int>(linesRead / lineCount * 100));
			linesRead = 0;
		}
	}

	emit updateProgress_Signal(100);

	file.close();
	emit loadingFinished_Signal(Imported_Mesh);
}