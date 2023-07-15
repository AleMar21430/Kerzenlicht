#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

struct RUI_Application;			// Application
struct RUI_Button;				// Push Button
struct RUI_Dock;				// Dock Widget
struct RUI_Drop_Down;			// Button Switch That Expands And Collapses Contents
struct RUI_File_Browser;		// Popup Window To Select File
struct RUI_Grid_Layout;			// Layout
struct RUI_Label;				// Widget With Text
struct RUI_Linear_Layout;		// Linear Layout
struct RUI_Linear_Contents;		// Widget With Linear Layout
struct RUI_Main_Window;			// Windows Window
struct RUI_Menu;				// Popup|Context Menu
struct RUI_Option;
struct RUI_Scroll_Area;			// Linear Layout With Scrollbar If Content Overflows
struct RUI_Slider;				// Integer Slider
struct RUI_Splitter;			// User Resizable Layout
struct RUI_Spreadsheet;
struct RUI_Spreadsheet_Item;
struct RUI_Tree;
struct RUI_Tree_Item;
struct RUI_Text_Input;			// HTML Text Input
struct RUI_Text_Stream;			// HTML Text Output
struct RUI_Toggle;				// Button Switch
struct RUI_ToolBar;				// File|Edit|Settings|etc... Toolbar
struct RUI_Value_Input;			// String|Float|Integer Text Input
struct RUI_Widget;				// Widget

struct RUI_Application : QApplication {
	RUI_Application(int argc, char* argv[]);
};

struct RUI_Widget : QWidget {
	RUI_Widget(std::string P_Style = "_Default_Widget");
};

struct RUI_Button : QPushButton {
	RUI_Button(std::string P_Style = "_Default_Button");
};

struct RUI_Dock : QDockWidget {
	RUI_Dock(std::string P_Style = "_Default_Dock");
};

struct RUI_File_Browser : QFileDialog {
	RUI_File_Browser(std::string P_Style = "_Default_File_Browser");
};

struct RUI_Grid_Layout : QGridLayout {
	RUI_Grid_Layout(std::string P_Style = "_Default_Grid_Layout");
};

struct RUI_Label : QLabel {
	RUI_Label(std::string P_Style = "_Default_Label");
};

struct RUI_Linear_Layout : QHBoxLayout {
	RUI_Linear_Layout(bool P_Vertical = true);

	void clear();
};

struct RUI_Linear_Contents : RUI_Widget {
	RUI_Linear_Layout* Layout;

	RUI_Linear_Contents(bool P_Vertical = true);
	RUI_Linear_Contents(std::string P_Style = "_Default_Linear_Contents", bool P_Vertical = true);
};

struct RUI_Toggle : RUI_Button {
	RUI_Toggle(std::string P_Style = "_Default_Toggle");
};

struct RUI_Floating_Toggle : RUI_Toggle {
	QPoint Drag_Pos;

	RUI_Floating_Toggle(std::string P_Style = "_Default_Floating_Toggle");

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
};

struct RUI_Drop_Down : RUI_Linear_Contents {
	RUI_Toggle* Expand_Collapse_Button;
	RUI_Linear_Contents* Expand_Contents;

	RUI_Drop_Down(std::string P_Style = "_Default_Dropdown");

	void expandCollapse(bool checked);
	void setWidget(RUI_Widget* P_Widget);
	void expand();
	void collapse();
};
struct RUI_Main_Window : QMainWindow {
	RUI_Main_Window(std::string P_Style = "_Default_Main_Window");
};
struct RUI_Menu : QMenu {
	RUI_Menu(std::string P_Style = "_Default_Menu");
};
struct RUI_Option : QComboBox {
	RUI_Option(std::string P_Style = "_Default_Option");
};
struct RUI_Scroll_Area : QScrollArea {
	RUI_Linear_Layout* Layout;
	RUI_Scroll_Area(std::string P_Style = "_Default_Scroll_Area", bool P_Vertical = true);
	RUI_Scroll_Area(bool P_Vertical = true);
};
struct RUI_Slider : QSlider {
	RUI_Slider(std::string P_Style = "_Default_Slider", bool P_Vertical = false);
	RUI_Slider(bool P_Vertical = false);
};
struct RUI_Splitter : QSplitter {
	RUI_Splitter(std::string P_Style = "_Default_Splitter", bool P_Vertical = true);
	RUI_Splitter(bool P_Vertical = true);
};
struct RUI_Spreadsheet : QTableWidget {
	RUI_Spreadsheet(std::string P_Style = "_Default_Spreadsheet");
};
struct RUI_Spreadsheet_Item : QTableWidgetItem {
	RUI_Spreadsheet_Item(std::string P_Value = "Item");
};
struct RUI_Tree : QTreeWidget {
	RUI_Tree(std::string P_Style = "_Default_Tree");
};
struct RUI_Tree_Item : QTreeWidgetItem {
	RUI_Tree_Item(RUI_Tree* P_Parent, std::string Text = "Item");
	RUI_Tree_Item(RUI_Tree_Item* P_Parent, std::string Text = "Item");
};
struct RUI_Tab_Widget : QTabWidget {
	RUI_Tab_Widget(std::string P_Style = "_Default_Tab_Widget", bool P_Vertical = true);
	RUI_Tab_Widget(bool P_Vertical = true);
};
struct RUI_Text_Input : QTextEdit {
	RUI_Text_Input(std::string P_Style = "_Default_Text_Input");
};
struct RUI_Text_Stream : QTextBrowser {
	RUI_Text_Stream(std::string P_Style = "_Default_Text_Stream");

	void concat(std::string Text);
};
struct RUI_ToolBar : QToolBar {
	RUI_ToolBar(std::string P_Style = "_Default_ToolBar");
};
struct RUI_Value_Input : QLineEdit {
	RUI_Value_Input(std::string P_Style = "_Default_Value_Input");
};