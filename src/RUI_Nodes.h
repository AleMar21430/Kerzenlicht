#pragma once

#include "RUI_Core.h"

struct RUI_Node : RUI_Linear_Contents {
	RUI_Linear_Contents* Header;
	RUI_Linear_Contents* Contents;
	RUI_Toggle* Drop_Down_Button;
	RUI_Label* Label;
	bool Move_Pressing;
	QPoint Start_Pos;

	RUI_Node();

	void expandCollapse(bool checked);
	void expand();
	void collapse();

	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
};