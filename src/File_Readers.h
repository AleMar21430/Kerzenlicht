#pragma once

#include "Object.h"

#include <QObject>
#include <QThread>
#include <QString>
#include <QFile>

struct Kerzenlicht_File_Loader : public QObject {
	Q_OBJECT

	Kerzenlicht_File_Loader();

	void updateProgress(uint_fast8_t bytesRead, uint_fast8_t totalBytes);

public slots:
	void loadFile(const std::string filePath);

signals:
	void progressUpdated(uint_fast8_t progress);
	void loadingFinished();
};

struct Obj_File_Loader : public QObject {
	Q_OBJECT
public:
	Obj_File_Loader();

	void updateProgress(int bytesRead, int totalBytes);

public slots:
	void loadObjFile(const std::string filePath);

signals:
	void progressUpdated(int progress);
	void loadingFinished(Object Output_Mesh_Object);
	void loadingFailed();
};