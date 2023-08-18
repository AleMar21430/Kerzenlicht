#pragma once

#include "Object.h"

#include <QObject>
#include <QThread>
#include <QString>
#include <QFile>

struct Obj_File_Loader : QThread {
	Q_OBJECT
	string File_Path;
	string tex;
	Vec3 pos, scale, rot;
	Fragment_Shader_Type::Enum shader;

public:
	explicit Obj_File_Loader(QObject* parent, string iobj, const Vec3& ipos, const Vec3& irot, const Vec3& iscale, const string itexture, const Fragment_Shader_Type::Enum ishader);
	void run() override;

signals:
	void updateProgress_Signal(int P_Progress);
	void loadingFinished_Signal(Object Output_Mesh_Object);
};