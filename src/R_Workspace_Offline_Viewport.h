#pragma once

#include "QT_Core.h"
#include "Core.h"

#include <QOpenGLFunctions>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <vector>

#include <iostream>
#include <fstream>

struct Offline_Renderer {
	QT_Text_Stream* Log;

	uint32_t ResX;
	uint32_t ResY;
	Rgba Pen_Color;
	float Pen_Opacity;
	std::vector<std::vector<Rgba>> Pixmap;

	std::vector<Vertex> Vertex_Buffer;
	std::vector<Tri> Triangle_Buffer;

	Offline_Renderer(QT_Text_Stream* P_Log, uint32_t P_ResX, uint32_t P_ResY);
	void setPenColor(Rgba P_Color);
	void setPenOpacity(float P_Opacity);
	void renderPixel(uint32_t P_X, uint32_t P_Y);
	void renderLine(int32_t P_Start_X, int32_t P_Start_Y, int32_t P_End_X, int32_t P_End_Y);

	void renderTriWire(std::string File);

	void renderTris();
	void renderQuad();

	void storeBmp(std::string P_File);
};

struct OpenGL_Preview : public QOpenGLWidget, protected QOpenGLFunctions {
	QT_Text_Stream* Log;
	bool Render;

	Offline_Renderer* Renderer;

	OpenGL_Preview(QT_Text_Stream* P_Log);

	void resizeGL(int width, int height) override;
	void initializeGL() override;
	void paintGL() override;
};

struct R_Workspace_Offline_Viewport : QT_Linear_Contents {
	QT_Text_Stream* Log;
	OpenGL_Preview* Preview;

	R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log);
};