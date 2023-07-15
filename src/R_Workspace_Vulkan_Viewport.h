#pragma once

#include "QT_Core.h"
#include "R_Vulkan_Viewport.h"

struct R_Workspace_Vulkan_Viewport : QT_Linear_Contents {
	QT_Text_Stream* Log;
	QWidget* Vulkan_Widget;

	R_Workspace_Vulkan_Viewport(QT_Text_Stream* P_Log);
};