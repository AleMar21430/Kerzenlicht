#include "R_Settings.h"

R_Settings::R_Settings(RUI_Application * Application) {
	App = Application;
	Font = Application->font();
	ResX = Application->primaryScreen()->size().width();
	ResY = Application->primaryScreen()->size().height();
}

uint16_t R_Settings::Size(long double Multiplier) {
	
	if (ResX > ResY) {
		return (ResY / 100.0 * 2.5 * Multiplier);
	}
	else {
		return (ResX / 100.0 * 2.5 * Multiplier);
	}
}