#include "R_Workspace_Offline_Viewport.h"

R_Workspace_Offline_Viewport::R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log) : QT_Linear_Contents("_Container", true) {
	Log = P_Log;

	Preview = new OpenGL_Preview(Log);
	Layout->addWidget(Preview);

	Log->append("<p style = color:rgb(250,140,25);>Opengl Viewport Initialized</p>");
}

OpenGL_Preview::OpenGL_Preview(QT_Text_Stream* P_Log) : QOpenGLWidget() {
	Log = P_Log;
	Render = false;

	Renderer = new Offline_Renderer(Log, 256, 256);
	Render = true;
}

void OpenGL_Preview::initializeGL() {
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void OpenGL_Preview::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-width()/height(), width()/height(), -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (Render) {
		for (int y = 0; y < Renderer->Pixmap.size(); y++) {
			for (int x = 0; x < Renderer->Pixmap[0].size(); x++) {
				Rgba& rgba = Renderer->Pixmap[y][x];
				glColor4f(rgba.R, rgba.G, rgba.B, rgba.A);
				glBegin(GL_POINTS);
				glVertex2f(x, y);
				glEnd();
			}
		}
		glFlush();

		//glDeleteTextures(1, &texture_id);
		//glDisable(GL_TEXTURE_2D);
	}
}

void OpenGL_Preview::resizeGL(int width, int height) {
	glViewport(0, 0, width, height);
}

Offline_Renderer::Offline_Renderer(QT_Text_Stream* P_Log, uint32_t Resolution_Width, uint32_t Resolution_Height) {
	Log = P_Log;

	ResX = Resolution_Width;
	ResY = Resolution_Height;
	Pen_Color = Rgba();
	Pen_Opacity = 1.0f;

	Pixmap = std::vector(Resolution_Height,std::vector<Rgba>(Resolution_Width));

	for (int y = 0; y < ResY; y++) {
		for (int x = 0; x < ResX; x++) {
			Pixmap[y][x] = Rgba();
		}
	}
	std::stringstream log;
	log << "X: " << Pixmap[0].size() << std::endl << "Y: " << Pixmap.size() << std::endl;
	/*for (const auto& row : Pixmap) {
		for (const auto& pixel : row) {
			log << "R: " << pixel.R << ", " << "G: " << pixel.G << ", " << "B: " << pixel.B << ", " << "A: " << pixel.A << std::endl;
		}
		log << std::endl;
	}*/
	Log->log(log.str());
}