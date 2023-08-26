#include "File_Readers.h"

Obj_File_Loader::Obj_File_Loader(QObject* parent, string iobj, const Vec3& ipos, const Vec3& irot, const Vec3& iscale, const string itexture, const Fragment_Shader_Type::Enum ishader) : QThread(parent) {
	File_Path = iobj;
	pos = ipos;
	rot = irot;
	scale = iscale;
	tex = itexture;
	shader = ishader;
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
					Imported_Mesh.MeshData.Vertex_Colors["Color"].push_back(Color);
				}
			}
			else if (Tokens[0] == "vt") {
				Vec2 Pos(
					stod(Tokens[1]),
					stod(Tokens[2])
				);
				Imported_Mesh.MeshData.Vertex_UV_Coords["UV"].push_back(Pos);
			}
			else if (Tokens[0] == "vn") {
				Vec3 Normal(
					stod(Tokens[1]),
					stod(Tokens[2]),
					stod(Tokens[3])
				);
				Imported_Mesh.MeshData.Vertex_Normals["Normals"].push_back(Normal);
			}
			else if (Tokens[0] == "f") {
				vector<string> Vert1 = Math::splitString(Tokens[1], "/");
				vector<string> Vert2 = Math::splitString(Tokens[2], "/");
				vector<string> Vert3 = Math::splitString(Tokens[3], "/");

				Mesh_Triangle triangle = Mesh_Triangle(
					stoull(Vert1[0]) - 1,
					stoull(Vert2[0]) - 1,
					stoull(Vert3[0]) - 1
				);
				if (Vert1.size() > 1) {
					if (!Vert1[1].empty()) {
						triangle.UV_1 = stoull(Vert1[1]) - 1;
						triangle.UV_2 = stoull(Vert2[1]) - 1;
						triangle.UV_3 = stoull(Vert3[1]) - 1;
					}
				}
				if (Vert1.size() == 3) {
					if (!Vert1[2].empty()) {
						triangle.Normal_1 = stoull(Vert1[2]) - 1;
						triangle.Normal_2 = stoull(Vert2[2]) - 1;
						triangle.Normal_3 = stoull(Vert3[2]) - 1;
					}
				}
				Imported_Mesh.MeshData.Faces.push_back(triangle);
			}
		}

		// Update progress every linesPerUpdate lines read
		if (++linesRead == linesPerUpdate) {
			emit updateProgress_Signal(static_cast<int>(linesRead / lineCount * 100));
			linesRead = 0;
		}
	}
	Imported_Mesh.Pos = pos;
	Imported_Mesh.Scale = scale;
	Imported_Mesh.Rot_Euler = rot;
	Imported_Mesh.MeshShader.Frag_Shader = shader;
	if (tex != "") Imported_Mesh.MeshShader.Albedo.loadfromBitmap(tex);

	emit updateProgress_Signal(100);

	file.close();
	emit loadingFinished_Signal(Imported_Mesh);
}