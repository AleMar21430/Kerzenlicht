#include "R_Workspace_Offline_Viewport.h"

R_Workspace_Offline_Viewport::R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log) : QGraphicsView() {
	Log = P_Log;

	Scene = new QGraphicsScene();
	setScene(Scene);

	Renderer = new Offline_Renderer(Log, 256, 256);
	Renderer->loadObj("D:/UVG/Kerzenlicht/resources/Head.obj");
	Renderer->renderTriWire();
	//Renderer->renderLine(0, 0, 250, 250);
	//Renderer->storeBmp("Output.bmp");
	//setImage("D:/UVG/Kerzenlicht/Output.bmp");
}

void R_Workspace_Offline_Viewport::setImage(std::string P_File) {
	Scene->clear();
	QPixmap Image(QString::fromStdString(P_File));
	QGraphicsPixmapItem* Item = new QGraphicsPixmapItem(Image);
	Scene->addItem(Item);
	centerOn(Item->boundingRect().center());
	fitInView(Item->boundingRect(), Qt::KeepAspectRatio);
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

	Vertex_Buffer = std::vector<Vertex>();
	Triangle_Buffer = std::vector<Tri>();

	for (int y = 0; y < P_ResY; y++) {
		for (int x = 0; x < P_ResX; x++) {
			Pixmap[y][x] = Rgba(1,0,0,1);
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
	if (P_X <= ResX && P_Y <= ResY) {
		Pixmap[P_Y][P_X] = Pen_Color;
	}
}

void Offline_Renderer::renderLine(int32_t P_Start_X, int32_t P_Start_Y, int32_t P_End_X, int32_t P_End_Y) {
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

	std::stringstream log;

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
			log << "v" << "|" << x << "|" << y << "|" << z << std::endl;
			Vertex_Buffer.push_back(vertex);
		}
		else if (prefix == "f") {
			std::string Value;
			iss >> Value;
			std::vector<std::string> Values = splitString(Value,"/");
			Tri triangle(std::stoi(Values[0]), std::stoi(Values[1]), std::stoi(Values[2]));
			log << "f" << "|" << Values[0] << "|" << Values[1] << "|" << Values[2] << std::endl;
			Triangle_Buffer.push_back(triangle);
		}
	}
	file.close();

	
	Log->log(log.str());
}

void Offline_Renderer::renderTriWire() {
	for (Tri tri : Triangle_Buffer) {
		Vertex v1 = Vertex_Buffer[tri.I1-1];
		Vertex v2 = Vertex_Buffer[tri.I2-1];
		Vertex v3 = Vertex_Buffer[tri.I3-1];

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

void Offline_Renderer::storeBmp(std::string P_File) {
	std::ofstream file(P_File, std::ios::binary);
	if (!file.is_open()) {
		// Handle file creation error
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