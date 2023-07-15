#include "R_Workspace_OpenGL_Viewport.h"

R_Workspace_OpenGL_Viewport::R_Workspace_OpenGL_Viewport(QT_Text_Stream* P_Log) : QT_Linear_Contents("_Container", true) {
	Log = P_Log;

	OpenGL* OpenGL_Container = new OpenGL(Log);

	Log->append("<p style = color:rgb(250,140,25);>Opengl Viewport Initialized</p>");
}

OpenGL::OpenGL(QT_Text_Stream* P_Log) : QOpenGLWidget() {
	Log = P_Log;
}