#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(Context* context) : context(context), receiver(new Receiver())
{
	QObject::connect(context->ui_MainWindow->StopButton, SIGNAL(clicked()), this, SLOT(toggleConnection()));
	//QObject::connect(context->ui_MainWindow->StopButton, SIGNAL(clicked()), this, SLOT(setConnection()));
}

void ConnectionManager::toggleConnection()
{
	if (context->connectionActive) {
		receiver->setConnection(QHostAddress("192.168.1.64"), 8080);
	}
	else
	{
		receiver->disconnect();
	}
}