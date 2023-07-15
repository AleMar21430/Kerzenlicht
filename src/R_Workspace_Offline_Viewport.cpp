#include "R_Workspace_Offline_Viewport.h"

R_Workspace_Offline_Viewport::R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log) : QT_Linear_Contents("_Container", true) {
	Log = P_Log;

	OpenGL_Preview* Preview = new OpenGL_Preview(Log);

	Log->append("<p style = color:rgb(250,140,25);>Opengl Viewport Initialized</p>");
}

OpenGL_Preview::OpenGL_Preview(QT_Text_Stream* P_Log) : QOpenGLWidget() {
	Log = P_Log;
	Render = false;

	Renderer = new Offline_Renderer(Log, 3840u, 2160u);
}

void OpenGL_Preview::paintGL() {
	/*glClear(GL_COLOR_BUFFER_BIT);
	if (this->Render) {
		int width = this->width() / this->Renderer.Pixmap[0].size();
		int height = this->height() / this->Renderer.Pixmap.size();

		glEnable(GL_TEXTURE_2D);

		GLuint texture_id;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->Renderer.Pixmap.data());

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
	}*/
}

Offline_Renderer::Offline_Renderer(QT_Text_Stream* P_Log, uint32_t Resolution_Width, uint32_t Resolution_Height) {
	Log = P_Log;

	ResX = Resolution_Width;
	ResY = Resolution_Height;
	Pen_Color = Rgba();
	Pen_Opacity = 1.0f;

	Pixmap = std::vector(Resolution_Height,std::vector(Resolution_Width,Rgba()));
}