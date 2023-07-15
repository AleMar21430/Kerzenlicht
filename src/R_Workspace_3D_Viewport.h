#pragma once

#include <QWidget>

#include "RUI_Core.h"
#include "R_Vulkan_Viewport.h"

struct R_Workspace_3D_Viewport : RUI_Linear_Contents {
	RUI_Text_Stream* Log;
	QWidget* Vulkan_Widget;

	R_Workspace_3D_Viewport(RUI_Text_Stream* P_Log);
};