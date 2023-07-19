#include "QT_Core.h"
#include "Main_Window.h"

int main(int argc, char *argv[]) {
	std::ofstream log;
	log.open("Log.txt");
	log.close();

	QT_Application* Application = new QT_Application(argc, argv);
	Main_Window* Window = new Main_Window(Application);
	return Application->exec();
}