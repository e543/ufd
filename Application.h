#pragma once
#include <QtWidgets/QApplication>

#include "ApplicationController.h"
#include "MainWindow.h"

class Application : public QApplication
{
	ApplicationController* appController;
	MainWindow mainWind;

	Context* context;
	Settings* settings;
public:
	Application(int argc, char* argv[]) : QApplication(argc, argv) 
	{
		appController = new ApplicationController(mainWind.getContext());

		appController->RenderContext();
	};
	const Context* getContext();
	MainWindow& getMainWindow();
};
