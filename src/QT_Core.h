#pragma once

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtOpenGLWidgets>

struct QT_Application;		// Application
struct QT_Button;			// Push Button
struct QT_Dock;				// Dock Widget
struct QT_Drop_Down;		// Button Switch That Expands And Collapses Contents
struct QT_File_Browser;		// Popup Window To Select File
struct QT_Grid_Layout;		// Layout
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
	QT_Widget(std::string P_Style = "_Default_Widget");
};

struct QT_Button : QPushButton {
	QT_Button(std::string P_Style = "_Default_Button");
};

struct QT_Dock : QDockWidget {
	QT_Dock(std::string P_Style = "_Default_Dock");
};

struct QT_File_Browser : QFileDialog {
	QT_File_Browser(std::string P_Style = "_Default_File_Browser");
};

struct QT_Grid_Layout : QGridLayout {
	QT_Grid_Layout(std::string P_Style = "_Default_Grid_Layout");
};

struct QT_Label : QLabel {
	QT_Label(std::string P_Style = "_Default_Label");
};

struct QT_Linear_Layout : QHBoxLayout {
	QT_Linear_Layout(bool P_Vertical = true);

	void clear();
};

struct QT_Linear_Contents : QT_Widget {
	QT_Linear_Layout* Layout;

	QT_Linear_Contents(bool P_Vertical = true);
	QT_Linear_Contents(std::string P_Style = "_Default_Linear_Contents", bool P_Vertical = true);
};

struct QT_Toggle : QT_Button {
	QT_Toggle(std::string P_Style = "_Default_Toggle");
};

struct QT_Floating_Toggle : QT_Toggle {
	QPoint Drag_Pos;

	QT_Floating_Toggle(std::string P_Style = "_Default_Floating_Toggle");

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
};

struct QT_Drop_Down : QT_Linear_Contents {
	QT_Toggle* Expand_Collapse_Button;
	QT_Linear_Contents* Expand_Contents;

	QT_Drop_Down(std::string P_Style = "_Default_Dropdown");

	void expandCollapse(bool checked);
	void setWidget(QT_Widget* P_Widget);
	void expand();
	void collapse();
};
struct QT_Main_Window : QMainWindow {
	QT_Main_Window(std::string P_Style = "_Default_Main_Window");
};
struct QT_Menu : QMenu {
	QT_Menu(std::string P_Style = "_Default_Menu");
};
struct QT_Option : QComboBox {
	QT_Option(std::string P_Style = "_Default_Option");
};
struct QT_Scroll_Area : QScrollArea {
	QT_Linear_Layout* Layout;
	QT_Scroll_Area(std::string P_Style = "_Default_Scroll_Area", bool P_Vertical = true);
	QT_Scroll_Area(bool P_Vertical = true);
};
struct QT_Slider : QSlider {
	QT_Slider(std::string P_Style = "_Default_Slider", bool P_Vertical = false);
	QT_Slider(bool P_Vertical = false);
};
struct QT_Splitter : QSplitter {
	QT_Splitter(std::string P_Style = "_Default_Splitter", bool P_Vertical = true);
	QT_Splitter(bool P_Vertical = true);
};
struct QT_Spreadsheet : QTableWidget {
	QT_Spreadsheet(std::string P_Style = "_Default_Spreadsheet");
};
struct QT_Spreadsheet_Item : QTableWidgetItem {
	QT_Spreadsheet_Item(std::string P_Value = "Item");
};
struct QT_Tree : QTreeWidget {
	QT_Tree(std::string P_Style = "_Default_Tree");
};
struct QT_Tree_Item : QTreeWidgetItem {
	QT_Tree_Item(QT_Tree* P_Parent, std::string Text = "Item");
	QT_Tree_Item(QT_Tree_Item* P_Parent, std::string Text = "Item");
};
struct QT_Tab_Widget : QTabWidget {
	QT_Tab_Widget(std::string P_Style = "_Default_Tab_Widget", bool P_Vertical = true);
	QT_Tab_Widget(bool P_Vertical = true);
};
struct QT_Text_Input : QTextEdit {
	QT_Text_Input(std::string P_Style = "_Default_Text_Input");
};
struct QT_Text_Stream : QTextBrowser {
	QT_Text_Stream(std::string P_Style = "_Default_Text_Stream");

	void concat(std::string Text);
};
struct QT_ToolBar : QToolBar {
	QT_ToolBar(std::string P_Style = "_Default_ToolBar");
};
struct QT_Value_Input : QLineEdit {
	QT_Value_Input(std::string P_Style = "_Default_Value_Input");
};