#include "Main_Window.h"

Main_Window::Main_Window(QT_Application* P_App) : QT_Main_Window() {
	setWindowIcon(QPixmap("./resources/Icon.png"));
	readStyle();

	Log = new QT_Text_Stream();
	App = P_App;

	Workspace_Manager* workspace_manager = new Workspace_Manager(Log, this);
	//Workspace_Toolbar* Toolbar = new Workspace_Toolbar(Log, this);
	addDockWidget(Qt::BottomDockWidgetArea, workspace_manager);
	//addToolBar(Toolbar);
	showMaximized();

	Log->append	("Kerzenlicht Version 0.1.0 Initialized Succesfully");
}

void Main_Window::readStyle() {
	std::ifstream File("resources/Style.txt");
	std::stringstream Temp;
	Temp << File.rdbuf();
	App->setStyleSheet(QString::fromUtf8(Temp.str()));
	File.close();
}