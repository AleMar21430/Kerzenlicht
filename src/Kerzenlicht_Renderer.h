#pragma once

#include "QT_Core.h"

enum Render_Mode {
	PATHTRACING,
	POINTCLOUD,
	PREVIEW,
	TEXTURED,
	WIREFRAME,
	ZDEPTH
};

struct Renderer_Menu;
struct Ray;

struct Kerzenlicht_Renderer : QT_Graphics_View {
	Q_OBJECT
public:
	QT_Text_Stream* Log;

	bool Left_Mouse_Pressed;
	bool Right_Mouse_Pressed;
	QPoint Mouse_Down_Pos;

	double Aspect_Ratio;
	Rgba Pen_Color;
	float Pen_Opacity;
	vector<vector<Rgba>> Pixmap;
	vector<vector<double>> ZBuffer;

	Render_Mode View_Mode;

	vector<QThread*> Thread_Storage;
	vector<Object> Render_Scene;
	Camera Render_Camera;

	QGraphicsScene* Scene;
	Renderer_Menu* Menu;

	Kerzenlicht_Renderer(QT_Text_Stream* P_Log);

	void setPenColor(Rgba P_Color);
	void setPenOpacity(float P_Opacity);

	void renderClear();
	void renderPixel(uint32_t P_X, uint32_t P_Y);
	void renderPixel(uint32_t P_X, uint32_t P_Y, Rgba P_Color);
	void renderLine(int P_Start_X, int P_Start_Y, int P_End_X, int P_End_Y);
	void render2DPoly(vector<pair<int, int>> P_Poly);
	
	void renderTriangle(Vertex P_Vert1, Vertex P_Vert2, Vertex P_Vert3);
	tuple<double, double, double> barycentricCoords(const Vec3& P_Pos1, const Vec3& P_Pos2, const Vec3& P_Pos3, double P_X, double P_Y);

	void renderWireframe();
	void renderPointCloud();
	void renderPreview();
	void renderZBuffer();
	void renderTextured();
	void renderPathTracer();
	void renderFrame();
	void drawToSurface();

	void loadObj(string P_File);
	void storeBmp(string P_File);
	
	void wheelEvent(QWheelEvent* P_Event) override;
	void mousePressEvent(QMouseEvent* P_Event) override;
	void mouseMoveEvent(QMouseEvent* P_Event) override;
	void mouseReleaseEvent(QMouseEvent* P_Event) override;
	void keyPressEvent(QKeyEvent* P_Event) override;
	void resizeEvent(QResizeEvent* P_Event) override;
	void closeEvent(QCloseEvent* P_Event) override;

	// Pathtracing
	Rgba calculatePixelColor(const Ray& ray) const;

public slots:
	void loadObject(Object P_Object);
	void updateProgress(int P_Progress);
};

struct Renderer_Menu : QT_Linear_Contents {

	Kerzenlicht_Renderer* Parent;

	QProgressBar* Progress;

	Renderer_Menu(Kerzenlicht_Renderer* P_Parent);
	void openObjFile();
	void renderSwitch(int Index);
	void changeXResolution(int P_Value);
	void changeYResolution(int P_Value);
	void save();
};

struct Ray {
	Vec3 org, dir;

	Ray(const Vec3 org, const Vec3& dir) : org(org), dir(dir) {}
	Ray() : org(Vec3(0, 0, 0)), dir(Vec3(0, 0, 0)) {}
};
