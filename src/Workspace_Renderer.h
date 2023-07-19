#pragma once

#include "QT_Core.h"

#include "Kerzenlicht_Renderer.h"

struct Workspace_Renderer : QT_Linear_Contents {
	QT_Text_Stream* Log;
	Kerzenlicht_Renderer* Renderer;

	Workspace_Renderer(QT_Text_Stream* P_Log);
};