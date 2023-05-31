#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(Context* context) : context(context), receiver(new Receiver()), x(0)
{
	auto* chart = context->firstWidget->getChart();
	if (chart)
	{
		auto* axisX = qobject_cast<QValueAxis*>(chart->axisX());
		width = qAbs(axisX->max() - axisX->min());
		if (axisX) delete axisX;
		qreal toothCount = 5;
		delta = width / (256*5);
	}
	QObject::connect(context->ui_MainWindow->StopButton, SIGNAL(clicked()), this, SLOT(toggleConnection()));
}

void ConnectionManager::handleData()
{
	auto* series = context->firstWidget->getSeries();
	data = receiver->fetchData();
	if (x > qreal(width)) {
		x = 0;
		series->clear();
	}
	qreal y = data.ampl_tact[0].ampl_us[0].ampl[0].ampl;
	auto newPoint = QPointF{ x , y };
	*series << QPointF{ x , y };

	x += delta;
	qDebug() << newPoint;

	/*qDebug() << data.ampl_tact[0].ampl_us[0].ampl[0].ampl
		<< data.ampl_tact[0].ampl_us[1].ampl[0].ampl
		<< data.ampl_tact[1].ampl_us[0].ampl[0].ampl
		<< data.ampl_tact[1].ampl_us[1].ampl[0].ampl
		<< data.ampl_tact[2].ampl_us[0].ampl[0].ampl
		<< data.ampl_tact[2].ampl_us[1].ampl[0].ampl
		<< data.ampl_tact[3].ampl_us[0].ampl[0].ampl
		<< data.ampl_tact[3].ampl_us[1].ampl[0].ampl;*/
}

void ConnectionManager::toggleConnection()
{
	if (context->connectionActive) {
		receiver->setConnection(QHostAddress("192.168.1.64"), 8080);
		QObject::connect(receiver, SIGNAL(dataReceived()), this, SLOT(handleData()));
	}
	else
	{
		QObject::disconnect(receiver, SIGNAL(dataReceived()), this, SLOT(handleData()));
		receiver->disconnect();
	}
}