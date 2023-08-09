#include "Kerzenlicht_Renderer.h"

Kerzenlicht_Renderer::Kerzenlicht_Renderer(QT_Text_Stream* P_Log) : QT_Graphics_View() {
	Log = P_Log;

	Left_Mouse_Pressed = false;
	Right_Mouse_Pressed = false;
	Mouse_Down_Pos = QPoint(0, 0);

	QSettings settings("Raylight", "KerzenLicht");
	
	Render_Scene = vector<Object>();
	Render_Camera = Camera();
	Render_Camera.f_processMatrix();

	Pen_Color = Rgba();
	Pen_Opacity = 1.0f;
	Pixmap = vector(Render_Camera.x_resolution, vector<Rgba>(Render_Camera.y_resolution));
	ZBuffer = vector(Render_Camera.x_resolution, vector<double>(Render_Camera.y_resolution));

	View_Mode = static_cast<Render_Mode>(settings.value("Render_Mode", Render_Mode::PREVIEW).toInt());

	Thread_Storage = vector<QThread*>(); // for obj loading

	Scene = new QGraphicsScene();
	setScene(Scene);
	Menu = new Renderer_Menu(this);

	///////////
	// Scene //
	///////////
	renderClear();
	drawToSurface();
	loadObj("./Mika.obj");
}

void Kerzenlicht_Renderer::drawToSurface() {
	Scene->clear();
	QImage image(Render_Camera.x_resolution, Render_Camera.y_resolution, QImage::Format_RGBA8888);

	for (int x = 0; x < Render_Camera.x_resolution; x++) {
		for (int y = 0; y < Render_Camera.y_resolution; y++) {
			Rgba& pixel = Pixmap[x][Render_Camera.y_resolution - y - 1];
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
	float Delta = P_Event->angleDelta().y()*0.025;
	Render_Camera.fov -= Delta;
	Render_Camera.f_processMatrix();
	renderFrame();
}

void Kerzenlicht_Renderer::mousePressEvent(QMouseEvent* P_Event) {
	if (P_Event->button() == Qt::LeftButton) Left_Mouse_Pressed = true;
	if (P_Event->button() == Qt::RightButton) Right_Mouse_Pressed = true;

	Mouse_Down_Pos = P_Event->pos();
}

void Kerzenlicht_Renderer::mouseMoveEvent(QMouseEvent* P_Event) {
	if (Right_Mouse_Pressed) {
		double DeltaX = P_Event->pos().x() - Mouse_Down_Pos.x();
		double DeltaY = P_Event->pos().y() - Mouse_Down_Pos.y();
		Render_Camera.rotation += Vec3(DeltaX * 0.005, DeltaY * 0.005, 0);
		Render_Camera.f_processMatrix();
		renderFrame();
	}
}

void Kerzenlicht_Renderer::mouseReleaseEvent(QMouseEvent* P_Event) {
	if (P_Event->button() == Qt::LeftButton) Left_Mouse_Pressed = false;
	if (P_Event->button() == Qt::RightButton) Right_Mouse_Pressed = false;
}

void Kerzenlicht_Renderer::keyPressEvent(QKeyEvent* P_Event) {
	if (P_Event->key() == Qt::Key::Key_W) {
		Render_Camera.f_moveForward(-0.25);
	}
	if (P_Event->key() == Qt::Key::Key_A) {
		Render_Camera.f_moveRight(-0.25);
	}
	if (P_Event->key() == Qt::Key::Key_S) {
		Render_Camera.f_moveForward(0.25);
	}
	if (P_Event->key() == Qt::Key::Key_D) {
		Render_Camera.f_moveRight(0.25);
	}
	if (P_Event->key() == Qt::Key::Key_E) {
		Render_Camera.f_moveUp(0.25);
	}
	if (P_Event->key() == Qt::Key::Key_Q) {
		Render_Camera.f_moveUp(-0.25);
	}
	if (P_Event->key() == Qt::Key::Key_Right) {
		Render_Camera.rotation += Vec3(-1, 0, 0);
	}
	if (P_Event->key() == Qt::Key::Key_Left) {
		Render_Camera.rotation += Vec3(1, 0, 0);
	}
	if (P_Event->key() == Qt::Key::Key_Up) {
		Render_Camera.rotation += Vec3(0, 1, 0);
	}
	if (P_Event->key() == Qt::Key::Key_Down) {
		Render_Camera.rotation += Vec3(0, -1, 0);
	}
	if (P_Event->key() == Qt::Key::Key_Comma) {
		Render_Camera.rotation += Vec3(0, 0, 2.5);
	}
	if (P_Event->key() == Qt::Key::Key_Period) {
		Render_Camera.rotation += Vec3(0, 0, -2.5);
	}
	Render_Camera.f_processMatrix();
	renderFrame();
}

void Kerzenlicht_Renderer::resizeEvent(QResizeEvent* P_Event) {
	centerOn(scene()->items()[0]->boundingRect().center());
	fitInView(scene()->items()[0]->boundingRect(), Qt::KeepAspectRatio);
}

void Kerzenlicht_Renderer::closeEvent(QCloseEvent* P_Event) {
	P_Event->accept();
}

Rgba Kerzenlicht_Renderer::calculatePixelColor(const Ray& ray) const {
	//vector<LightPath> all_light_paths = TraceLightRays(8); // Light Bounces
	return Rgba();//radiance(ray, all_light_paths);
}

void Kerzenlicht_Renderer::updateProgress(int P_Progress) {
	Menu->Progress->setValue(P_Progress);
}

void Kerzenlicht_Renderer::loadObject(Object P_Object) {
	Object Import_Obj = P_Object;
	Import_Obj.Pos = Vec3(0, 0, 0);
	Import_Obj.Scale = Vec3(1, 1, 1);
	Import_Obj.Rot_Euler = Vec3(0, 0, 0);
	Import_Obj.MeshShader.Albedo.loadfromBitmap("./Mika.bmp");

	Render_Scene.push_back(Import_Obj);
	renderFrame();
}

void Kerzenlicht_Renderer::setPenColor(Rgba P_Color) {
	Pen_Color = P_Color;
}

void Kerzenlicht_Renderer::setPenOpacity(float P_Opacity) {
	Pen_Opacity = P_Opacity;
}

void Kerzenlicht_Renderer::renderClear() {
	ZBuffer = vector(Render_Camera.x_resolution, vector(Render_Camera.y_resolution, Render_Camera.far_clip));
	Pixmap = vector(Render_Camera.x_resolution, vector(Render_Camera.y_resolution, Rgba(0.1, 0.1, 0.1, 1)));
}

void Kerzenlicht_Renderer::renderPixel(uint32_t P_X, uint32_t P_Y) {
	if (P_X < Render_Camera.x_resolution && P_X >= 0 && P_Y < Render_Camera.y_resolution && P_Y >= 0) {
		Pixmap[P_X][P_Y] = Pen_Color;
	}
}

void Kerzenlicht_Renderer::renderPixel(uint32_t P_X, uint32_t P_Y, Rgba P_Color) {
	if (P_X < Render_Camera.x_resolution && P_X >= 0 && P_Y < Render_Camera.y_resolution && P_Y >= 0) {
		Pixmap[P_X][P_Y] = P_Color;
	}
}

void Kerzenlicht_Renderer::renderLine(int P_Start_X, int P_Start_Y, int P_End_X, int P_End_Y) {
	int dx = abs(P_End_X - P_Start_X);
	int dy = abs(P_End_Y - P_Start_Y);
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

void Kerzenlicht_Renderer::render2DPoly(vector<pair<int,int>> P_Poly) {

	int minY = P_Poly[0].second;
	int maxY = P_Poly[0].second;

	for (const auto& point : P_Poly) {
		minY = min(minY, point.second);
		maxY = max(maxY, point.second);
	}

	// Scanline fill
	for (int y = minY; y <= maxY; y++) {
		vector<int> intersectX;

		for (size_t i = 0; i < P_Poly.size(); i++) {
			size_t nextIndex = (i + 1) % P_Poly.size();
			int P_Start_Y = P_Poly[i].second;
			int P_End_Y = P_Poly[nextIndex].second;

			if ((P_Start_Y <= y && P_End_Y > y) || (P_End_Y <= y && P_Start_Y > y)) {
				int P_Start_X = P_Poly[i].first;
				int P_End_X = P_Poly[nextIndex].first;

				int intersectXVal = P_Start_X + (y - P_Start_Y) * (P_End_X - P_Start_X) / (P_End_Y - P_Start_Y);
				intersectX.push_back(intersectXVal);
			}
		}

		sort(intersectX.begin(), intersectX.end());

		for (size_t i = 0; i < intersectX.size(); i += 2) {
			int P_Start_X = intersectX[i];
			int P_End_X = intersectX[i + 1];
			renderLine(P_Start_X, y, P_End_X, y);
		}
	}
}

void Kerzenlicht_Renderer::renderTriangle(Vertex P_Vert1, Vertex P_Vert2, Vertex P_Vert3) {
	int minX = min({ P_Vert1.Pos.X, P_Vert2.Pos.X, P_Vert3.Pos.X });
	int maxX = max({ P_Vert1.Pos.X, P_Vert2.Pos.X, P_Vert3.Pos.X }) + 1;
	int minY = min({ P_Vert1.Pos.Y, P_Vert2.Pos.Y, P_Vert3.Pos.Y });
	int maxY = max({ P_Vert1.Pos.Y, P_Vert2.Pos.Y, P_Vert3.Pos.Y }) + 1;

	for (int x = minX; x < maxX; x++) {
		for (int y = minY; y < maxY; y++) {
			if (x >= 0 && x < Render_Camera.x_resolution && y >= 0 && y < Render_Camera.y_resolution) {
				double u, v, w;
				tie(u, v, w) = barycentricCoords(P_Vert1.Pos, P_Vert2.Pos, P_Vert3.Pos, x, y);
				if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && w >= 0.0 && w <= 1.0) {
					double Depth = u * P_Vert1.Pos.Z + v * P_Vert2.Pos.Z + w * P_Vert3.Pos.Z;
					if (Depth < ZBuffer[x][y]) {
						ZBuffer[x][y] = Depth;
						renderPixel(x, y, Rgba(
							P_Vert1.Color * u + P_Vert2.Color * v + P_Vert3.Color * w,
							1.0
						));
					}
				}
			}
		}
	}
}

tuple<double, double, double> Kerzenlicht_Renderer::barycentricCoords(const Vec3& P_A, const Vec3& P_B, const Vec3& P_C, double P_X, double P_Y) {
	double AreaPBC = (P_B.Y - P_C.Y) * (P_X   - P_C.X) + (P_C.X - P_B.X) * (P_Y   - P_C.Y);
	double AreaACP = (P_C.Y - P_A.Y) * (P_X   - P_C.X) + (P_A.X - P_C.X) * (P_Y   - P_C.Y);
	double AreaABC = (P_B.Y - P_C.Y) * (P_A.X - P_C.X) + (P_C.X - P_B.X) * (P_A.Y - P_C.Y);
	
	double u = AreaPBC / AreaABC;
	double v = AreaACP / AreaABC;
	double w = 1.0 - u - v;

	return make_tuple(u,v,w);
}

void Kerzenlicht_Renderer::loadObj(string P_File) {
	Obj_File_Loader* thread = new Obj_File_Loader(this, P_File);
	
	connect(thread, SIGNAL( loadingFinished_Signal(Object) ), SLOT( loadObject(Object) ));
	connect(thread, SIGNAL( updateProgress_Signal(int) ), SLOT( updateProgress(int) ));
	connect(thread, &Obj_File_Loader::finished, thread, &Obj_File_Loader::deleteLater);

	thread->start();
}

void Kerzenlicht_Renderer::renderWireframe() {
	setPenColor(Rgba(1, 1, 1, 1));
	for (Object& Obj : Render_Scene) {
		Obj.MeshData.f_processModelMatrix(Obj.Pos, Obj.Rot_Euler, Obj.Scale);
		Obj.MeshData.f_processVertexShader(Render_Camera.camera_matrix, Render_Camera.projection_matrix, Render_Camera.viewport_matrix);
		for (const Mesh_Triangle& tri : Obj.MeshData.Faces) {
			Vec3 v1 = Obj.MeshData.Vertex_Output[tri.Index1].Pos;
			Vec3 v2 = Obj.MeshData.Vertex_Output[tri.Index2].Pos;
			Vec3 v3 = Obj.MeshData.Vertex_Output[tri.Index3].Pos;

			renderLine(v1.X, v1.Y, v2.X, v2.Y);
			renderLine(v2.X, v2.Y, v3.X, v3.Y);
			renderLine(v1.X, v1.Y, v3.X, v3.Y);
		}
	}
}

void Kerzenlicht_Renderer::renderPreview() {
	for (Object& Obj : Render_Scene) {
		Obj.MeshData.f_processModelMatrix(Obj.Pos, Obj.Rot_Euler, Obj.Scale);
		Obj.MeshData.f_processVertexShader(Render_Camera.camera_matrix, Render_Camera.projection_matrix, Render_Camera.viewport_matrix);
		for (const Mesh_Triangle& tri : Obj.MeshData.Faces) {
			Vertex v1 = Obj.MeshData.Vertex_Output[tri.Index1];
			Vertex v2 = Obj.MeshData.Vertex_Output[tri.Index2];
			Vertex v3 = Obj.MeshData.Vertex_Output[tri.Index3];
			renderTriangle(v1, v2, v3);
		}
	}
}

void Kerzenlicht_Renderer::renderZBuffer() {
	for (Object& Obj : Render_Scene) {
		Obj.MeshData.f_processModelMatrix(Obj.Pos, Obj.Rot_Euler, Obj.Scale);
		Obj.MeshData.f_processVertexShader(Render_Camera.camera_matrix, Render_Camera.projection_matrix, Render_Camera.viewport_matrix);
		vector<double> Z_Positions;
		for (const Vertex& vert : Obj.MeshData.Vertex_Output) {
			Z_Positions.push_back(vert.Pos.Z);
		}
		double minZ = *min_element(Z_Positions.begin(), Z_Positions.end());
		double maxZ = *max_element(Z_Positions.begin(), Z_Positions.end());

		for (const Mesh_Triangle& tri : Obj.MeshData.Faces) {
			Vertex v1 = Obj.MeshData.Vertex_Output[tri.Index1];
			Vertex v2 = Obj.MeshData.Vertex_Output[tri.Index2];
			Vertex v3 = Obj.MeshData.Vertex_Output[tri.Index3];

			int minX = min({ v1.Pos.X, v2.Pos.X, v3.Pos.X });
			int maxX = max({ v1.Pos.X, v2.Pos.X, v3.Pos.X }) + 1;
			int minY = min({ v1.Pos.Y, v2.Pos.Y, v3.Pos.Y });
			int maxY = max({ v1.Pos.Y, v2.Pos.Y, v3.Pos.Y }) + 1;

			for (int x = minX; x < maxX; x++) {
				for (int y = minY; y < maxY; y++) {
					if (x >= 0 && x < Render_Camera.x_resolution && y >= 0 && y < Render_Camera.y_resolution) {
						double u, v, w;
						tie(u, v, w) = barycentricCoords(v1.Pos, v2.Pos, v3.Pos, x, y);
						if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && w >= 0.0 && w <= 1.0) {
							double Depth = u * v1.Pos.Z + v * v2.Pos.Z + w * v3.Pos.Z;
							if (Depth < ZBuffer[x][y]) {
								ZBuffer[x][y] = Depth;
								Rgb Val = Rgb(Math::clamp(-(Depth - maxZ) / (maxZ - minZ), 0.0, 1.0));
								renderPixel(x, y, Rgba(
									Val,
									1.0
								));
							}
						}
					}
				}
			}
		}
	}
}

void Kerzenlicht_Renderer::renderTextured() {
	for (Object& Obj : Render_Scene) {
		if (Obj.MeshShader.Albedo.Width != 0) {
			Obj.MeshData.f_processModelMatrix(Obj.Pos, Obj.Rot_Euler, Obj.Scale);
			Obj.MeshData.f_processVertexShader(Render_Camera.camera_matrix, Render_Camera.projection_matrix, Render_Camera.viewport_matrix);
			for (const Mesh_Triangle& tri : Obj.MeshData.Faces) {
				Vertex v1 = Obj.MeshData.Vertex_Output[tri.Index1];
				Vertex v2 = Obj.MeshData.Vertex_Output[tri.Index2];
				Vertex v3 = Obj.MeshData.Vertex_Output[tri.Index3];

				int minX = min({ v1.Pos.X, v2.Pos.X, v3.Pos.X });
				int maxX = max({ v1.Pos.X, v2.Pos.X, v3.Pos.X }) + 1;
				int minY = min({ v1.Pos.Y, v2.Pos.Y, v3.Pos.Y });
				int maxY = max({ v1.Pos.Y, v2.Pos.Y, v3.Pos.Y }) + 1;

				for (int x = minX; x < maxX; x++) {
					for (int y = minY; y < maxY; y++) {
						if (x >= 0 && x < Render_Camera.x_resolution && y >= 0 && y < Render_Camera.y_resolution) {
							double u, v, w;
							tie(u, v, w) = barycentricCoords(v1.Pos, v2.Pos, v3.Pos, x, y);
							if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && w >= 0.0 && w <= 1.0) {
								double Depth = u * v1.Pos.Z + v * v2.Pos.Z + w * v3.Pos.Z;
								if (Depth < ZBuffer[x][y]) {
									ZBuffer[x][y] = Depth;
									Vec2 UVs = Vec2(
										u * v1.UV.X + v * v2.UV.X + w * v3.UV.X,
										u * v1.UV.Y + v * v2.UV.Y + w * v3.UV.Y
									);
									renderPixel(x, y, Obj.MeshShader.Albedo.getColor(UVs));
								}
							}
						}
					}
				}
			}
		}
	}
}

void Kerzenlicht_Renderer::renderPathTracer() {
}

void Kerzenlicht_Renderer::renderPointCloud() {
	setPenColor(Rgba(1, 1, 1, 1));
	for (Object& Obj : Render_Scene) {
		Obj.MeshData.f_processModelMatrix(Obj.Pos, Obj.Rot_Euler, Obj.Scale);
		Obj.MeshData.f_processVertexShader(Render_Camera.camera_matrix, Render_Camera.projection_matrix, Render_Camera.viewport_matrix);
		for (const Mesh_Triangle& tri : Obj.MeshData.Faces) {
			Vertex v1 = Obj.MeshData.Vertex_Output[tri.Index1];
			Vertex v2 = Obj.MeshData.Vertex_Output[tri.Index2];
			Vertex v3 = Obj.MeshData.Vertex_Output[tri.Index3];

			if (Obj.MeshData.Vertex_Colors.size() > 0) {
				setPenColor(Rgba::fromRgb(v1.Color));
				renderPixel(v1.Pos.X, v1.Pos.Y);

				setPenColor(Rgba::fromRgb(v2.Color));
				renderPixel(v2.Pos.X, v2.Pos.Y);

				setPenColor(Rgba::fromRgb(v3.Color));
				renderPixel(v3.Pos.X, v3.Pos.Y);
			}
			else {
				renderPixel(v1.Pos.X, v1.Pos.Y);
				renderPixel(v2.Pos.X, v2.Pos.Y);
				renderPixel(v3.Pos.X, v3.Pos.Y);
			}
		}
	}
}

void Kerzenlicht_Renderer::renderFrame() {
	renderClear();
	if (View_Mode == Render_Mode::POINTCLOUD) {
		renderPointCloud();
	}
	else if (View_Mode == Render_Mode::PREVIEW) {
		renderPreview();
	}
	else if (View_Mode == Render_Mode::TEXTURED) {
		renderTextured();
	}
	else if (View_Mode == Render_Mode::WIREFRAME) {
		renderWireframe();
	}
	else if (View_Mode == Render_Mode::ZDEPTH) {
		renderZBuffer();
	}
	else {
		renderPreview();
	}
	drawToSurface();
}

void Kerzenlicht_Renderer::storeBmp(string P_File) {
	ofstream file(P_File, ios::binary);
	if (!file.is_open()) {
		return;
	}

	uint32_t filesize = Render_Camera.x_resolution * Render_Camera.y_resolution * 4;

	// BMP header (14 bytes)
	file.write("BM", 2);													// Signature
	file.write(reinterpret_cast<const char*>(&filesize), 4);				// File size
	file.write("\0\0\0\0", 4);												// Reserved
	uint32_t pixelDataOffset = 54;
	file.write(reinterpret_cast<const char*>(&pixelDataOffset), 4);			// Pixel data offset

	// DIB header (40 bytes)
	uint32_t dibHeaderSize = 40;
	file.write(reinterpret_cast<const char*>(&dibHeaderSize), 4);			// DIB header size
	file.write(reinterpret_cast<const char*>(&Render_Camera.x_resolution), 4);					// Image width
	file.write(reinterpret_cast<const char*>(&Render_Camera.y_resolution), 4);					// Image height
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
	for (size_t y = 0; y < Render_Camera.y_resolution; y++) {
		for (size_t x = 0; x < Render_Camera.x_resolution; x++) {
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

	QT_Option* Render_Mode_Select = new QT_Option();

	Render_Mode_Select->addItem("Path Tracer", 0);
	Render_Mode_Select->addItem("Point Cloud", 1);
	Render_Mode_Select->addItem("Preview", 2);
	Render_Mode_Select->addItem("Textured", 3);
	Render_Mode_Select->addItem("Wireframe", 4);
	Render_Mode_Select->addItem("Z-Depth Debug", 5);

	Render_Mode_Select->setCurrentIndex(Parent->View_Mode);

	connect(Render_Mode_Select, &QT_Option::currentIndexChanged, [this](int Index) { renderSwitch(Index); });

	QT_Value_Input* ResX = new QT_Value_Input();
	QIntValidator* ValidatorX = new QIntValidator();
	ResX->setValidator(ValidatorX);
	ResX->setText(QString::fromStdString(to_string(Parent->Render_Camera.x_resolution)));
	connect(ResX, &QT_Value_Input::returnPressed, [this, ResX]() {changeXResolution(ResX->text().toInt()); });

	QT_Value_Input* ResY = new QT_Value_Input();
	QIntValidator* ValidatorY = new QIntValidator();
	ResY->setValidator(ValidatorY);
	ResY->setText(QString::fromStdString(to_string(Parent->Render_Camera.y_resolution)));
	connect(ResY, &QT_Value_Input::returnPressed, [this, ResY]() {changeYResolution(ResY->text().toInt()); });

	QT_Button* Save_Button = new QT_Button;
	Save_Button->setText("Save to .Bmp");
	connect(Save_Button, &QT_Button::clicked, this, &Renderer_Menu::save);

	Layout->addWidget(Load_File_Button);
	Layout->addWidget(Render_Mode_Select);
	Layout->addWidget(ResX);
	Layout->addWidget(ResY);
	Layout->addWidget(Save_Button);
	Layout->addWidget(Progress);
}

void Renderer_Menu::openObjFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Object (*.obj)"));
	if (!fileName.isEmpty()) Parent->loadObj(fileName.toStdString());
}

void Renderer_Menu::renderSwitch(int Index) {
	Parent->View_Mode = static_cast<Render_Mode>(Index);
	QSettings("Raylight", "KerzenLicht").setValue("Render_Mode", static_cast<Render_Mode>(Index));
	Parent->renderFrame();
}

void Renderer_Menu::changeXResolution(int value) {
	Parent->Render_Camera.x_resolution = value;
	Parent->Render_Camera.f_processMatrix();
	Parent->renderClear();
	QSettings("Raylight", "KerzenLicht").setValue("Render_Camera.x_resolution", value);
	Parent->renderFrame();
}

void Renderer_Menu::changeYResolution(int value) {
	Parent->Render_Camera.y_resolution = value;
	Parent->Render_Camera.f_processMatrix();
	Parent->renderClear();
	QSettings("Raylight", "KerzenLicht").setValue("Render_Camera.y_resolution", value);
	Parent->renderFrame();
}

void Renderer_Menu::save() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Create New File"), "", tr("BitMap (*.bmp)"));
	Parent->storeBmp(fileName.toStdString());
}