#include "R_Main_Window.h"

Main_Window::Main_Window(QT_Application* P_App) : QT_Main_Window() {
	setWindowIcon(QPixmap("./resources/Icon.png"));
	Log = new QT_Text_Stream();
	App = P_App;

	R_Workspace_Offline_Viewport* Offline_Renderer = new R_Workspace_Offline_Viewport(Log);
	QT_Splitter* Splitter = new QT_Splitter(false);
	Splitter->addWidget(Offline_Renderer->Menu);
	Splitter->addWidget(Offline_Renderer);
	setCentralWidget(Splitter);
	//readStyle();
	showMaximized();

	Log->log("Kerzenlicht Version 0.1.0 Initialized Succesfully");
}

void Main_Window::readStyle() {
	std::ifstream File("src/Style.txt");
	std::stringstream Temp;
	Temp << File.rdbuf();
	App->setStyleSheet(QString::fromUtf8(Temp.str()));
	File.close();
}