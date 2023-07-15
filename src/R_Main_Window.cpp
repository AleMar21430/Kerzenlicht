#include "R_Main_Window.h"

Main_Window::Main_Window(QT_Application* P_App) : QT_Main_Window() {
	setWindowIcon(QPixmap("./resources/Icon.png"));
	Log = new QT_Text_Stream();
	App = P_App;

	//R_Workspace_Vulkan_Viewport* Vulkan = new R_Workspace_Vulkan_Viewport(Log);
	//setCentralWidget(Vulkan->Vulkan_Widget);
	R_Workspace_OpenGL_Viewport* OpenGL = new R_Workspace_OpenGL_Viewport(Log);
	setCentralWidget(OpenGL);
	readStyle();
	showMaximized();

	Log->append("<p style = color:rgb(250,140,25);>Raylight Version 1.0 Initialized Succesfully< / p>");
}

void Main_Window::readStyle() {
	std::ifstream File("src/Style.txt");
	std::stringstream Temp;
	Temp << File.rdbuf();
	App->setStyleSheet(QString::fromUtf8(Temp.str()));
	File.close();
}