#pragma once

#include "QT_Core.h"

struct Workspace_Log : QT_Linear_Contents {
	QT_Text_Stream* Log;

	Workspace_Log(QT_Text_Stream* P_Log);

	void append(std::string P_Text);
	void exportLog();
};