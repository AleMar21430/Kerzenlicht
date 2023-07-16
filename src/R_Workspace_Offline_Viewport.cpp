#include "R_Workspace_Offline_Viewport.h"

R_Workspace_Offline_Viewport::R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log) : QGraphicsView() {
	Log = P_Log;

	currentScale = 1;
	Scene = new QGraphicsScene();
	setScene(Scene);
	setMouseTracking(true);
	setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorViewCenter);
	setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
	setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
	setRenderHint(QPainter::RenderHint::Antialiasing);

	Renderer = new Offline_Renderer(Log, 4096, 4096);
	
	Renderer->createObject("Paimon");
	Renderer->loadObj("./Paimon.obj");
	Renderer->loadModel("Paimon");
	Renderer->clearBuffers();

	Renderer->Object_Array["Paimon"].scale(0.175);
	Renderer->Object_Array["Paimon"].translate(Vec3(0,-0.95,0));

	Renderer->renderWire();
	//Renderer->renderLine(0, 0, 250, 250);
	Renderer->storeBmp("Output.bmp");
	setImage("./Output.bmp");
}

void R_Workspace_Offline_Viewport::setImage(std::string P_File) {
	Scene->clear();
	QPixmap Image(QString::fromStdString(P_File));
	QGraphicsPixmapItem* Item = new QGraphicsPixmapItem(Image);
	Scene->addItem(Item);
	centerOn(Item->boundingRect().center());
	fitInView(Item->boundingRect(), Qt::KeepAspectRatio);
}

void R_Workspace_Offline_Viewport::wheelEvent(QWheelEvent* P_Event) {
	float zoomFactor = 1.25;

	QPointF oldPos = mapToScene(P_Event->position().toPoint());
	if (P_Event->angleDelta().y() > 0) {
		scale(zoomFactor, zoomFactor);
		currentScale *= zoomFactor;
	}
	
	else if (currentScale > 0.1) {
		scale(1 / zoomFactor, 1 / zoomFactor);
		currentScale /= zoomFactor;
	}

	QPointF newPos = mapToScene(P_Event->position().toPoint());
	QPointF delta = newPos - oldPos;
	translate(delta.x(), delta.y());
}

//////////////
// Renderer //
//////////////

Offline_Renderer::Offline_Renderer(QT_Text_Stream* P_Log, uint32_t P_ResX, uint32_t P_ResY) {
	Log = P_Log;

	ResX = P_ResX;
	ResY = P_ResY;
	Pen_Color = Rgba();
	Pen_Opacity = 1.0f;

	Pixmap = std::vector(ResY,std::vector<Rgba>(ResX));

	Object_Array = std::map<std::string, Object>();

	Vertex_Buffer = std::vector<Vertex>();
	Triangle_Buffer = std::vector<Tri>();

	for (int y = 0; y < P_ResY; y++) {
		for (int x = 0; x < P_ResX; x++) {
			Pixmap[y][x] = Rgba(0.1,0.1,0.1,1);
		}
	}

	std::stringstream log;
	log << "Renderer Settings" << std::endl;
	log << "Res X: " << Pixmap[0].size() << std::endl;
	log << "Res Y: " << Pixmap.size() << std::endl;
	Log->log(log.str());
}

void Offline_Renderer::setPenColor(Rgba P_Color) {
	Pen_Color = P_Color;
}

void Offline_Renderer::setPenOpacity(float P_Opacity) {
	Pen_Opacity = P_Opacity;
}

void Offline_Renderer::renderPixel(uint32_t P_X, uint32_t P_Y) {
	if (P_X < ResX && P_X >= 0 && P_Y < ResY && P_Y >= 0) {
		Pixmap[P_Y][P_X] = Pen_Color;
	}
}

void Offline_Renderer::renderLine(int P_Start_X, int P_Start_Y, int P_End_X, int P_End_Y) {
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

void Offline_Renderer::loadObj(std::string P_File) {
	std::ifstream file(P_File);
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v") {
			double x, y, z;
			iss >> x;
			iss >> y;
			iss >> z;
			Vertex vertex(Vec3(x,y,z));
			Vertex_Buffer.push_back(vertex);
		}
		else if (prefix == "f") {
			std::string V1, V2, V3;
			iss >> V1 >> V2 >> V3;
			int I1 = std::stoi(splitString(V1, "/")[0]) - 1;
			int I2 = std::stoi(splitString(V2, "/")[0]) - 1;
			int I3 = std::stoi(splitString(V3, "/")[0]) - 1;
			Tri triangle(I1,I2,I3);
			Triangle_Buffer.push_back(triangle);
		}
	}
	file.close();
}

void Offline_Renderer::createObject(std::string P_Name) {
	Object Temp(P_Name);
	Object_Array[P_Name] = Temp;
}

void Offline_Renderer::loadModel(std::string P_Name) {
	Object& Ref = Object_Array[P_Name];
	Ref.Vertices = Vertex_Buffer;
	Ref.Triangles = Triangle_Buffer;
	Ref.renderPass();
}

void Offline_Renderer::clearBuffers() {
	Vertex_Buffer = std::vector<Vertex>();
	Triangle_Buffer = std::vector<Tri>();
}

void Offline_Renderer::renderWire() {
	for (auto& Data : Object_Array) {
		for (Tri tri : Data.second.Triangle_Buffer) {
			if (tri.I1 > 0 && tri.I1 < Data.second.Vertex_Buffer.size() &&
				tri.I2 > 0 && tri.I2 < Data.second.Vertex_Buffer.size() &&
				tri.I3 > 0 && tri.I3 < Data.second.Vertex_Buffer.size()) {
				Vertex v1 = Data.second.Vertex_Buffer[tri.I1];
				Vertex v2 = Data.second.Vertex_Buffer[tri.I2];
				Vertex v3 = Data.second.Vertex_Buffer[tri.I3];
				int x1 = static_cast<int>((v1.Pos.X + 1.0f) * 0.5f * ResY);
				int y1 = static_cast<int>((v1.Pos.Y + 1.0f) * 0.5f * ResX);
				int x2 = static_cast<int>((v2.Pos.X + 1.0f) * 0.5f * ResY);
				int y2 = static_cast<int>((v2.Pos.Y + 1.0f) * 0.5f * ResX);
				int x3 = static_cast<int>((v3.Pos.X + 1.0f) * 0.5f * ResY);
				int y3 = static_cast<int>((v3.Pos.Y + 1.0f) * 0.5f * ResX);

				renderLine(x1, y1, x2, y2);
				renderLine(x2, y2, x3, y3);
				renderLine(x3, y3, x1, y1);
			}
		}
	}
}

void Offline_Renderer::storeBmp(std::string P_File) {
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
	for (const std::vector<Rgba>& row : Pixmap) {
		for (const Rgba& pixel : row) {
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