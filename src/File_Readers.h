#pragma once

#include "Object.h"

#include <QObject>
#include <QThread>
#include <QString>
#include <QFile>

struct Obj_File_Loader : QThread {
	Q_OBJECT
	std::string File_Path;
public:
	explicit Obj_File_Loader(QObject* parent = nullptr, std::string P_File_Path = "");
	void run() override;

public slots:
	void loadObjFile(const std::string P_Filepath);

signals:
	void progressUpdated(int P_Progress);
	void loadingFinished(Object Output_Mesh_Object);
};