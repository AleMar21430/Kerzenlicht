#include "RUI_Core.h"
#include "R_Main_Window.h"

int main(int argc, char *argv[]) {
	RUI_Application* Application = new RUI_Application(argc, argv);
	R_Main_Window* Main_Window = new R_Main_Window(Application);
	return Application->exec();
}