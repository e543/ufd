#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(Context* context) : context(context), receiver(new Receiver())
{
	series = context->firstWidget->getSeries();

	QObject::connect(context->timer, &QTimer::timeout,
		this, &ConnectionManager::dataTimer);

	auto* chart = context->firstWidget->getChart();
	if (chart)
	{
		auto* axisX = qobject_cast<QValueAxis*>(chart->axisX());
		width = qAbs(axisX->max() - axisX->min());
		qreal toothCount = 5;
		delta = width / 256;
	}
	qreal x = 0;
	for (int i = 0; i < 256; ++i) {
		points.append(QPointF{ x , qreal(i) });
		x += delta;
	}
	//QObject::connect(context->ui_MainWindow->StopButton, SIGNAL(clicked()), this, SLOT(toggleConnection()));
}

void ConnectionManager::resetChart()
{
	x = 0;
	context->firstWidget->getSeries()->clear();
}

void ConnectionManager::handleData()
{
	osc = receiver->fetchData();
	QVector<QPointF> points;


	resetChart();
	if (context->channelSelected) {
		series->replace(points);
	}
}

void ConnectionManager::toggleConnection()
{
	if (context->connectionActive) {
		context->fpsTimer->start();
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

void ConnectionManager::dataTimer()
{
	static int frameCount = 0;
	static QString labelText = QStringLiteral("FPS: %1");
	auto* fpsTimer = context->fpsTimer;
	auto* fpsLabel = context->ui_MainWindow->fpsLabel;

	for (int i = 0; i < 256; ++i) {
		qreal y = points[i].y() + qreal(1);
		if (y >= 256) y = 0;
		points[i].setY(y);
	}

	//resetChart();
	if (context->channelSelected) {
		series->replace(points);
	}

	frameCount++;
	int elapsed = fpsTimer->elapsed();
	if (elapsed >= 1000) {
		elapsed = fpsTimer->restart();
		qreal fps = qreal(0.1 * int(10000.0 * (qreal(frameCount) / qreal(elapsed))));
		fpsLabel->setText(labelText.arg(QString::number(fps, 'f', 1)));
		fpsLabel->adjustSize();
		frameCount = 0;
	}
}