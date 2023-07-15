#pragma once

#include "QT_Core.h"
#include "Core.h"

#include <QOpenGLFunctions>
#include <vector>

struct Offline_Renderer {
	QT_Text_Stream* Log;

	uint32_t ResX;
	uint32_t ResY;
	Rgba Pen_Color;
	float Pen_Opacity;
	std::vector<std::vector<Rgba>> Pixmap;

	Offline_Renderer(QT_Text_Stream* P_Log, uint32_t Resolution_Width, uint32_t Resolution_Height);
};

struct OpenGL_Preview : QOpenGLWidget, QOpenGLFunctions {
	QT_Text_Stream* Log;
	bool Render;

	Offline_Renderer* Renderer;

	OpenGL_Preview(QT_Text_Stream* P_Log);
	void initializeGL() override;
	void paintGL() override;
};

struct R_Workspace_Offline_Viewport : QT_Linear_Contents {
	QT_Text_Stream* Log;
	OpenGL_Preview* Preview;

	R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log);
};