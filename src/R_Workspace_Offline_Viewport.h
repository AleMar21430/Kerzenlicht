#pragma once

#include "QT_Core.h"

struct R_Workspace_Offline_Viewport : QT_Graphics_View {
	QT_Text_Stream* Log;

	QGraphicsScene* Scene;
	float Viewport_Scale;

	uint32_t ResX;
	uint32_t ResY;
	double Aspect_Ratio;
	Rgba Pen_Color;
	float Pen_Opacity;
	std::vector<std::vector<Rgba>> Pixmap;

	std::map<std::string, Object> Object_Array;

	std::vector<Vec3> Vertex_Positions_Buffer;
	std::map<std::string, std::vector<Rgb>> Vertex_Colors_Buffer;
	std::map<std::string, std::map<std::string, double>> Vertex_Weights_Buffer;
	std::vector<Tri> Face_Buffer;

	R_Workspace_Offline_Viewport(QT_Text_Stream* P_Log);

	void setPenColor(Rgba P_Color);
	void setPenOpacity(float P_Opacity);

	void renderPixel(uint32_t P_X, uint32_t P_Y);
	void renderLine(int P_Start_X, int P_Start_Y, int P_End_X, int P_End_Y);
	void renderWire();
	void renderPointCloud();
	void drawToSurface();

	void createObject(std::string P_Name);
	void loadModel(std::string P_Name);
	void clearBuffers();

	void loadObj(std::string P_File, bool P_Vert_Colors = false, bool P_Textured = false, bool P_Normals = false);

	void renderTris();
	void renderQuad();

	void storeBmp(std::string P_File);

	void setImage(std::string P_File);
	
	void wheelEvent(QWheelEvent* P_Event) override;
};

struct Renderer_Menu : QT_Linear_Contents {
	R_Workspace_Offline_Viewport* Parent;

	Renderer_Menu(R_Workspace_Offline_Viewport* P_Parent);
	void openFile();
	void render();
	void save();
};