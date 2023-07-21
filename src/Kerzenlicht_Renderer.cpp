#include "Kerzenlicht_Renderer.h"

Kerzenlicht_Renderer::Kerzenlicht_Renderer(QT_Text_Stream* P_Log) : QT_Graphics_View() {
	Log = P_Log;

	Left_Mouse_Pressed = false;
	Right_Mouse_Pressed = false;
	Mouse_Down_Pos = QPoint(0, 0);

	QSettings settings("Raylight", "KerzenLicht");

	ResX = settings.value("ResX", 1800).toInt();
	ResY = settings.value("ResY", 1120).toInt();
	Aspect_Ratio = static_cast<double>(ResX) / static_cast<double>(ResY);
	Pen_Color = Rgba();
	Pen_Opacity = 1.0f;
	Pixmap = std::vector(ResX, std::vector<Rgba>(ResY));

	View_Mode = static_cast<Render_Mode>(settings.value("Render_Mode", Render_Mode::POINTCLOUD).toInt());

	Thread_Storage = std::vector<QThread*>();
	Object_Array = std::map<std::string, Object>();

	Scene = new QGraphicsScene();
	setScene(Scene);
	Menu = new Renderer_Menu(this);

	std::stringstream log;
	log << "Renderer Settings" << std::endl;
	log << "Res X: " << Pixmap[0].size() << std::endl;
	log << "Res Y: " << Pixmap.size() << std::endl;
	Log->append(QString::fromStdString(log.str()));

	///////////
	// Scene //
	///////////
	//renderClear();
	//drawToSurface();
	//loadObj("./Monke.obj");
	renderClear();
	render2DPoly({
		std::pair<int,int>(165, 380),
		std::pair<int,int>(185, 360),
		std::pair<int,int>(180, 330),
		std::pair<int,int>(207, 345),
		std::pair<int,int>(233, 330),
		std::pair<int,int>(230, 360),
		std::pair<int,int>(250, 380),
		std::pair<int,int>(220, 385),
		std::pair<int,int>(205, 410),
		std::pair<int,int>(193, 383) 
	});
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
	if (P_Event->button() == Qt::LeftButton) {
		Left_Mouse_Pressed = true;
	}
	if (P_Event->button() == Qt::RightButton) {
		Right_Mouse_Pressed = true;
	}
	Mouse_Down_Pos = P_Event->pos();
}

void Kerzenlicht_Renderer::mouseMoveEvent(QMouseEvent* P_Event) {
	if (Left_Mouse_Pressed) {
		for (std::pair<const std::string, Object>& Obj : Object_Array) {
			Obj.second.rotate(Vec3((P_Event->pos().y() - Mouse_Down_Pos.y()) * 0.001, (P_Event->pos().x() - Mouse_Down_Pos.x()) * 0.001, 0));
			Obj.second.processTransform();
			renderFrame();
		}
	}
	if (Right_Mouse_Pressed) {
		double DeltaY = P_Event->pos().y() - Mouse_Down_Pos.y();
		double DeltaX = P_Event->pos().x() - Mouse_Down_Pos.x();
		for (std::pair<const std::string, Object>& Obj : Object_Array) {
			Obj.second.translate(Vec3(DeltaX * 0.00025, DeltaY * -0.00025, 0));
			Obj.second.processTransform();
			renderFrame();
		}
	}
}

void Kerzenlicht_Renderer::mouseReleaseEvent(QMouseEvent* P_Event) {
	if (P_Event->button() == Qt::LeftButton) {
		Left_Mouse_Pressed = false;
	}
	if (P_Event->button() == Qt::RightButton) {
		Right_Mouse_Pressed = false;
	}
}

void Kerzenlicht_Renderer::resizeEvent(QResizeEvent* P_Event) {
	centerOn(scene()->items()[0]->boundingRect().center());
	fitInView(scene()->items()[0]->boundingRect(), Qt::KeepAspectRatio);
}

void Kerzenlicht_Renderer::closeEvent(QCloseEvent* P_Event) {
	P_Event->accept();
}

void Kerzenlicht_Renderer::updateProgress(int P_Progress) {
	Menu->Progress->setValue(P_Progress);
}

void Kerzenlicht_Renderer::loadObject(Object P_Object) {
	Object_Array["Imported_Obj"] = P_Object;
	renderFrame();
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

void Kerzenlicht_Renderer::render2DPoly(std::vector<std::pair<int,int>> P_Poly) {
	std::vector<std::pair<int, int>> points = P_Poly;
	for (std::pair<int, int> &Point : points) {
		Point.first -= 150;
		Point.second -= 300;
		Point.first *= 3;
		Point.second *= 3;
	}

	int minY = points[0].second;
	int maxY = points[0].second;

	for (const auto& point : points) {
		minY = std::min(minY, point.second);
		maxY = std::max(maxY, point.second);
	}

	// Scanline filling
	for (int y = minY; y <= maxY; y++) {
		std::vector<int> intersectX;

		for (size_t i = 0; i < points.size(); i++) {
			size_t nextIndex = (i + 1) % points.size();
			int P_Start_Y = points[i].second;
			int P_End_Y = points[nextIndex].second;

			if ((P_Start_Y <= y && P_End_Y > y) || (P_End_Y <= y && P_Start_Y > y)) {
				int P_Start_X = points[i].first;
				int P_End_X = points[nextIndex].first;

				int intersectXVal = P_Start_X + (y - P_Start_Y) * (P_End_X - P_Start_X) / (P_End_Y - P_Start_Y);
				intersectX.push_back(intersectXVal);
			}
		}

		std::sort(intersectX.begin(), intersectX.end());

		// Draw the scanline
		for (size_t i = 0; i < intersectX.size(); i += 2) {
			int P_Start_X = intersectX[i];
			int P_End_X = intersectX[i + 1];
			renderLine(P_Start_X, y, P_End_X, y);
		}
	}
}

void Kerzenlicht_Renderer::loadObj(std::string P_File) {
	R_String log;
	log << "Loading Obj Model, File: " << P_File << ".";
	Log->append(QString::fromStdString(log.write()));

	Obj_File_Loader* thread = new Obj_File_Loader(this, P_File);
	
	connect(thread, SIGNAL( loadingFinished_Signal(Object) ), SLOT( loadObject(Object) ));
	connect(thread, SIGNAL( updateProgress_Signal(int) ), SLOT( updateProgress(int) ));
	connect(thread, &Obj_File_Loader::finished, thread, &Obj_File_Loader::deleteLater);
	/*Obj_File_Loader::connect(objLoader, &Obj_File_Loader::loadingFinished, [objLoader, thread, this]() {
		objLoader->deleteLater();
		thread->quit();
		thread->wait();
		thread->deleteLater();
		Thread_Storage.erase(std::remove(Thread_Storage.begin(), Thread_Storage.end(), thread), Thread_Storage.end());
		}
	);*/ // TODO Threads can be destroyed while creatingObject after finished if file is too large
	thread->start();
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

	Progress = new QProgressBar();
	Progress->setValue(0);
	Progress->setMaximum(100);
	Progress->setTextVisible(true);
	Progress->setVisible(true);

	QT_Button* Load_File_Button = new QT_Button();
	Load_File_Button->setText("Import Obj File");
	connect(Load_File_Button, &QT_Button::clicked, this, &Renderer_Menu::openObjFile);

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

	Layout->addWidget(Load_File_Button);
	Layout->addWidget(Render_Wire_Button);
	Layout->addWidget(Render_Points_Button);
	Layout->addWidget(Render_Visualizer_Button);
	Layout->addWidget(ResX_Input);
	Layout->addWidget(ResY_Input);
	Layout->addWidget(Save_Button);
	Layout->addWidget(Progress);
}

void Renderer_Menu::openObjFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Object (*.obj)"));
	Parent->loadObj(fileName.toStdString());
}

void Renderer_Menu::renderWireframe() {
	Parent->View_Mode = Render_Mode::WIREFRAME;
	Parent->renderClear();
	Parent->renderWireframe();
	Parent->drawToSurface();
	QSettings("Raylight", "KerzenLicht").setValue("Render_Mode", Render_Mode::WIREFRAME);
}
void Renderer_Menu::renderPointCloud() {
	Parent->View_Mode = Render_Mode::POINTCLOUD;
	Parent->renderClear();
	Parent->renderPointCloud();
	Parent->drawToSurface();
	QSettings("Raylight", "KerzenLicht").setValue("Render_Mode", Render_Mode::POINTCLOUD);
}
void Renderer_Menu::renderEdgeVisualizer() {
	Parent->View_Mode = Render_Mode::VISUALIZER;
	Parent->renderClear();
	Parent->renderEdgeVisualizer();
	Parent->drawToSurface();
	QSettings("Raylight", "KerzenLicht").setValue("Render_Mode", Render_Mode::VISUALIZER);
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
	Parent->storeBmp(fileName.toStdString());
}