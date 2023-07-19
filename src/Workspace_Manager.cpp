#include "Workspace_Manager.h"

Workspace_Manager::Workspace_Manager(QT_Text_Stream* P_Log, Main_Window* P_Parent): QT_Dock() {
	Log = P_Log;
	Parent = P_Parent;

	Titlebar = new Workspace_Dock_Header(Log, this);
	setTitleBarWidget(Titlebar);
	setWidget(new QT_Linear_Contents("_Container", true));
	setWindowTitle("Workspace");
}

Workspace_Dock_Header::Workspace_Dock_Header(QT_Text_Stream* P_Log, Workspace_Manager* P_Parent) : QT_Linear_Contents(false) {
	Log = P_Log;
	Parent = P_Parent;
	setObjectName("_Horizontal_Linear_Contents");

	Expand_Collapse_Toggle = new QT_Floating_Toggle("Floating_Toggle");
	Workspace_Type_Button = new QT_Button("Icon");
	More_Settings_Toggle = new QT_Toggle("Toggle");
	Add_Dock_Button = new QT_Button("Icon");
	Add_Tab_Button = new QT_Button("Icon");
	Floating_Toggle = new QT_Toggle("Toggle");
	Collapsible_Mode_Toggle = new QT_Toggle("Toggle");
	Close_Dock_Button = new QT_Button("Icon");

	Expand_Collapse_Toggle->setIcon(QPixmap("resources/Icons/Collapse.png"));
	Workspace_Type_Button->setIcon(QPixmap("resources/Icons/workspaces/Dock_Manager.png"));
	More_Settings_Toggle->setIcon(QPixmap("resources/Icons/Settings.png"));
	Add_Dock_Button->setIcon(QPixmap("resource/Iconss/Add.png"));
	Add_Tab_Button->setIcon(QPixmap("resources/Icons/Add_Tab.png"));
	Floating_Toggle->setIcon(QPixmap("resources/Icons/Float_Dock.png"));
	Collapsible_Mode_Toggle->setIcon(QPixmap("resources/Icons/Collapse.png"));
	Close_Dock_Button->setIcon(QPixmap("resources/Icons/Close.png"));

	connect(Expand_Collapse_Toggle, &QT_Toggle::clicked, [this](bool checked) {expandCollapseToggle(checked); });
	connect(Workspace_Type_Button, &QT_Button::clicked, [this]() {selectWorkspaceTypeClick(); });
	connect(More_Settings_Toggle, &QT_Toggle::clicked, [this](bool checked) {showHideSettings(checked); });
	connect(Add_Dock_Button, &QT_Button::clicked, [this]() {addDockClick(); });
	connect(Add_Tab_Button, &QT_Button::clicked, [this]() {addDockTabClick(); });
	connect(Floating_Toggle, &QT_Toggle::clicked, [this](bool checked) {setFloatingToggle(checked); });
	connect(Collapsible_Mode_Toggle, &QT_Toggle::clicked, [this](bool checked) {setCollapsibleModeToggle(checked); });
	connect(Close_Dock_Button, &QT_Button::clicked, [this]() {closeClick(); });

	Expand_Collapse_Toggle->setChecked(true);
	Expand_Collapse_Toggle->setText("Manager");
	this->Layout->addWidget(Workspace_Type_Button);
	this->Layout->addWidget(More_Settings_Toggle);
	this->Layout->addWidget(Add_Dock_Button);
	this->Layout->addWidget(Add_Tab_Button);
	this->Layout->addWidget(Floating_Toggle);
	this->Layout->addWidget(Collapsible_Mode_Toggle);
	this->Layout->addWidget(Close_Dock_Button);

	More_Settings_Toggle->setChecked(true);
	Collapsible_Mode_Toggle->setCheckable(false);

	initWorkspacesMenu();
	initCloseConfirmationMenu();
}

void Workspace_Dock_Header::initWorkspacesMenu() {
	Workspace_Menu = new QT_Menu();
	
	QT_Linear_Layout* Layout = new QT_Linear_Layout();
	
	QT_Button* Viewport_3D_Button = new QT_Button("Item");
	QT_Button* Log_Button = new QT_Button("Item");

	//Viewport_3D_Button->setIcon(QIcon(""));
	Viewport_3D_Button->setText(" Renderer");
	connect(Viewport_3D_Button, &QT_Button::clicked, [this]() {setWorkspace("Renderer"); });

	//Log_Button->setIcon(QIcon(""));
	Log_Button->setText(" Log");
	connect(Log_Button, &QT_Button::clicked, [this]() {setWorkspace("Log"); });

	Layout->addWidget(Viewport_3D_Button);
	Layout->addWidget(Log_Button);

	Workspace_Menu->setLayout(Layout);
}

void Workspace_Dock_Header::selectWorkspaceTypeClick() {
	Workspace_Menu->exec(mapToGlobal(QPoint(Workspace_Type_Button->pos().x(), Workspace_Type_Button->pos().y() + 25)));
}

void Workspace_Dock_Header::setWorkspace(std::string P_Type) {
	if (Parent->windowTitle() != "Log") {
		try {Parent->widget()->deleteLater(); } catch(...) {}
	}
	else {
		Parent->widget()->setParent(Parent->Parent);
	}
	if (P_Type == "Renderer") {
		Parent->setWindowTitle("Renderer");
		//Workspace_Type_Button->setIcon(QIcon("resources/workspaces/Viewport_3D.png"));
		Parent->setWidget((new Kerzenlicht_Renderer(Log)));
		Expand_Collapse_Toggle->setText("Renderer");
	}
	else if (P_Type == "Log") {
		Parent->setWindowTitle("Log");
		//Workspace_Type_Button->setIcon(QIcon("resources/workspaces/Log.png"));
		Parent->setWidget(new Workspace_Log(Log));
		Expand_Collapse_Toggle->setText("Log");
	}
	Workspace_Menu->close();
}

void Workspace_Dock_Header::showHideSettings(bool P_Toggle) {
	if (P_Toggle) {
		Add_Dock_Button->show();
		Add_Tab_Button->show();
		Floating_Toggle->show();
		Collapsible_Mode_Toggle->show();
		Close_Dock_Button->show();
	}
	else {
		Add_Dock_Button->hide();
		Add_Tab_Button->hide();
		Floating_Toggle->hide();
		Collapsible_Mode_Toggle->hide();
		Close_Dock_Button->hide();
	}
}

void Workspace_Dock_Header::initCloseConfirmationMenu() {
	Close_Confirmation_Menu = new QT_Menu();

	QT_Linear_Layout* Layout = new QT_Linear_Layout();

	QT_Button* Close_Button = new QT_Button("Option");
	Close_Button->setText("Close");
	connect(Close_Button, &QT_Button::clicked, [this]() {closeDock(); });

	QT_Button* Cancel_Button = new QT_Button("Option");
	Cancel_Button->setText("Cancel");
	connect(Cancel_Button, &QT_Button::clicked, [this]() {Close_Confirmation_Menu->close(); });

	Layout->addWidget(Close_Button);
	Layout->addWidget(Cancel_Button);

	Close_Confirmation_Menu->setLayout(Layout);
}

void Workspace_Dock_Header::closeClick() {
	int PosX = (mapToGlobal(Parent->widget()->geometry().center()).x() - Close_Confirmation_Menu->layout()->sizeHint().width() / 2);
	int PosY = (mapToGlobal(Parent->widget()->geometry().center()).y() - Close_Confirmation_Menu->layout()->sizeHint().height() / 2);
	Close_Confirmation_Menu->exec(QPoint(PosX,PosY));
}

void Workspace_Dock_Header::closeDock() {
	Close_Confirmation_Menu->close();
	if (Parent->windowTitle() == "Log") {
		Parent->widget()->setParent(Parent->Parent);
	}
	Parent->Parent->removeDockWidget(Parent);
	Expand_Collapse_Toggle->deleteLater();
	Parent->deleteLater();
	deleteLater();
}

void Workspace_Dock_Header::addDockClick() {
	Workspace_Manager* New_Dock = new Workspace_Manager(Log, Parent->Parent);
	Parent->Parent->addDockWidget(Qt::TopDockWidgetArea, New_Dock);
	New_Dock->setFloating(true);
	dynamic_cast<Workspace_Dock_Header*>(New_Dock->Titlebar)->More_Settings_Toggle->setChecked(true);
	dynamic_cast<Workspace_Dock_Header*>(New_Dock->Titlebar)->showHideSettings(true);
	New_Dock->setGeometry(QRect(mapToGlobal(QPoint(Parent->widget()->geometry().center().x() - 100, Parent->widget()->geometry().center().y() - 100)), mapToGlobal(QPoint(Parent->widget()->geometry().center().x() + 100, Parent->widget()->geometry().center().y() + 100))));
}

void Workspace_Dock_Header::addDockTabClick() {
	Workspace_Manager* New_Dock = new Workspace_Manager(Log, Parent->Parent);
	Parent->Parent->tabifyDockWidget(Parent, New_Dock);
}

void Workspace_Dock_Header::setFloatingToggle(bool P_Toggle) {
	if (P_Toggle) {
		//Float
		Collapsible_Mode_Toggle->setCheckable(true);
		Parent->setFloating(true);
		Parent->setAllowedAreas(Qt::NoDockWidgetArea);
		Floating_Toggle->setIcon(QPixmap("resources/Redock_Dock.png"));
	}
	else {
		//Dock
		Collapsible_Mode_Toggle->setCheckable(false);
		Collapsible_Mode_Toggle->setChecked(false);
		setCollapsibleModeToggle(false);
		Parent->setFloating(false);
		Parent->setAllowedAreas(Qt::AllDockWidgetAreas);
		Floating_Toggle->setIcon(QPixmap("resources/Float_Dock.png"));
	}
}

void Workspace_Dock_Header::setCollapsibleModeToggle(bool P_Toggle) {
	if (P_Toggle) {
		Expand_Collapse_Toggle->show();
		Expand_Collapse_Toggle->setGeometry(QRect(QPoint(mapToGlobal(Parent->rect().topLeft()).x() - Expand_Collapse_Toggle->sizeHint().width(), mapToGlobal(Parent->rect().topLeft()).y() - Expand_Collapse_Toggle->sizeHint().height()), mapToGlobal(Parent->rect().topLeft())));
		//TODO if window closed when button is active it will keep showing
		//TODO alt tab keeps buttons on top
	}
	else {
		Expand_Collapse_Toggle->hide();
	}
}

void Workspace_Dock_Header::expandCollapseToggle(bool P_Toggle) {
	if (P_Toggle) { //Expand
		Expand_Collapse_Toggle->setIcon(QPixmap("resources/Collapse.png"));
		Parent->show();
		Parent->restoreGeometry(Stored_Expanded);
	}
	else { //Collapse
		Expand_Collapse_Toggle->setIcon(QPixmap("resources/Expand.png"));
		Stored_Expanded = Parent->saveGeometry();
		Parent->hide();
	}
}

void Workspace_Dock_Header::mouseDoubleClickEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
	}
}