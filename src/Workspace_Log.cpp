#include "Workspace_Log.h"

Workspace_Log::Workspace_Log(QT_Text_Stream* P_Log) : QT_Linear_Contents("_Container", true) {
	Log = P_Log;

	QT_Linear_Contents* Options = new QT_Linear_Contents(false, true, false);
	QT_Button* Clear_Button = new QT_Button();
	Clear_Button->setText("Clear");
	connect(Clear_Button, &QT_Button::clicked, [this]() {Log->clear(); });

	QT_Button* Export_Button = new QT_Button();
	Export_Button->setText("Export");
	connect(Export_Button, &QT_Button::clicked, [this]() {exportLog(); });

	Options->Layout->addWidget(Clear_Button);
	Options->Layout->addWidget(Export_Button);

	this->Layout->addWidget(Options);
	this->Layout->addWidget(Log);
}

void Workspace_Log::append(string P_Text) {
	Log->append(QString::fromStdString(P_Text));
}

void Workspace_Log::exportLog() {
	/*ofstream file(QT_File_Browser::getSaveFileName(nullptr,"Save Log").toStdString());
	if (file.is_open()){
		file << "Log->toPlainText().toStdString()";
		file.close();
	}*/ //TODO Fix Crash
}