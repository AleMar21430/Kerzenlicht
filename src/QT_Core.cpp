#include "QT_Core.h"

QT_Application::QT_Application(int argc, char* argv[]) : QApplication(argc, argv) {
}

QT_Widget::QT_Widget(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QT_Button::QT_Button(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QT_Dock::QT_Dock(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setAllowedAreas(Qt::AllDockWidgetAreas);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QT_File_Browser::QT_File_Browser(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QT_Grid_Layout::QT_Grid_Layout(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(1, 1, 1, 1);
	setSpacing(1);
}

QT_Label::QT_Label(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

QT_Linear_Layout::QT_Linear_Layout(bool P_Vertical) {
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

void QT_Linear_Layout::clear() {
	if (layout() != NULL) {
		QLayoutItem* item;
		while ((item = layout()->takeAt(0)) != NULL) {
			item->widget()->hide();
			item->widget()->deleteLater();
		}
	}
}

QT_Linear_Contents::QT_Linear_Contents(bool P_Vertical) {
	setObjectName("_Default_Linear_Contents");
	Layout = new QT_Linear_Layout(P_Vertical);
	setLayout(Layout);
}

QT_Linear_Contents::QT_Linear_Contents(std::string P_Style, bool P_Vertical) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	Layout = new QT_Linear_Layout(P_Vertical);
	setLayout(Layout);
}

QT_Toggle::QT_Toggle(std::string P_Style) : QT_Button() {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setCheckable(true);
	setChecked(false);
}

QT_Floating_Toggle::QT_Floating_Toggle(std::string P_Style) : QT_Toggle() {
	setObjectName(P_Style);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

void QT_Floating_Toggle::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::RightButton) {
		Drag_Pos = event->pos();
	}
	QPushButton::mousePressEvent(event);
}

void QT_Floating_Toggle::mouseMoveEvent(QMouseEvent* event) {
	if (event->buttons() & Qt::RightButton) {
		move(mapToParent(event->pos() - Drag_Pos));
	}
	QPushButton::mouseMoveEvent(event);
}

QT_Drop_Down::QT_Drop_Down(std::string P_Style) : QT_Linear_Contents(true) {
	setObjectName(P_Style);
	Expand_Collapse_Button = new QT_Toggle();
	Expand_Contents = new QT_Linear_Contents(true);
	Layout->addWidget(Expand_Collapse_Button);
	Layout->addWidget(Expand_Contents);
	Expand_Contents->setFixedHeight(0);
	connect(Expand_Collapse_Button, &QPushButton::clicked, [this](bool checked) {expandCollapse(checked); });
}

void QT_Drop_Down::expandCollapse(bool checked) {
	if (checked) { Expand_Contents->setFixedHeight(Expand_Contents->sizeHint().height()); }
	else { Expand_Contents->setFixedHeight(0); }
}

void QT_Drop_Down::setWidget(QT_Widget* P_Widget) {
	Expand_Contents->Layout->addWidget(P_Widget);
}

void QT_Drop_Down::expand() {
	Expand_Collapse_Button->setChecked(true);
	Expand_Contents->setFixedHeight(Expand_Contents->sizeHint().height());
}

void QT_Drop_Down::collapse() {
	Expand_Collapse_Button->setChecked(false);
	Expand_Contents->setFixedHeight(0);
}

QT_Main_Window::QT_Main_Window(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);

	setDockNestingEnabled(true);
	setTabPosition(Qt::DockWidgetArea::AllDockWidgetAreas, QTabWidget::West);
	setContextMenuPolicy(Qt::NoContextMenu);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QT_Menu::QT_Menu(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QT_Option::QT_Option(std::string P_Style) : QComboBox() {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QT_Scroll_Area::QT_Scroll_Area(std::string P_Style, bool P_Vertical) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	Layout = new QT_Linear_Layout(P_Vertical);
	setWidgetResizable(true);
	setLayout(Layout);
}
QT_Scroll_Area::QT_Scroll_Area(bool P_Vertical) {
	setObjectName("_Default_Scroll_Area");
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	Layout = new QT_Linear_Layout(P_Vertical);
	setWidgetResizable(true);
	setLayout(Layout);
}

QT_Slider::QT_Slider(std::string P_Style, bool P_Vertical) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	if (P_Vertical) { setOrientation(Qt::Vertical); }
	else { setOrientation(Qt::Horizontal); }
}
QT_Slider::QT_Slider(bool P_Vertical) {
	setObjectName("_Default_Slider");
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	if (P_Vertical) { setOrientation(Qt::Vertical); }
	else { setOrientation(Qt::Horizontal); }
}

QT_Splitter::QT_Splitter(std::string P_Style, bool P_Vertical) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	if (P_Vertical) { setOrientation(Qt::Vertical); }
	else { setOrientation(Qt::Horizontal); }
}
QT_Splitter::QT_Splitter(bool P_Vertical) {
	setObjectName("_Default_Splitter");
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	if (P_Vertical) { setOrientation(Qt::Vertical); }
	else { setOrientation(Qt::Horizontal); }
}

QT_Spreadsheet::QT_Spreadsheet(std::string P_Style) {
	setObjectName(P_Style);
	this->horizontalHeader()->setObjectName("_Horizontal");
	this->verticalHeader()->setObjectName("_Vertical");
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

QT_Spreadsheet_Item::QT_Spreadsheet_Item(std::string P_Value) {
	setText(QString::fromUtf8(P_Value));
}

QT_Tree::QT_Tree(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	setRootIsDecorated(false);
	setHeaderHidden(true);
}

QT_Tree_Item::QT_Tree_Item(QT_Tree* P_Parent, std::string Text) : QTreeWidgetItem(P_Parent) {
	setText(0, QString::fromUtf8(Text));
}
QT_Tree_Item::QT_Tree_Item(QT_Tree_Item* P_Parent, std::string Text) : QTreeWidgetItem(P_Parent) {
	setText(0, QString::fromUtf8(Text));
}

QT_Tab_Widget::QT_Tab_Widget(std::string P_Style, bool P_Vertical) {
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
QT_Tab_Widget::QT_Tab_Widget(bool P_Vertical) {
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

QT_Text_Input::QT_Text_Input(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

QT_Text_Stream::QT_Text_Stream(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}

void QT_Text_Stream::concat(std::string Text) {
	moveCursor(QTextCursor::End);
	insertPlainText(QString::fromUtf8(Text));
}

QT_ToolBar::QT_ToolBar(std::string P_Style) {
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

QT_Value_Input::QT_Value_Input(std::string P_Style) {
	setObjectName(P_Style);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}