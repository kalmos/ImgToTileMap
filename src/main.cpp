//
// main.cpp for WallConvert
//
// Made by lucas stefas
// Login   stefas_l
//
// Started on 09/01/2014 12:13 2014 by lucas stefas
//

#include <ui_imgConvert.h>
#include "Moc/MainWindow.h"
#include <QtConcurrentRun>
#if defined Windows
#	include <Windows.h>
#endif

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

	std::string path;
	QApplication  app(argc, argv);
	MainWindow widget(path.c_str());
	
	widget.show();
	return app.exec();
	
	return (0);
}

#if defined Windows
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	return (main(0, 0));
}
#endif