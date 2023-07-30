#include "File_Readers.h"

Obj_File_Loader::Obj_File_Loader(QObject* parent, string P_File_Path) : QThread(parent) {
	File_Path = P_File_Path;
}

void Obj_File_Loader::run() {
	emit updateProgress_Signal(0);
	ifstream file(File_Path, ios::sync_with_stdio(false));
	ifstream fileTemp(File_Path, ios::sync_with_stdio(false));

	int lineCount = 0;
	string file_line;
	while (getline(fileTemp, file_line)) {
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

	string line;
	while (getline(file, line)) {
		vector<string> Tokens = Math::splitString(line);
		if (!Tokens.empty()) {
			if (Tokens[0] == "v") {
				Vec3 Pos(
					stod(Tokens[1]),
					stod(Tokens[2]),
					stod(Tokens[3])
				);
				Imported_Mesh.MeshData.Vertex_Positions.push_back(Pos);
				if (Tokens.size() > 6) {
					Rgb Color(
						stod(Tokens[4]),
						stod(Tokens[5]),
						stod(Tokens[6])
					);
					Imported_Mesh.MeshData.Vertex_Colors["Col"].push_back(Color);
				}
			}
			else if (Tokens[0] == "f") {
				Mesh_Face triangle(
					stoull(Math::splitString(Tokens[1], "/")[0]) - 1,
					stoull(Math::splitString(Tokens[2], "/")[0]) - 1,
					stoull(Math::splitString(Tokens[3], "/")[0]) - 1
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