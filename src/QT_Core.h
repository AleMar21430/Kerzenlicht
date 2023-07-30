#pragma once

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtOpenGLWidgets>

#include "Core.h"

struct QT_Application;		// Application
struct QT_Button;			// Push Button
struct QT_Dock;				// Dock Widget
struct QT_Drop_Down;		// Button Switch That Expands And Collapses Contents
struct QT_File_Browser;		// Popup Window To Select File
struct QT_Grid_Layout;		// Layout
struct QT_Graphics_View;
struct QT_Label;			// Widget With Text
struct QT_Linear_Layout;	// Linear Layout
struct QT_Linear_Contents;	// Widget With Linear Layout
struct QT_Main_Window;		// Windows Window
struct QT_Menu;				// Popup|Context Menu
struct QT_Option;
struct QT_Scroll_Area;		// Linear Layout With Scrollbar If Content Overflows
struct QT_Slider;			// Integer Slider
struct QT_Splitter;			// User Resizable Layout
struct QT_Spreadsheet;
struct QT_Spreadsheet_Item;
struct QT_Tree;
struct QT_Tree_Item;
struct QT_Text_Input;		// HTML Text Input
struct QT_Text_Stream;		// HTML Text Output
struct QT_Toggle;			// Button Switch
struct QT_ToolBar;			// File|Edit|Settings|etc... Toolbar
struct QT_Value_Input;		// String|Float|Integer Text Input
struct QT_Widget;			// Widget

struct QT_Application : QApplication {
	QT_Application(int argc, char* argv[]);
};

struct QT_Widget : QWidget {
	QT_Widget(string P_Style = "_Default_Widget");
};

struct QT_Button : QPushButton {
	QT_Button(string P_Style = "_Default_Button");
};

struct QT_Dock : QDockWidget {
	QT_Dock(string P_Style = "_Default_Dock");
};

struct QT_File_Browser : QFileDialog {
	QT_File_Browser(string P_Style = "_Default_File_Browser");
};

struct QT_Graphics_View : QGraphicsView {
	QT_Graphics_View();
};

struct QT_Grid_Layout : QGridLayout {
	QT_Grid_Layout(string P_Style = "_Default_Grid_Layout");
};

struct QT_Label : QLabel {
	QT_Label(string P_Style = "_Default_Label");
};

struct QT_Linear_Layout : QHBoxLayout {
	QT_Linear_Layout(bool P_Vertical = true);

	void clear();
};

struct QT_Linear_Contents : QT_Widget {
	QT_Linear_Layout* Layout;

	QT_Linear_Contents(bool P_Vertical);
	QT_Linear_Contents(bool P_Vertical, bool P_Expand_X, bool P_Expand_Y);
	QT_Linear_Contents(string P_Style = "_Default_Linear_Contents", bool P_Vertical = true);
};

struct QT_Toggle : QT_Button {
	QT_Toggle(string P_Style = "_Default_Toggle");
};

struct QT_Floating_Toggle : QT_Toggle {
	QPoint Drag_Pos;

	QT_Floating_Toggle(string P_Style = "_Default_Floating_Toggle");

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
};

struct QT_Drop_Down : QT_Linear_Contents {
	QT_Toggle* Expand_Collapse_Button;
	QT_Linear_Contents* Expand_Contents;

	QT_Drop_Down(string P_Style = "_Default_Dropdown");

	void expandCollapse(bool checked);
	void setWidget(QT_Widget* P_Widget);
	void expand();
	void collapse();
};
struct QT_Main_Window : QMainWindow {
	QT_Main_Window(string P_Style = "_Default_Main_Window");
};
struct QT_Menu : QMenu {
	QT_Menu(string P_Style = "_Default_Menu");
};
struct QT_Option : QComboBox {
	QT_Option(string P_Style = "_Default_Option");
};
struct QT_Scroll_Area : QScrollArea {
	QT_Linear_Layout* Layout;
	QT_Scroll_Area(string P_Style = "_Default_Scroll_Area", bool P_Vertical = true);
	QT_Scroll_Area(bool P_Vertical = true);
};
struct QT_Slider : QSlider {
	QT_Slider(string P_Style = "_Default_Slider", bool P_Vertical = false);
	QT_Slider(bool P_Vertical = false);
};
struct QT_Splitter : QSplitter {
	QT_Splitter(string P_Style = "_Default_Splitter", bool P_Vertical = true);
	QT_Splitter(bool P_Vertical = true);
};
struct QT_Spreadsheet : QTableWidget {
	QT_Spreadsheet(string P_Style = "_Default_Spreadsheet");
};
struct QT_Spreadsheet_Item : QTableWidgetItem {
	QT_Spreadsheet_Item(string P_Value = "Item");
};
struct QT_Tree : QTreeWidget {
	QT_Tree(string P_Style = "_Default_Tree");
};
struct QT_Tree_Item : QTreeWidgetItem {
	QT_Tree_Item(QT_Tree* P_Parent, string Text = "Item");
	QT_Tree_Item(QT_Tree_Item* P_Parent, string Text = "Item");
};
struct QT_Tab_Widget : QTabWidget {
	QT_Tab_Widget(string P_Style = "_Default_Tab_Widget", bool P_Vertical = true);
	QT_Tab_Widget(bool P_Vertical = true);
};
struct QT_Text_Input : QTextEdit {
	QT_Text_Input(string P_Style = "_Default_Text_Input");
};
struct QT_Text_Stream : QTextBrowser {
	QT_Text_Stream(string P_Style = "_Default_Text_Stream");
};
struct QT_ToolBar : QToolBar {
	QT_ToolBar(string P_Style = "_Default_ToolBar");
};
struct QT_Value_Input : QLineEdit {
	QT_Value_Input(string P_Style = "_Default_Value_Input");
};