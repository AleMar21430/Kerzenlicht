#pragma once

#include "QT_Core.h"

struct R_Workspace_OpenGL_Viewport : QT_Linear_Contents {
	QT_Text_Stream* Log;
	QWidget* Vulkan_Widget;

	R_Workspace_OpenGL_Viewport(QT_Text_Stream* P_Log);
};