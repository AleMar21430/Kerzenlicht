#include "Main_Window.h"

int main(int argc, char *argv[]) {
	QT_Application* Application = new QT_Application(argc, argv);
	Main_Window* Window = new Main_Window(Application);
	return Application->exec();
}