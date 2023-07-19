#include "Workspace_Renderer.h"

Workspace_Renderer::Workspace_Renderer(QT_Text_Stream* P_Log) : QT_Linear_Contents("_Container", true) {
	Log = P_Log;
	Renderer = new Kerzenlicht_Renderer(Log);

	QT_Splitter* Splitter = new QT_Splitter(false);

	Splitter->addWidget(Renderer);
	Splitter->addWidget(Renderer->Menu);

	Layout->addWidget(Splitter);
}