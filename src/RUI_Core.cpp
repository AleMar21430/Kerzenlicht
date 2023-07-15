#include "RUI_Core.h"

RUI_Application::RUI_Application(int argc, char* argv[]) : QApplication(argc, argv) {
}

RUI_Widget::RUI_Widget(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

RUI_Button::RUI_Button(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

RUI_Dock::RUI_Dock(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setAllowedAreas(Qt::AllDockWidgetAreas);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

RUI_File_Browser::RUI_File_Browser(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

RUI_Grid_Layout::RUI_Grid_Layout(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(1, 1, 1, 1);
	setSpacing(1);
}

RUI_Label::RUI_Label(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

RUI_Linear_Layout::RUI_Linear_Layout(bool P_Vertical) {
	setSpacing(1);
	setContentsMargins(1, 1, 1, 1);
	if (P_Vertical) {
		setDirection(QBoxLayout::Direction::TopToBottom);
		setAlignment(Qt::AlignTop);
	}
	else {
		setDirection(QBoxLayout::Direction::LeftToRight);
		setAlignment(Qt::AlignLeft);
	}
}

void RUI_Linear_Layout::clear() {
	if (layout() != NULL) {
		QLayoutItem* item;
		while ((item = layout()->takeAt(0)) != NULL) {
			item->widget()->hide();
			item->widget()->deleteLater();
		}
	}
}

RUI_Linear_Contents::RUI_Linear_Contents(bool P_Vertical) {
	setObjectName("_Default_Linear_Contents");
	Layout = new RUI_Linear_Layout(P_Vertical);
	setLayout(Layout);
}

RUI_Linear_Contents::RUI_Linear_Contents(std::string P_Style, bool P_Vertical) {
	setObjectName(P_Style);
	Layout = new RUI_Linear_Layout(P_Vertical);
	setLayout(Layout);
}

RUI_Toggle::RUI_Toggle(std::string P_Style) : RUI_Button() {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setCheckable(true);
	setChecked(false);
}

RUI_Floating_Toggle::RUI_Floating_Toggle(std::string P_Style) : RUI_Toggle() {
	setObjectName(P_Style);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

void RUI_Floating_Toggle::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::RightButton) {
		Drag_Pos = event->pos();
	}
	QPushButton::mousePressEvent(event);
}

void RUI_Floating_Toggle::mouseMoveEvent(QMouseEvent* event) {
	if (event->buttons() & Qt::RightButton) {
		move(mapToParent(event->pos() - Drag_Pos));
	}
	QPushButton::mouseMoveEvent(event);
}

RUI_Drop_Down::RUI_Drop_Down(std::string P_Style) : RUI_Linear_Contents(true) {
	setObjectName(P_Style);
	Expand_Collapse_Button = new RUI_Toggle();
	Expand_Contents = new RUI_Linear_Contents(true);
	Layout->addWidget(Expand_Collapse_Button);
	Layout->addWidget(Expand_Contents);
	Expand_Contents->setFixedHeight(0);
	connect(Expand_Collapse_Button, &QPushButton::clicked, [this](bool checked) {expandCollapse(checked); });
}

void RUI_Drop_Down::expandCollapse(bool checked) {
	if (checked) { Expand_Contents->setFixedHeight(Expand_Contents->sizeHint().height()); }
	else { Expand_Contents->setFixedHeight(0); }
}

void RUI_Drop_Down::setWidget(RUI_Widget* P_Widget) {
	Expand_Contents->Layout->addWidget(P_Widget);
}

void RUI_Drop_Down::expand() {
	Expand_Collapse_Button->setChecked(true);
	Expand_Contents->setFixedHeight(Expand_Contents->sizeHint().height());
}

void RUI_Drop_Down::collapse() {
	Expand_Collapse_Button->setChecked(false);
	Expand_Contents->setFixedHeight(0);
}

RUI_Main_Window::RUI_Main_Window(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);

	setDockNestingEnabled(true);
	setTabPosition(Qt::DockWidgetArea::AllDockWidgetAreas, QTabWidget::West);
	setContextMenuPolicy(Qt::NoContextMenu);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

RUI_Menu::RUI_Menu(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

RUI_Option::RUI_Option(std::string P_Style) : QComboBox() {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

RUI_Scroll_Area::RUI_Scroll_Area(std::string P_Style, bool P_Vertical) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	Layout = new RUI_Linear_Layout(P_Vertical);
	setWidgetResizable(true);
	setLayout(Layout);
}
RUI_Scroll_Area::RUI_Scroll_Area(bool P_Vertical) {
	setObjectName("_Default_Scroll_Area");
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	Layout = new RUI_Linear_Layout(P_Vertical);
	setWidgetResizable(true);
	setLayout(Layout);
}

RUI_Slider::RUI_Slider(std::string P_Style, bool P_Vertical) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	if (P_Vertical) { setOrientation(Qt::Vertical); }
	else { setOrientation(Qt::Horizontal); }
}
RUI_Slider::RUI_Slider(bool P_Vertical) {
	setObjectName("_Default_Slider");
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	if (P_Vertical) { setOrientation(Qt::Vertical); }
	else { setOrientation(Qt::Horizontal); }
}

RUI_Splitter::RUI_Splitter(std::string P_Style, bool P_Vertical) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	if (P_Vertical) { setOrientation(Qt::Vertical); }
	else { setOrientation(Qt::Horizontal); }
}
RUI_Splitter::RUI_Splitter(bool P_Vertical) {
	setObjectName("_Default_Splitter");
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	if (P_Vertical) { setOrientation(Qt::Vertical); }
	else { setOrientation(Qt::Horizontal); }
}

RUI_Spreadsheet::RUI_Spreadsheet(std::string P_Style) {
	setObjectName(P_Style);
	this->horizontalHeader()->setObjectName("_Horizontal");
	this->verticalHeader()->setObjectName("_Vertical");
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

RUI_Spreadsheet_Item::RUI_Spreadsheet_Item(std::string P_Value) {
	setText(QString::fromUtf8(P_Value));
}

RUI_Tree::RUI_Tree(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	setRootIsDecorated(false);
	setHeaderHidden(true);
}

RUI_Tree_Item::RUI_Tree_Item(RUI_Tree* P_Parent, std::string Text) : QTreeWidgetItem(P_Parent) {
	setText(0, QString::fromUtf8(Text));
}
RUI_Tree_Item::RUI_Tree_Item(RUI_Tree_Item* P_Parent, std::string Text) : QTreeWidgetItem(P_Parent) {
	setText(0, QString::fromUtf8(Text));
}

RUI_Tab_Widget::RUI_Tab_Widget(std::string P_Style, bool P_Vertical) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setTabsClosable(false);
	if (P_Vertical) {
		setTabPosition(QTabWidget::North);
	}
	else {
		setTabPosition(QTabWidget::West);
	}
}
RUI_Tab_Widget::RUI_Tab_Widget(bool P_Vertical) {
	setObjectName("_Default_Tab_Widget");
	setContentsMargins(0, 0, 0, 0);
	setTabsClosable(false);
	if (P_Vertical) {
		setTabPosition(QTabWidget::North);
	}
	else {
		setTabPosition(QTabWidget::West);
	}
}

RUI_Text_Input::RUI_Text_Input(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

RUI_Text_Stream::RUI_Text_Stream(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}

void RUI_Text_Stream::concat(std::string Text) {
	moveCursor(QTextCursor::End);
	insertPlainText(QString::fromUtf8(Text));
}

RUI_ToolBar::RUI_ToolBar(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	layout()->setContentsMargins(1, 1, 1, 1);
	layout()->setSpacing(1);
	setFloatable(false);
	setMovable(false);
	setContextMenuPolicy(Qt::PreventContextMenu);
	setLayoutDirection(Qt::LeftToRight);
}

RUI_Value_Input::RUI_Value_Input(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}