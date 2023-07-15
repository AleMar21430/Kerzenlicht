#include "R_Workspace_Offline_Viewport.h"

R_Workspace_Offline_Viewport::R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log) : QT_Linear_Contents("_Container", true) {
	Log = P_Log;

	QOpenGLWidget* OpenGL_Container = new QOpenGLWidget();

	Log->append("<p style = color:rgb(250,140,25);>Opengl Viewport Initialized</p>");
}

OpenGL::OpenGL(QT_Text_Stream* P_Log) : QOpenGLWidget() {
	Log = P_Log;
}

Renderer::Renderer(QT_Text_Stream* P_Log) {
	Log = P_Log;
}