#pragma once

#include "QT_Core.h"

#include "R_Workspace_Vulkan_Viewport.h"
#include "R_Workspace_OpenGL_Viewport.h"
#include "R_Workspace_Offline_Viewport.h"

#include <sstream>
#include <fstream>

struct Main_Window : QT_Main_Window {
	QT_Text_Stream* Log;
	QT_Application* App;

	Q_OBJECT
public:
	Main_Window(QT_Application* P_App);

	void readStyle();
};