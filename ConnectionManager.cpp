#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(Context* context) : context(context), receiver(new Receiver())
{
	auto* chart = context->firstWidget->getChart();
	if (chart)
	{
		auto* axisX = qobject_cast<QValueAxis*>(chart->axisX());
		width = qAbs(axisX->max() - axisX->min());
		qreal toothCount = 5;
		delta = width / (256*5);
	}
	QObject::connect(context->ui_MainWindow->StopButton, SIGNAL(clicked()), this, SLOT(toggleConnection()));
}

void ConnectionManager::resetChart()
{
	i = 0;
	x = 0;
	context->firstWidget->getSeries()->clear();
}

void ConnectionManager::handleData()
{
	auto* series = context->firstWidget->getSeries();
	osc = receiver->fetchData();

	/*if (x > qreal(width)) {
		context->channelChanged = false;
		resetChart();
	}

	if (context->channelChanged) {
		context->channelChanged = false;
		resetChart();
	}*/
	resetChart();
	if (context->channelSelected){
		for (int i = 0; i < 256, x < width; ++i, x += delta)
			series->append(QPointF{ x, qreal(osc[i]) });
	}
}

void ConnectionManager::toggleConnection()
{
	if (context->connectionActive) {
		receiver->setConnection(QHostAddress("192.168.1.64"), 8080);
		QObject::connect(receiver, SIGNAL(dataReceived()), this, SLOT(handleData()));
		resetChart();
	}
	else
	{
		QObject::disconnect(receiver, SIGNAL(dataReceived()), this, SLOT(handleData()));
		receiver->disconnect();
	}
}