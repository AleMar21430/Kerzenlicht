#include "R_Main_Window.h"

Main_Window::Main_Window(QT_Application* P_App) : QT_Main_Window() {
	setWindowIcon(QPixmap("./resources/Icon.png"));
	Log = new QT_Text_Stream();
	App = P_App;
	showMaximized();

	//R_Workspace_Vulkan_Viewport* Vulkan_Renderer = new R_Workspace_Vulkan_Viewport(Log);
	//setCentralWidget(Vulkan_Renderer->Vulkan_Widget);
	//R_Workspace_OpenGL_Viewport* OpenGL_Renderer = new R_Workspace_OpenGL_Viewport(Log);
	//setCentralWidget(OpenGL_Renderer);
	R_Workspace_Offline_Viewport* Offline_Renderer = new R_Workspace_Offline_Viewport(Log);
	setCentralWidget(Offline_Renderer);
	readStyle();

	Log->log("Kerzenlicht Version 1.0 Initialized Succesfully");
}

void Main_Window::readStyle() {
	std::ifstream File("src/Style.txt");
	std::stringstream Temp;
	Temp << File.rdbuf();
	App->setStyleSheet(QString::fromUtf8(Temp.str()));
	File.close();
}