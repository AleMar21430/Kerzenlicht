#pragma once

#include "RUI_Core.h"

#include "R_Workspace_3D_Viewport.h"

#include <sstream>
#include <fstream>

struct R_Main_Window : RUI_Main_Window {
	RUI_Text_Stream* Log;
	RUI_Application* App;

	Q_OBJECT
public:
	R_Main_Window(RUI_Application* P_App);

	void readStyle();
};