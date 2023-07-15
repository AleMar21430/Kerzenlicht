#include "R_Workspace_Offline_Viewport.h"

R_Workspace_Offline_Viewport::R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log) : QT_Linear_Contents("_Container", true) {
	Log = P_Log;

	Preview = new OpenGL_Preview(Log);
	Layout->addWidget(Preview);
}

OpenGL_Preview::OpenGL_Preview(QT_Text_Stream* P_Log) : QOpenGLWidget() {
	Log = P_Log;
	Render = false;

	Renderer = new Offline_Renderer(Log, 256, 256);
	Render = true;
	//Renderer->renderTriWire("D:/UVG/Kerzenlicht/resources/Head.obj");
	Renderer->renderLine(0, 0, 250, 250);
	Renderer->storeBmp("Output.bmp");
}

void OpenGL_Preview::initializeGL() {
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	std::stringstream log;
	log << "OpenGl Initialized" << std::endl;
	Log->log(log.str());
}

void OpenGL_Preview::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-width()/height(), width()/height(), -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (Render) {
		std::vector<float> Flattened_Pixmap;
		for (int y = 0; y < Renderer->ResY; y++) {
			for (int x = 0; x < Renderer->ResX; x++) {
				Rgba& rgba = Renderer->Pixmap[y][x];
				Flattened_Pixmap.push_back(rgba.R);
				Flattened_Pixmap.push_back(rgba.G);
				Flattened_Pixmap.push_back(rgba.B);
				Flattened_Pixmap.push_back(rgba.A);
			}
		}

		int x = width() / Renderer->ResX;
		int y = height() / Renderer->ResY;

		glEnable(GL_TEXTURE_2D);

		GLuint texture_id;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_FLOAT, Flattened_Pixmap.data());

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(-1, -1);
		glTexCoord2f(1, 0);
		glVertex2f(1, -1);
		glTexCoord2f(1, 1);
		glVertex2f(1, 1);
		glTexCoord2f(0, 1);
		glVertex2f(-1, 1);
		glEnd();

		glDeleteTextures(1, &texture_id);
		glDisable(GL_TEXTURE_2D);
	}
}

void OpenGL_Preview::resizeGL(int width, int height) {
	glViewport(0, 0, width, height);
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

void Offline_Renderer::renderTriWire(std::string P_File) {
	std::ifstream file(P_File);

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v") {
			Vertex vertex;
			iss >> vertex.Pos.X >> vertex.Pos.Y >> vertex.Pos.Z;
			Vertex_Buffer.push_back(vertex);
		}
		else if (prefix == "f") {
			Tri face;
			iss >> face.I1 >> face.I2 >> face.I3;
			Triangle_Buffer.push_back(face);
		}
	}

	file.close();

	for (const Tri& tri : Triangle_Buffer) {
		const Vertex& v1 = Vertex_Buffer[tri.I1 - 1];
		const Vertex& v2 = Vertex_Buffer[tri.I2 - 1];
		const Vertex& v3 = Vertex_Buffer[tri.I3 - 1];

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