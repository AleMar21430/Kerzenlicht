#include "R_Main_Window.h"

R_Main_Window::R_Main_Window(RUI_Application* P_App) : RUI_Main_Window() {
	setWindowIcon(QPixmap("./resources/Icon.png"));
	Log = new RUI_Text_Stream();
	App = P_App;

	R_Workspace_3D_Viewport* Vulkan = new R_Workspace_3D_Viewport(Log);
	setCentralWidget(Vulkan->Vulkan_Widget);
	readStyle();
	showMaximized();

	Log->append("<p style = color:rgb(250,140,25);>Raylight Version 1.0 Initialized Succesfully< / p>");
}

void R_Main_Window::readStyle() {
	std::ifstream File("src/Style.txt");
	std::stringstream Temp;
	Temp << File.rdbuf();
	App->setStyleSheet(QString::fromUtf8(Temp.str()));
	File.close();
}