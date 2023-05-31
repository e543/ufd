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
	x = 0;
	context->firstWidget->getSeries()->clear();
}

void ConnectionManager::handleData()
{
	auto* series = context->firstWidget->getSeries();
	context->data = receiver->fetchData();
	auto data = context->data;
	qreal y = 0;

	if (x > qreal(width)) {
		context->channelChanged = false;
		resetChart();
	}

	if (context->channelChanged) {
		context->channelChanged = false;
		resetChart();
	}

	if (context->channelSelected) {
		switch (context->selectedChannel)
		{
		default:
			y = 0;
			break;
		case 0:
			y = data.ampl_tact[0].ampl_us[0].ampl[0].ampl;
			break;
		case 1:
			y = data.ampl_tact[0].ampl_us[1].ampl[0].ampl;
			break;
		case 2:
			y = data.ampl_tact[1].ampl_us[0].ampl[0].ampl;
			break;
		case 3:
			y = data.ampl_tact[1].ampl_us[1].ampl[0].ampl;
			break;
		case 4:
			y = data.ampl_tact[2].ampl_us[0].ampl[0].ampl;
			break;
		case 5:
			y = data.ampl_tact[2].ampl_us[1].ampl[0].ampl;
			break;
		case 6:
			y = data.ampl_tact[3].ampl_us[0].ampl[0].ampl;
			break;
		case 7:
			y = data.ampl_tact[3].ampl_us[1].ampl[0].ampl;
			break;
		}
		auto newPoint = QPointF{ x , y };
		*series << QPointF{ x , y };
		x += delta;
	}


	qDebug() << data.ampl_tact[0].ampl_us[0].ampl[0].ampl
		<< data.ampl_tact[0].ampl_us[1].ampl[0].ampl
		<< data.ampl_tact[1].ampl_us[0].ampl[0].ampl
		<< data.ampl_tact[1].ampl_us[1].ampl[0].ampl
		<< data.ampl_tact[2].ampl_us[0].ampl[0].ampl
		<< data.ampl_tact[2].ampl_us[1].ampl[0].ampl
		<< data.ampl_tact[3].ampl_us[0].ampl[0].ampl
		<< data.ampl_tact[3].ampl_us[1].ampl[0].ampl;
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