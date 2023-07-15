#pragma once

#include "RUI_Core.h"
#include <QScreen>
#include <QFont>
#include <QFontMetrics>

struct R_Settings {
	RUI_Application* App;
	uint16_t ResX = 1080;
	uint16_t ResY = 1920;
	QFont Font;

	R_Settings(RUI_Application * App);

	uint16_t Size(long double Multiplier = 1);
};