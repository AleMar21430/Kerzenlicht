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
	Pixmap = vector(ResX, vector<Rgba>(ResY));
	ZBuffer = vector(ResX, vector<double>(ResY));

	View_Mode = static_cast<Render_Mode>(settings.value("Render_Mode", Render_Mode::PREVIEW).toInt());

	Thread_Storage = vector<QThread*>(); // for obj loading

	Render_Object = Object();
	Render_Camera = Camera();

	Scene = new QGraphicsScene();
	setScene(Scene);
	Menu = new Renderer_Menu(this);

	stringstream log;
	log << "Renderer Settings" << endl;
	log << "Res X: " << Pixmap[0].size() << endl;
	log << "Res Y: " << Pixmap.size() << endl;
	Log->append(QString::fromStdString(log.str()));

	///////////
	// Scene //
	///////////
	renderClear();
	drawToSurface();
	loadObj("./Koharu.obj");
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
	float Delta = P_Event->angleDelta().y()*0.05;
	Render_Object.scale(Vec3(Delta, Delta, Delta));
	renderFrame();
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
		Render_Object.rotate(Vec3(
			(P_Event->pos().x() - Mouse_Down_Pos.x()) * -0.001,
			(P_Event->pos().y() - Mouse_Down_Pos.y()) * -0.001,
			0)
		);
		renderFrame();
	}
	if (Right_Mouse_Pressed) {
		double DeltaX = P_Event->pos().x() - Mouse_Down_Pos.x();
		double DeltaY = P_Event->pos().y() - Mouse_Down_Pos.y();
		Render_Object.translate(Vec3(DeltaX * 0.05, DeltaY * -0.05, 0));
		renderFrame();
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

void Kerzenlicht_Renderer::keyPressEvent(QKeyEvent* P_Event) {
	if (P_Event->key() == Qt::Key::Key_W) {
		Render_Camera.f_moveForward(0.05);
	}
	if (P_Event->key() == Qt::Key::Key_A) {
		Render_Camera.f_moveRight(-0.05);
	}
	if (P_Event->key() == Qt::Key::Key_S) {
		Render_Camera.f_moveForward(-0.05);
	}
	if (P_Event->key() == Qt::Key::Key_D) {
		Render_Camera.f_moveRight(0.05);
	}
	if (P_Event->key() == Qt::Key::Key_E) {
		Render_Camera.f_moveUp(0.05);
	}
	if (P_Event->key() == Qt::Key::Key_Q) {
		Render_Camera.f_moveUp(-0.05);
	}
	if (P_Event->key() == Qt::Key::Key_Right) {
		Render_Camera.f_rotate(0, 0.05,0);
	}
	if (P_Event->key() == Qt::Key::Key_Left) {
		Render_Camera.f_rotate(0, -0.05, 0);
	}
	if (P_Event->key() == Qt::Key::Key_Up) {
		Render_Camera.f_rotate(0.05, 0, 0);
	}
	if (P_Event->key() == Qt::Key::Key_Down) {
		Render_Camera.f_rotate(-0.05, 0, 0);
	}
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
	Render_Object = P_Object;
	Render_Object.Pos = Vec3(0, 0, 0);
	Render_Object.Scale = Vec3(600, 600, 600);
	Render_Object.Rot_Euler = Vec3(180, 0, 0);

	Render_Object.translate(Vec3(ResX / 2.0, 100, 0));
	renderFrame();
}

void Kerzenlicht_Renderer::setPenColor(Rgba P_Color) {
	Pen_Color = P_Color;
}

void Kerzenlicht_Renderer::setPenOpacity(float P_Opacity) {
	Pen_Opacity = P_Opacity;
}

void Kerzenlicht_Renderer::renderClear() {
	ZBuffer = vector(ResX, vector(ResY, 1'000'000.0));
	Pixmap = vector(ResX, vector(ResY, Rgba(0.1, 0.1, 0.1, 1)));
}

void Kerzenlicht_Renderer::renderPixel(uint32_t P_X, uint32_t P_Y) {
	if (P_X < ResX && P_X >= 0 && P_Y < ResY && P_Y >= 0) {
		Pixmap[P_X][P_Y] = Pen_Color;
	}
}

void Kerzenlicht_Renderer::renderPixel(uint32_t P_X, uint32_t P_Y, Rgba P_Color) {
	if (P_X < ResX && P_X >= 0 && P_Y < ResY && P_Y >= 0) {
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
			if (x >= 0 && x < ResX && y >= 0 && y < ResY) {
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
	R_String log;
	log << "Loading Obj Model, File: " << P_File << ".";
	Log->append(QString::fromStdString(log.write()));

	Obj_File_Loader* thread = new Obj_File_Loader(this, P_File);
	
	connect(thread, SIGNAL( loadingFinished_Signal(Object) ), SLOT( loadObject(Object) ));
	connect(thread, SIGNAL( updateProgress_Signal(int) ), SLOT( updateProgress(int) ));
	connect(thread, &Obj_File_Loader::finished, thread, &Obj_File_Loader::deleteLater);

	thread->start();
}

void Kerzenlicht_Renderer::renderWireframe() {
	setPenColor(Rgba(1, 1, 1, 1));
	Render_Object.MeshData.applyTransformMatrix(Render_Object.Pos, Render_Object.Rot_Euler, Render_Object.Scale);
	for (const Mesh_Triangle& tri : Render_Object.MeshData.Faces) {
		Vec3 v1 = Render_Object.MeshData.Vertex_Output[tri.Index1].Pos;
		Vec3 v2 = Render_Object.MeshData.Vertex_Output[tri.Index2].Pos;
		Vec3 v3 = Render_Object.MeshData.Vertex_Output[tri.Index3].Pos;

		renderLine(v1.X, v1.Y, v2.X, v2.Y);
		renderLine(v2.X, v2.Y, v3.X, v3.Y);
		renderLine(v1.X, v1.Y, v3.X, v3.Y);
	}
}

void Kerzenlicht_Renderer::renderPreview() {
	Render_Object.MeshData.applyTransformMatrix(Render_Object.Pos, Render_Object.Rot_Euler, Render_Object.Scale);
	for (const Mesh_Triangle& tri : Render_Object.MeshData.Faces) {
		Vertex v1 = Render_Object.MeshData.Vertex_Output[tri.Index1];
		Vertex v2 = Render_Object.MeshData.Vertex_Output[tri.Index2];
		Vertex v3 = Render_Object.MeshData.Vertex_Output[tri.Index3];
		renderTriangle(v1, v2, v3);
	}
}

void Kerzenlicht_Renderer::renderZBuffer() {
	Render_Object.MeshData.applyTransformMatrix(Render_Object.Pos, Render_Object.Rot_Euler, Render_Object.Scale);
	vector<double> Z_Positions;
	for (const Vertex& vert : Render_Object.MeshData.Vertex_Output) {
		Z_Positions.push_back(vert.Pos.Z);
	}
	double minZ = *min_element(Z_Positions.begin(), Z_Positions.end());
	double maxZ = *max_element(Z_Positions.begin(), Z_Positions.end());

	for (const Mesh_Triangle& tri : Render_Object.MeshData.Faces) {
		Vertex v1 = Render_Object.MeshData.Vertex_Output[tri.Index1];
		Vertex v2 = Render_Object.MeshData.Vertex_Output[tri.Index2];
		Vertex v3 = Render_Object.MeshData.Vertex_Output[tri.Index3];

		int minX = min({ v1.Pos.X, v2.Pos.X, v3.Pos.X });
		int maxX = max({ v1.Pos.X, v2.Pos.X, v3.Pos.X }) + 1;
		int minY = min({ v1.Pos.Y, v2.Pos.Y, v3.Pos.Y });
		int maxY = max({ v1.Pos.Y, v2.Pos.Y, v3.Pos.Y }) + 1;

		for (int x = minX; x < maxX; x++) {
			for (int y = minY; y < maxY; y++) {
				if (x >= 0 && x < ResX && y >= 0 && y < ResY) {
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

void Kerzenlicht_Renderer::renderPathTracer() {
	Rgba accumulated_radiance = Rgba();
	Vec3 cx_ = Vec3(ResX * 0.5135 / ResY, 0, 0);
	Vec3 cy_ = (cx_.cross(Render_Camera.forward_vec)).normalize() * 0.5135;

	for (int x = 0; x < ResX; x++) {
		for (int y = 0; y < ResY; y++) {
			/* Compute radiance at subpixel using 8 samples */
			for (int s = 0; s < 8; s++) {
				const double r1 = 2.0 * (rand() / static_cast<double>(RAND_MAX) * 0.1);
				const double r2 = 2.0 * (rand() / static_cast<double>(RAND_MAX) * 0.1);
				/* Transform uniform into non-uniform filter samples */
				double dx;
				if (r1 < 1.0)
					dx = sqrt(r1) - 1.0;
				else
					dx = 1.0 - sqrt(2.0 - r1);
				double dy;
				if (r2 < 1.0)
					dy = sqrt(r2) - 1.0;
				else
					dy = 1.0 - sqrt(2.0 - r2);

				Vec3 dir = cx_ * ((x + (0.5 + dx) / 2.0) / ResX - 0.5) +
					cy_ * ((y + (0.5 + dy) / 2.0) / ResY - 0.5) +
					Render_Camera.forward_vec;

				/* Extend camera_ ray to start inside box */

				dir = dir.normalize();

				Ray ray = Ray(Render_Camera.position, dir);

				if (Render_Camera.depth_of_field) {
					//DoF
					double u1 = ((rand() / static_cast<double>(RAND_MAX) * 0.1) * 2.0) - 1.0;
					double u2 = ((rand() / static_cast<double>(RAND_MAX) * 0.1) * 2.0) - 1.0;

					double fac = (double)(2 * M_PI * u2);

					Vec3 offset = Vec3(u1 * cos(fac), u1 * sin(fac), 0.0) * Render_Camera.depth_of_field_f_stops;
					Vec3 focalPlaneIntersection = ray.org + ray.dir * (Render_Camera.Fov / Render_Camera.forward_vec.dot(ray.dir));
					ray.org = ray.org + offset;
					ray.dir = (focalPlaneIntersection - ray.org).normalize();

				}
				/* Accumulate radiance */
				//accumulated_radiance = accumulated_radiance + calculatePixelColor(ray) / 8;// / dof_samples;
			}

			accumulated_radiance = accumulated_radiance.clamp() * 0.25;


			Pixmap[x][y] = accumulated_radiance;
		}
	}
}

void Kerzenlicht_Renderer::renderPointCloud() {
	setPenColor(Rgba(1, 1, 1, 1));
	Render_Object.MeshData.applyTransformMatrix(Render_Object.Pos, Render_Object.Rot_Euler, Render_Object.Scale);
	for (const Mesh_Triangle& tri : Render_Object.MeshData.Faces) {
		Vertex v1 = Render_Object.MeshData.Vertex_Output[tri.Index1];
		Vertex v2 = Render_Object.MeshData.Vertex_Output[tri.Index2];
		Vertex v3 = Render_Object.MeshData.Vertex_Output[tri.Index3];

		if (Render_Object.MeshData.Vertex_Colors.size() > 0) {
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

	QT_Option* Render_Mode_Select = new QT_Option();

	Render_Mode_Select->addItem("Path Tracer", 0);
	Render_Mode_Select->addItem("Point Cloud", 1);
	Render_Mode_Select->addItem("Preview", 2);
	Render_Mode_Select->addItem("Textured", 3);
	Render_Mode_Select->addItem("Wireframe", 4);
	Render_Mode_Select->addItem("Z-Depth Debug", 5);

	Render_Mode_Select->setCurrentIndex(Parent->View_Mode);

	connect(Render_Mode_Select, &QT_Option::currentIndexChanged, [this](int Index) { renderSwitch(Index); });

	QT_Value_Input* ResX_Input = new QT_Value_Input();
	QIntValidator* ValidatorX = new QIntValidator();
	ResX_Input->setValidator(ValidatorX);
	ResX_Input->setText(QString::fromStdString(to_string(Parent->ResX)));
	connect(ResX_Input, &QT_Value_Input::returnPressed, [this, ResX_Input]() {changeXResolution(ResX_Input->text().toInt()); });

	QT_Value_Input* ResY_Input = new QT_Value_Input();
	QIntValidator* ValidatorY = new QIntValidator();
	ResY_Input->setValidator(ValidatorY);
	ResY_Input->setText(QString::fromStdString(to_string(Parent->ResY)));
	connect(ResY_Input, &QT_Value_Input::returnPressed, [this, ResY_Input]() {changeYResolution(ResY_Input->text().toInt()); });

	QT_Button* Save_Button = new QT_Button;
	Save_Button->setText("Save to .Bmp");
	connect(Save_Button, &QT_Button::clicked, this, &Renderer_Menu::save);

	Layout->addWidget(Load_File_Button);
	Layout->addWidget(Render_Mode_Select);
	Layout->addWidget(ResX_Input);
	Layout->addWidget(ResY_Input);
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
	Parent->ResX = value;
	Parent->Aspect_Ratio = static_cast<double>(Parent->ResX) / static_cast<double>(Parent->ResY);
	Parent->renderClear();
	QSettings("Raylight", "KerzenLicht").setValue("ResX", value);
	Parent->renderFrame();
}

void Renderer_Menu::changeYResolution(int value) {
	Parent->ResY = value;
	Parent->Aspect_Ratio = static_cast<double>(Parent->ResX) / static_cast<double>(Parent->ResY);
	Parent->renderClear();
	QSettings("Raylight", "KerzenLicht").setValue("ResY", value);
	Parent->renderFrame();
}

void Renderer_Menu::save() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Create New File"), "", tr("BitMap (*.bmp)"));
	Parent->storeBmp(fileName.toStdString());
}