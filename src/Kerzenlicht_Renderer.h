#pragma once

#include "QT_Core.h"

enum Render_Mode {
	WIREFRAME,
	POINTCLOUD,
	VISUALIZER
};

struct Renderer_Menu;

struct Kerzenlicht_Renderer : QT_Graphics_View {
	QT_Text_Stream* Log;

	bool Mouse_Pressed;
	QPoint Mouse_Down_Pos;

	uint32_t ResX;
	uint32_t ResY;
	double Aspect_Ratio;
	Rgba Pen_Color;
	float Pen_Opacity;
	std::vector<std::vector<Rgba>> Pixmap;

	Render_Mode View_Mode;

	std::map<std::string, Object> Object_Array;

	std::vector<Mesh_Face> Face_Buffer;
	std::vector<Rgb> Vertex_Colors_Buffer;
	std::vector<Vec3> Vertex_Positions_Buffer;
	std::map<std::string, std::map<std::string, double>> Vertex_Weights_Buffer;

	QGraphicsScene* Scene;
	Renderer_Menu* Menu;

	Kerzenlicht_Renderer(QT_Text_Stream* P_Log);

	void setPenColor(Rgba P_Color);
	void setPenOpacity(float P_Opacity);

	void renderClear();
	void renderPixel(uint32_t P_X, uint32_t P_Y);
	void renderLine(int P_Start_X, int P_Start_Y, int P_End_X, int P_End_Y);
	void renderWireframe();
	void renderPointCloud();
	void renderEdgeVisualizer();
	void renderFrame();
	void drawToSurface();

	void createObject(std::string P_Name);
	void loadModel(std::string P_Name);
	void clearBuffers();

	void loadObj(std::string P_File, bool P_Vert_Colors = false, bool P_Textured = false, bool P_Normals = false);

	void storeBmp(std::string P_File);

	void setImage(std::string P_File);
	
	void wheelEvent(QWheelEvent* P_Event) override;
	void mousePressEvent(QMouseEvent* P_Event) override;
	void mouseMoveEvent(QMouseEvent* P_Event) override;
	void mouseReleaseEvent(QMouseEvent* P_Event) override;
	void resizeEvent(QResizeEvent* P_Event) override;
	void closeEvent(QCloseEvent* P_Event) override;
};

struct Renderer_Menu : QT_Linear_Contents {
	Kerzenlicht_Renderer* Parent;

	QProgressBar* Progress;

	QCheckBox* Obj_Vertex_Colors;
	QCheckBox* Obj_Textured;
	QCheckBox* Obj_Normals;

	bool Vertex_Colors_Obj_Import;
	bool Textured_Obj_Import;
	bool Normals_Obj_Import;

	Renderer_Menu(Kerzenlicht_Renderer* P_Parent);
	void openObjFile();
	void clearScene();
	void renderWireframe();
	void renderPointCloud();
	void renderEdgeVisualizer();
	void changeXResolution(int value);
	void changeYResolution(int value);
	void save();
};