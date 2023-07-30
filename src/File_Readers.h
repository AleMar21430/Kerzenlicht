#pragma once

#include "Object.h"

#include <QObject>
#include <QThread>
#include <QString>
#include <QFile>

struct Obj_File_Loader : QThread {
	Q_OBJECT
	string File_Path;
public:
	explicit Obj_File_Loader(QObject* parent = nullptr, string P_File_Path = "");
	void run() override;

signals:
	void updateProgress_Signal(int P_Progress);
	void loadingFinished_Signal(Object Output_Mesh_Object);
};