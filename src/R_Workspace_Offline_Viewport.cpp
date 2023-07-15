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
		std::vector<float> Flattened_Pixmap;
		for (int y = 0; y < Renderer->Pixmap.size(); y++) {
			for (int x = 0; x < Renderer->Pixmap[0].size(); x++) {
				Rgba& rgba = Renderer->Pixmap[y][x];
				Flattened_Pixmap.push_back(rgba.R);
				Flattened_Pixmap.push_back(rgba.G);
				Flattened_Pixmap.push_back(rgba.B);
				Flattened_Pixmap.push_back(rgba.A);
			}
		}

		int x = width() / Renderer->Pixmap[0].size();
		int y = height() / Renderer->Pixmap.size();

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

Offline_Renderer::Offline_Renderer(QT_Text_Stream* P_Log, uint32_t P_ResX, uint32_t P_ResY) {
	Log = P_Log;

	ResX = P_ResX;
	ResY = P_ResY;
	Pen_Color = Rgba();
	Pen_Opacity = 1.0f;

	Pixmap = std::vector(ResY,std::vector<Rgba>(ResX));

	for (int y = 0; y < P_ResY; y++) {
		for (int x = 0; x < P_ResX; x++) {
			Pixmap[y][x] = Rgba(1,0,0,1);
		}
	}

	std::stringstream log;
	log << "X: " << Pixmap[0].size() << std::endl << "Y: " << Pixmap.size() << std::endl;
	Log->log(log.str());
}