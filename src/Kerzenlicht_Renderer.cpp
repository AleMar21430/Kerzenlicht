#include "Kerzenlicht_Renderer.h"

Kerzenlicht_Renderer::Kerzenlicht_Renderer(QT_Text_Stream* P_Log) : QT_Graphics_View() {
	Log = P_Log;

	Mouse_Pressed = false;
	Mouse_Down_Pos = QPoint(0, 0);

	QSettings settings("Raylight", "KerzenLicht");

	ResX = settings.value("ResX", 1920).toInt();
	ResY = settings.value("ResY", 1080).toInt();
	Aspect_Ratio = static_cast<double>(ResX) / static_cast<double>(ResY);
	Pen_Color = Rgba();
	Pen_Opacity = 1.0f;
	Pixmap = std::vector(ResX, std::vector<Rgba>(ResY));

	View_Mode = Render_Mode::WIREFRAME;

	Object_Array = std::map<std::string, Object>();

	Face_Buffer = std::vector<Mesh_Face>();
	Vertex_Colors_Buffer = std::vector<Rgb>();
	Vertex_Positions_Buffer = std::vector<Vec3>();
	Vertex_Weights_Buffer = std::map<std::string, std::map<std::string, double>>();

	for (int x = 0; x < ResX; x++) {
		for (int y = 0; y < ResY; y++) {
			Pixmap[x][y] = Rgba(0.1, 0.1, 0.1, 1);
		}
	}

	Scene = new QGraphicsScene();
	setScene(Scene);
	Menu = new Renderer_Menu(this);

	std::stringstream log;
	log << "Renderer Settings" << std::endl;
	log << "Res X: " << Pixmap[0].size() << std::endl;
	log << "Res Y: " << Pixmap.size() << std::endl;
	Log->log(log.str());

	///////////
	// Scene //
	///////////

	createObject("Paimon");

	loadObj("./Paimon.obj", false, true, true);
	loadModel("Paimon");
	Object_Array["Paimon"].loadBuffers();
	clearBuffers();

	Object_Array["Paimon"].setScale(Vec3(0.175, 0.175, 0.175));
	Object_Array["Paimon"].translate(Vec3(0, -0.95, 0));
	Object_Array["Paimon"].processTransform();

	renderWireframe();
	drawToSurface();
}

void Kerzenlicht_Renderer::setImage(std::string P_File) {
	Scene->clear();
	QPixmap Image(QString::fromStdString(P_File));
	QGraphicsPixmapItem* Item = new QGraphicsPixmapItem(Image);
	Scene->addItem(Item);
	centerOn(Item->boundingRect().center());
	fitInView(Item->boundingRect(), Qt::KeepAspectRatio);
}

void Kerzenlicht_Renderer::drawToSurface() {
	Scene->clear();
	QImage image(ResX, ResY, QImage::Format_RGBA8888);

	for (int x = 0; x < ResX; x++) {
		for (int y = 0; y < ResY; y++) {
			Rgba& pixel = Pixmap[x][ResY - y - 1];
			QRgb rgba = qRgba(
				static_cast<uint8_t>(pixel.R*255),
				static_cast<uint8_t>(pixel.G*255),
				static_cast<uint8_t>(pixel.B*255),
				static_cast<uint8_t>(pixel.A*255)
			);
			image.setPixel(x, y, rgba);
		}
	}

	QPixmap Image = QPixmap::fromImage(image);
	QGraphicsPixmapItem* Item = new QGraphicsPixmapItem(Image);
	Item->setTransformationMode(Qt::TransformationMode::SmoothTransformation);
	Scene->addItem(Item);
	centerOn(Item->boundingRect().center());
	fitInView(Item->boundingRect(), Qt::KeepAspectRatio);
}

void Kerzenlicht_Renderer::wheelEvent(QWheelEvent* P_Event) {
	for (std::pair<const std::string, Object>& Obj : Object_Array) {
		float Delta = P_Event->angleDelta().y()*0.00005;
		Obj.second.scale(Vec3(Delta, Delta, Delta));
		Obj.second.processTransform();
		renderFrame();
	}
}

void Kerzenlicht_Renderer::mousePressEvent(QMouseEvent* P_Event) {
	Mouse_Pressed = true;
	Mouse_Down_Pos = P_Event->pos();
}

void Kerzenlicht_Renderer::mouseMoveEvent(QMouseEvent* P_Event) {
	if (Mouse_Pressed) {
		double Delta = P_Event->pos().x() - Mouse_Down_Pos.x();
		for (std::pair<const std::string, Object>& Obj : Object_Array) {
			Obj.second.rotate(Vec3(0,Delta*0.001,0));
			Obj.second.processTransform();
			renderFrame();
		}
	}
}

void Kerzenlicht_Renderer::mouseReleaseEvent(QMouseEvent* P_Event) {
	Mouse_Pressed = false;
}

void Kerzenlicht_Renderer::resizeEvent(QResizeEvent* P_Event) {
	centerOn(scene()->items()[0]->boundingRect().center());
	fitInView(scene()->items()[0]->boundingRect(), Qt::KeepAspectRatio);
}

void Kerzenlicht_Renderer::closeEvent(QCloseEvent* P_Event) {
	P_Event->accept();
}

void Kerzenlicht_Renderer::setPenColor(Rgba P_Color) {
	Pen_Color = P_Color;
}

void Kerzenlicht_Renderer::setPenOpacity(float P_Opacity) {
	Pen_Opacity = P_Opacity;
}

void Kerzenlicht_Renderer::renderClear() {
	for (int x = 0; x < ResX; x++) {
		for (int y = 0; y < ResY; y++) {
			Pixmap[x][y] = Rgba(0.1, 0.1, 0.1, 1);
		}
	}
}

void Kerzenlicht_Renderer::renderPixel(uint32_t P_X, uint32_t P_Y) {
	if (P_X < ResX && P_X >= 0 && P_Y < ResY && P_Y >= 0) {
		Pixmap[P_X][P_Y] = Pen_Color;
	}
}

void Kerzenlicht_Renderer::renderLine(int P_Start_X, int P_Start_Y, int P_End_X, int P_End_Y) {
	int dx = std::abs(P_End_X - P_Start_X);
	int dy = std::abs(P_End_Y - P_Start_Y);
	int err = dx - dy;
	int x = P_Start_X;
	int y = P_Start_Y;

	while (x != P_End_X || y != P_End_Y) {
		renderPixel(x,y);

		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x += (P_Start_X < P_End_X) ? 1 : -1;
		}
		if (e2 < dx) {
			err += dx;
			y += (P_Start_Y < P_End_Y) ? 1 : -1;
		}
	}
	renderPixel(P_End_X, P_End_Y);
}

void Kerzenlicht_Renderer::loadObj(std::string P_File, bool P_Vert_Colors, bool P_Textured, bool P_Normals) {
	clearBuffers();
	
	std::ifstream file(P_File);
	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> Tokens = Math::splitString(line);
		if (!Tokens.empty()) {
			if (Tokens[0] == "v") {
				if (!P_Vert_Colors) {
					Vec3 Pos(
						std::stod(Tokens[1]),
						std::stod(Tokens[2]),
						std::stod(Tokens[3])
					);
					Vertex_Positions_Buffer.push_back(Pos);
				}
				else {
					Vec3 Pos(
						std::stod(Tokens[1]),
						std::stod(Tokens[2]),
						std::stod(Tokens[3])
					);
					Rgb Color(
						std::stod(Tokens[4]),
						std::stod(Tokens[5]),
						std::stod(Tokens[6])
					);
					Vertex_Positions_Buffer.push_back(Pos);
					Vertex_Colors_Buffer.push_back(Color);
				}
			}
			else if (Tokens[0] == "f") {
				if (!P_Textured && !P_Normals) {
					Mesh_Face triangle(
						std::stoi(Tokens[1]) - 1,
						std::stoi(Tokens[2]) - 1,
						std::stoi(Tokens[3]) - 1
					);
					Face_Buffer.push_back(triangle);
				}
				else {
					Mesh_Face triangle(
						std::stoi(Math::splitString(Tokens[1], "/")[0]) - 1,
						std::stoi(Math::splitString(Tokens[2], "/")[0]) - 1,
						std::stoi(Math::splitString(Tokens[3], "/")[0]) - 1
					);
					Face_Buffer.push_back(triangle);
				}
			}
		}
	}
	file.close();
}

void Kerzenlicht_Renderer::createObject(std::string P_Name) {
	Object Temp(P_Name, Object_Type::MESH);
	Object_Array[P_Name] = Temp;
}

void Kerzenlicht_Renderer::loadModel(std::string P_Name) {
	Object& Ref = Object_Array[P_Name];
	if (Ref.Type == MESH) {
		Ref.MeshData.Vertex_Positions = Vertex_Positions_Buffer;
		Ref.MeshData.Faces = Face_Buffer;
		if (Vertex_Colors_Buffer.size() == Vertex_Positions_Buffer.size()) {
			Ref.MeshData.Vertex_Colors["Col"] = Vertex_Colors_Buffer;
		}
	}
}

void Kerzenlicht_Renderer::clearBuffers() {
	Vertex_Colors_Buffer = std::vector<Rgb>();
	Vertex_Positions_Buffer = std::vector<Vec3>();
	Face_Buffer = std::vector<Mesh_Face>();
}

void Kerzenlicht_Renderer::renderWireframe() {
	setPenColor(Rgba(1, 1, 1, 1));
	for (auto& Data : Object_Array) {
		if (Data.second.Type == MESH){
			for (Mesh_Face tri : Data.second.MeshData.Faces) {
				if (tri.I1 > 0 && tri.I1 < Data.second.MeshData.Vertex_Output.size() &&
					tri.I2 > 0 && tri.I2 < Data.second.MeshData.Vertex_Output.size() &&
					tri.I3 > 0 && tri.I3 < Data.second.MeshData.Vertex_Output.size()) {

					Vertex v1 = Data.second.MeshData.Vertex_Output[tri.I1];
					Vertex v2 = Data.second.MeshData.Vertex_Output[tri.I2];
					Vertex v3 = Data.second.MeshData.Vertex_Output[tri.I3];

					if (Aspect_Ratio >= 1) {
						int x1 = static_cast<int>((v1.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX);
						int y1 = static_cast<int>((v1.Pos.Y + 1.0f) * 0.5f * ResY);
						int x2 = static_cast<int>((v2.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX);
						int y2 = static_cast<int>((v2.Pos.Y + 1.0f) * 0.5f * ResY);
						int x3 = static_cast<int>((v3.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX);
						int y3 = static_cast<int>((v3.Pos.Y + 1.0f) * 0.5f * ResY);

						renderLine(x1, y1, x2, y2);
						renderLine(x2, y2, x3, y3);
						renderLine(x3, y3, x1, y1);
					}
					else {
						int x1 = static_cast<int>((v1.Pos.X + 1.0f) * 0.5f * ResX);
						int y1 = static_cast<int>((v1.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY);
						int x2 = static_cast<int>((v2.Pos.X + 1.0f) * 0.5f * ResX);
						int y2 = static_cast<int>((v2.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY);
						int x3 = static_cast<int>((v3.Pos.X + 1.0f) * 0.5f * ResX);
						int y3 = static_cast<int>((v3.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY);

						renderLine(x1, y1, x2, y2);
						renderLine(x2, y2, x3, y3);
						renderLine(x3, y3, x1, y1);
					}
				}
			}
		}
	}
}

void Kerzenlicht_Renderer::renderEdgeVisualizer() {
	for (auto& Data : Object_Array) {
		if (Data.second.Type == MESH) {
			for (Mesh_Face tri : Data.second.MeshData.Faces) {
				if (tri.I1 > 0 && tri.I1 < Data.second.MeshData.Vertex_Output.size() &&
					tri.I2 > 0 && tri.I2 < Data.second.MeshData.Vertex_Output.size() &&
					tri.I3 > 0 && tri.I3 < Data.second.MeshData.Vertex_Output.size()) {

					Vertex v1 = Data.second.MeshData.Vertex_Output[tri.I1];
					Vertex v2 = Data.second.MeshData.Vertex_Output[tri.I2];
					Vertex v3 = Data.second.MeshData.Vertex_Output[tri.I3];

					if (Aspect_Ratio >= 1) {
						int x1 = static_cast<int>((v1.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX);
						int y1 = static_cast<int>((v1.Pos.Y + 1.0f) * 0.5f * ResY);
						int x2 = static_cast<int>((v2.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX);
						int y2 = static_cast<int>((v2.Pos.Y + 1.0f) * 0.5f * ResY);
						int x3 = static_cast<int>((v3.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX);
						int y3 = static_cast<int>((v3.Pos.Y + 1.0f) * 0.5f * ResY);

						setPenColor(Rgba::random());
						renderLine(x1, y1, x2, y2);
						renderLine(x2, y2, x3, y3);
						renderLine(x3, y3, x1, y1);
					}
					else {
						int x1 = static_cast<int>((v1.Pos.X + 1.0f) * 0.5f * ResX);
						int y1 = static_cast<int>((v1.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY);
						int x2 = static_cast<int>((v2.Pos.X + 1.0f) * 0.5f * ResX);
						int y2 = static_cast<int>((v2.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY);
						int x3 = static_cast<int>((v3.Pos.X + 1.0f) * 0.5f * ResX);
						int y3 = static_cast<int>((v3.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY);

						setPenColor(Rgba::random());
						renderLine(x1, y1, x2, y2);
						renderLine(x2, y2, x3, y3);
						renderLine(x3, y3, x1, y1);
					}
				}
			}
		}
	}
}

void Kerzenlicht_Renderer::renderPointCloud() {
	setPenColor(Rgba(1, 1, 1, 1));
	for (auto& Data : Object_Array) {
		if (Data.second.Type == MESH) {
			for (Mesh_Face tri : Data.second.MeshData.Faces) {
				if (tri.I1 > 0 && tri.I1 < Data.second.MeshData.Vertex_Output.size() &&
					tri.I2 > 0 && tri.I2 < Data.second.MeshData.Vertex_Output.size() &&
					tri.I3 > 0 && tri.I3 < Data.second.MeshData.Vertex_Output.size()) {

					Vertex v1 = Data.second.MeshData.Vertex_Output[tri.I1];
					Vertex v2 = Data.second.MeshData.Vertex_Output[tri.I2];
					Vertex v3 = Data.second.MeshData.Vertex_Output[tri.I3];
					
					if (Aspect_Ratio >= 1) {
						if (Data.second.MeshData.Vertex_Colors.size() > 0) {
							setPenColor(Rgba::fromRgb(v1.Color));
							renderPixel(
								static_cast<int>((v1.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX),
								static_cast<int>((v1.Pos.Y + 1.0f) * 0.5f * ResY)
							);
							setPenColor(Rgba::fromRgb(v2.Color));
							renderPixel(
								static_cast<int>((v2.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX),
								static_cast<int>((v2.Pos.Y + 1.0f) * 0.5f * ResY)
							);
							setPenColor(Rgba::fromRgb(v3.Color));
							renderPixel(
								static_cast<int>((v3.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX),
								static_cast<int>((v3.Pos.Y + 1.0f) * 0.5f * ResY)
							);
						}
						else {
							renderPixel(
								static_cast<int>((v1.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX),
								static_cast<int>((v1.Pos.Y + 1.0f) * 0.5f * ResY)
							);
							renderPixel(
								static_cast<int>((v2.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX),
								static_cast<int>((v2.Pos.Y + 1.0f) * 0.5f * ResY)
							);
							renderPixel(
								static_cast<int>((v3.Pos.X / Aspect_Ratio + 1.0f) * 0.5f * ResX),
								static_cast<int>((v3.Pos.Y + 1.0f) * 0.5f * ResY)
							);
						}
					}
					else {
						if (Data.second.MeshData.Vertex_Colors.size() > 0) {
							setPenColor(Rgba::fromRgb(v1.Color));
							renderPixel(
								static_cast<int>((v1.Pos.X + 1.0f) * 0.5f * ResX),
								static_cast<int>((v1.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY)
							);

							setPenColor(Rgba::fromRgb(v2.Color));
							renderPixel(
								static_cast<int>((v2.Pos.X + 1.0f) * 0.5f * ResX),
								static_cast<int>((v2.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY)
							);

							setPenColor(Rgba::fromRgb(v3.Color));
							renderPixel(
								static_cast<int>((v3.Pos.X + 1.0f) * 0.5f * ResX),
								static_cast<int>((v3.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY)
							);
						}
						else {
							renderPixel(
								static_cast<int>((v1.Pos.X + 1.0f) * 0.5f * ResX),
								static_cast<int>((v1.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY)
							);
							renderPixel(
								static_cast<int>((v2.Pos.X + 1.0f) * 0.5f * ResX),
								static_cast<int>((v2.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY)
							);
							renderPixel(
								static_cast<int>((v3.Pos.X + 1.0f) * 0.5f * ResX),
								static_cast<int>((v3.Pos.Y * Aspect_Ratio + 1.0f) * 0.5f * ResY)
							);
						}
					}
				}
			}
		}
	}
}

void Kerzenlicht_Renderer::renderFrame() {
	renderClear();
	if (View_Mode == Render_Mode::WIREFRAME) {
		renderWireframe();
	}
	else if (View_Mode == Render_Mode::POINTCLOUD) {
		renderPointCloud();
	}
	else if (View_Mode == Render_Mode::VISUALIZER) {
		renderEdgeVisualizer();
	}
	drawToSurface();
}

void Kerzenlicht_Renderer::storeBmp(std::string P_File) {
	std::ofstream file(P_File, std::ios::binary);
	if (!file.is_open()) {
		return;
	}

	uint32_t filesize = ResX * ResY * 4;

	// BMP header (14 bytes)
	file.write("BM", 2);													// Signature
	file.write(reinterpret_cast<const char*>(&filesize), 4);				// File size
	file.write("\0\0\0\0", 4);												// Reserved
	uint32_t pixelDataOffset = 54;
	file.write(reinterpret_cast<const char*>(&pixelDataOffset), 4);			// Pixel data offset

	// DIB header (40 bytes)
	uint32_t dibHeaderSize = 40;
	file.write(reinterpret_cast<const char*>(&dibHeaderSize), 4);			// DIB header size
	file.write(reinterpret_cast<const char*>(&ResX), 4);					// Image width
	file.write(reinterpret_cast<const char*>(&ResY), 4);					// Image height
	uint16_t colorPlanes = 1;
	file.write(reinterpret_cast<const char*>(&colorPlanes), 2);				// Color planes
	uint16_t bitsPerPixel = 32;
	file.write(reinterpret_cast<const char*>(&bitsPerPixel), 2);			// Bits per pixel
	uint32_t compressionMethod = 0;
	file.write(reinterpret_cast<const char*>(&compressionMethod), 4);		// Compression method
	file.write(reinterpret_cast<const char*>(&filesize), 4);				// Image size
	uint32_t horizontalResolution = 2835;
	file.write(reinterpret_cast<const char*>(&horizontalResolution), 4);	// Horizontal resolution (pixels per meter)
	uint32_t verticalResolution = 2835;
	file.write(reinterpret_cast<const char*>(&verticalResolution), 4);		// Vertical resolution (pixels per meter)
	uint32_t colorsInPalette = 0;
	file.write(reinterpret_cast<const char*>(&colorsInPalette), 4);			// Colors in color palette
	uint32_t importantColors = 0;
	file.write(reinterpret_cast<const char*>(&importantColors), 4);			// Important colors

	// Pixel data
	for (size_t y = 0; y < ResY; y++) {
		for (size_t x = 0; x < ResX; x++) {
			const Rgba& pixel = Pixmap[x][y];
			uint8_t blue = static_cast<uint8_t>(pixel.B * 255.0f);
			uint8_t green = static_cast<uint8_t>(pixel.G * 255.0f);
			uint8_t red = static_cast<uint8_t>(pixel.R * 255.0f);
			uint8_t alpha = static_cast<uint8_t>(pixel.A * 255.0f);
			file.write(reinterpret_cast<const char*>(&blue), 1);
			file.write(reinterpret_cast<const char*>(&green), 1);
			file.write(reinterpret_cast<const char*>(&red), 1);
			file.write(reinterpret_cast<const char*>(&alpha), 1);
		}
	}

	file.close();
}

Renderer_Menu::Renderer_Menu(Kerzenlicht_Renderer* P_Parent) : QT_Linear_Contents(true) {
	Parent = P_Parent;

	Vertex_Colors_Obj_Import = false;
	Textured_Obj_Import = false;
	Normals_Obj_Import = false;

	Obj_Vertex_Colors = new QCheckBox("Import Obj Vertex Colors");
	connect(Obj_Vertex_Colors, &QCheckBox::stateChanged, [this](int State) {Vertex_Colors_Obj_Import = State; });

	Obj_Textured = new QCheckBox("Import Obj Textured");
	connect(Obj_Textured, &QCheckBox::stateChanged, [this](int State) {Textured_Obj_Import = State; });

	Obj_Normals = new QCheckBox("Import Obj Normals");
	connect(Obj_Normals, &QCheckBox::stateChanged, [this](int State) {Normals_Obj_Import = State; });

	QT_Button* Load_File_Button = new QT_Button();
	Load_File_Button->setText("Import Obj File");
	connect(Load_File_Button, &QT_Button::clicked, this, &Renderer_Menu::openObjFile);

	QT_Button* Clear_Button = new QT_Button();
	Clear_Button->setText("Clear Scene");
	connect(Clear_Button, &QT_Button::clicked, this, &Renderer_Menu::clearScene);

	QT_Button* Render_Wire_Button = new QT_Button();
	Render_Wire_Button->setText("Render Wireframe");
	connect(Render_Wire_Button, &QT_Button::clicked, this, &Renderer_Menu::renderWireframe);

	QT_Button* Render_Points_Button = new QT_Button();
	Render_Points_Button->setText("Render Points");
	connect(Render_Points_Button, &QT_Button::clicked, this, &Renderer_Menu::renderPointCloud);

	QT_Button* Render_Visualizer_Button = new QT_Button();
	Render_Visualizer_Button->setText("Render Visualizer");
	connect(Render_Visualizer_Button, &QT_Button::clicked, this, &Renderer_Menu::renderEdgeVisualizer);

	QT_Value_Input* ResX_Input = new QT_Value_Input();
	QIntValidator* ValidatorX = new QIntValidator();
	ResX_Input->setValidator(ValidatorX);
	ResX_Input->setText(QString::fromStdString(std::to_string(Parent->ResX)));
	connect(ResX_Input, &QT_Value_Input::textChanged, [this](QString text) {changeXResolution(text.toInt()); });

	QT_Value_Input* ResY_Input = new QT_Value_Input();
	QIntValidator* ValidatorY = new QIntValidator();
	ResY_Input->setValidator(ValidatorY);
	ResY_Input->setText(QString::fromStdString(std::to_string(Parent->ResY)));
	connect(ResY_Input, &QT_Value_Input::textChanged, [this](QString text) {changeYResolution(text.toInt()); });

	QT_Button* Save_Button = new QT_Button;
	Save_Button->setText("Save to .Bmp");
	connect(Save_Button, &QT_Button::clicked, this, &Renderer_Menu::save);

	Layout->addWidget(Obj_Vertex_Colors);
	Layout->addWidget(Obj_Textured);
	Layout->addWidget(Obj_Normals);
	Layout->addWidget(Load_File_Button);
	Layout->addWidget(Clear_Button);
	Layout->addWidget(Render_Wire_Button);
	Layout->addWidget(Render_Points_Button);
	Layout->addWidget(Render_Visualizer_Button);
	Layout->addWidget(ResX_Input);
	Layout->addWidget(ResY_Input);
	Layout->addWidget(Save_Button);
}

void Renderer_Menu::openObjFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Object (*.obj)"));
	if (!fileName.isEmpty()) {
		std::string File_Path = fileName.toStdString();

		Parent->createObject(File_Path);

		Parent->loadObj(File_Path, Vertex_Colors_Obj_Import, Textured_Obj_Import, Normals_Obj_Import);
		Parent->loadModel(File_Path);
		Parent->Object_Array[File_Path].loadBuffers();
		Parent->clearBuffers();
	}
}

void Renderer_Menu::renderWireframe() {
	Parent->View_Mode = Render_Mode::WIREFRAME;
	Parent->renderClear();
	Parent->renderWireframe();
	Parent->drawToSurface();
}
void Renderer_Menu::renderPointCloud() {
	Parent->View_Mode = Render_Mode::POINTCLOUD;
	Parent->renderClear();
	Parent->renderPointCloud();
	Parent->drawToSurface();
}
void Renderer_Menu::renderEdgeVisualizer() {
	Parent->View_Mode = Render_Mode::VISUALIZER;
	Parent->renderClear();
	Parent->renderEdgeVisualizer();
	Parent->drawToSurface();
}

void Renderer_Menu::changeXResolution(int value) {
	Parent->ResX = value;
	QSettings("Raylight", "KerzenLicht").setValue("ResX", value);
	Parent->Aspect_Ratio = static_cast<double>(Parent->ResX) / static_cast<double>(Parent->ResY);
	Parent->Pixmap = std::vector(Parent->ResX, std::vector<Rgba>(Parent->ResY));
	Parent->renderClear();
	Parent->renderFrame();
}

void Renderer_Menu::changeYResolution(int value) {
	Parent->ResY = value;
	Parent->Aspect_Ratio = static_cast<double>(Parent->ResX) / static_cast<double>(Parent->ResY);
	QSettings("Raylight", "KerzenLicht").setValue("ResY", value);
	Parent->Pixmap = std::vector(Parent->ResX, std::vector<Rgba>(Parent->ResY));
	Parent->renderClear();
	Parent->renderFrame();
}

void Renderer_Menu::save() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Create New File"), "", tr("BitMap (*.bmp)"));
	if (!fileName.isEmpty()) {
		std::ofstream log;
		log.open(fileName.toStdString());
		log.close();
		Parent->storeBmp(fileName.toStdString());
	}
}
void Renderer_Menu::clearScene() {
	Parent->Object_Array = std::map<std::string, Object>();
	Parent->renderClear();
	Parent->drawToSurface();
}