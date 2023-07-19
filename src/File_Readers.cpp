#include "File_Readers.h"


Kerzenlicht_File_Loader::Kerzenlicht_File_Loader() : QObject() {}

void Kerzenlicht_File_Loader::loadFile(const std::string filePath)
{
	QFile file(QString::fromStdString(filePath));
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		emit loadingFinished();
		return;
	}

	uint_fast64_t totalBytes = file.size();
	uint_fast64_t bytesRead = 0;
	int linesRead = 0;
	const int linesPerUpdate = 128;

	while (!file.atEnd()) {
		std::string line = file.readLine().toStdString();
		bytesRead += line.size();

		// Process the line (you can implement your .obj parsing logic here)

		std::vector<std::string> Tokens = Math::splitString(line);
		if (!Tokens.empty()) {
			if (Tokens[0] == "v") {

			}
		}

		// Update progress every linesPerUpdate lines read
		if (++linesRead == linesPerUpdate) {
			updateProgress(bytesRead, totalBytes);
			linesRead = 0;
		}
	}

	// Update progress for the remaining lines
	if (linesRead > 0) {
		updateProgress(bytesRead, totalBytes);
	}

	file.close();
	emit loadingFinished();
}

void Kerzenlicht_File_Loader::updateProgress(uint_fast8_t bytesRead, uint_fast8_t totalBytes) {
	uint_fast8_t progress = static_cast<uint_fast8_t>((bytesRead * 100) / totalBytes);
	emit progressUpdated(progress);
}

Obj_File_Loader::Obj_File_Loader() : QObject() {}

void Obj_File_Loader::loadObjFile(const std::string filePath) {
	std::ifstream file(filePath);
	std::ifstream fileTemp(filePath);
	
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

		// Update progress every linesPerUpdate lines read
		if (++linesRead == linesPerUpdate) {
			updateProgress(linesRead, lineCount);
			linesRead = 0;
		}
	}

	// Update progress for the remaining lines
	if (linesRead > 0) {
		updateProgress(linesRead, lineCount);
	}

	file.close();
	Imported_Mesh.loadBuffers();
	emit loadingFinished(Imported_Mesh);
}

void Obj_File_Loader::updateProgress(int Precent, int Total) {
	int progress = static_cast<int>((Precent * 100) / Total);
	emit progressUpdated(progress);
}