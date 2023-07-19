#pragma once

#include "QT_Core.h"

#include "Workspace_Manager.h"

struct Main_Window : QT_Main_Window {
	QT_Text_Stream* Log;
	QT_Application* App;

	Main_Window(QT_Application* P_App);

	void readStyle();
};