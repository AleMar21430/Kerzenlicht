#pragma once

#include "QT_Core.h"

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

	void renderTriWire();
	void loadObj(std::string P_File);

	void renderTris();
	void renderQuad();

	void storeBmp(std::string P_File);
};

struct R_Workspace_Offline_Viewport : QGraphicsView {
	QT_Text_Stream* Log;

	Offline_Renderer* Renderer;
	QGraphicsScene* Scene;

	R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log);
	void setImage(std::string P_File);
};