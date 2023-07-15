#pragma once

#include "QT_Core.h"
#include "Core.h"

#include <QtOpenGLWidgets>
#include <vector>

struct R_Workspace_Offline_Viewport : QT_Linear_Contents {
	QT_Text_Stream* Log;

	R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log);
};

struct OpenGL : QOpenGLWidget {
	QT_Text_Stream* Log;

	OpenGL(QT_Text_Stream* P_Log);
};

struct Renderer {
	QT_Text_Stream* Log;

	std::vector<std::vector<Rgba>> Pixmap;
	Rgba Pen_Color;
	float Pen_Opacity;

	Renderer(QT_Text_Stream* P_Log);
};
