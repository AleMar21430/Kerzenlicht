#pragma once

#include "QT_Core.h"

#include "Main_Window.h"
#include "Workspace_Renderer.h"
#include "Workspace_Log.h"

struct Main_Window; //Fwd Decl.
struct Workspace_Dock_Header; //Fwd Decl.

struct Workspace_Manager : QT_Dock {
	QT_Text_Stream* Log;
	Main_Window* Parent;
	Workspace_Dock_Header* Titlebar;

	Workspace_Manager(QT_Text_Stream* P_Log, Main_Window* P_Parent);
};

struct Workspace_Dock_Header : QT_Linear_Contents {
	QT_Text_Stream* Log;
	Workspace_Manager* Parent;
	QT_Floating_Toggle* Expand_Collapse_Toggle;
	QT_Button* Workspace_Type_Button;
	QT_Toggle* More_Settings_Toggle;
	QT_Button* Add_Dock_Button;
	QT_Button* Add_Tab_Button;
	QT_Toggle* Floating_Toggle;
	QT_Toggle* Collapsible_Mode_Toggle;
	QT_Button* Close_Dock_Button;
	QT_Menu* Workspace_Menu;
	QT_Menu* Close_Confirmation_Menu;
	QByteArray Stored_Expanded;

	Workspace_Dock_Header(QT_Text_Stream* P_Log, Workspace_Manager* P_Parent);

	void initWorkspacesMenu();
	void selectWorkspaceTypeClick();
	void setWorkspace(std::string P_Type = "Manager");
	void showHideSettings(bool P_Toggle);
	void initCloseConfirmationMenu();
	void closeClick();
	void closeDock();
	void addDockClick();
	void addDockTabClick();
	void setFloatingToggle(bool P_Toggle);
	void setCollapsibleModeToggle(bool P_Toggle);
	void expandCollapseToggle(bool P_Toggle);
	void mouseDoubleClickEvent(QMouseEvent* event) override;
};