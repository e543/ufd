#include "Application.h"

const Context* Application::getContext()
{
	return context;
}

MainWindow& Application::getMainWindow()
{
	return mainWind;
}
