#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(Context* context) : context(context), client(new Client())
{
	series = context->firstWidget->getSeries();

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
	QObject::connect(context->timer, &QTimer::timeout, this, &ConnectionManager::dataTimer);
	QObject::connect(context->ui_MainWindow->StopButton, SIGNAL(clicked()), this, SLOT(toggleConnection()));
}

void ConnectionManager::resetChart()
{
	x = 0;
	context->firstWidget->getSeries()->clear();
}

void ConnectionManager::handleData()
{
	static int frameCount = 0;
	static QString labelText = QStringLiteral("FPS: %1");


	auto* fpsTimer = context->fpsTimer;
	auto* fpsLabel = context->ui_MainWindow->fpsLabel;
	Result result = client->fetchData();

	if (result.command == "o") {
		osc = client->fetchData().osc;
		QVector<QPointF> points;
		points.resize(256);

		for (int i = 0; i < 256 && x < width; ++i, x += delta) {
			qreal y = osc[i];
			points[i] = QPointF{ x, y };
		}

		resetChart();
		if (context->channelSelected) {
			series->replace(points);
		}
	}
	else 
	if (result.command == "a") {
		auto num = context->selectedChannel;
		auto& channel = result.data.ampl_tact[num / 2].ampl_us[num % 2].ampl;
		for (int i = 0; i < 5; ++i) {
			channel[i];
		}
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

void ConnectionManager::toggleConnection()
{
	auto* timer = context->timer;
	if (context->connectionActive) {
		timer->start(1);
		context->fpsTimer->start();
		client->setConnection();
		strobeChanged();
		QObject::connect(client, SIGNAL(dataReceived()), this, SLOT(handleData()));
		QObject::connect(context->firstWidget, &ChartWidget::strobesChanged, this, &ConnectionManager::strobeChanged);
		resetChart();
	}
	else
	{
		timer->stop();
		QObject::disconnect(client, SIGNAL(dataReceived()), this, SLOT(handleData()));
		QObject::disconnect(context->firstWidget, &ChartWidget::strobesChanged, this, &ConnectionManager::strobeChanged);
		client->disconnect();
	}
}

void ConnectionManager::dataTimer()
{
	if (context->channelSelected) {
		client->sendCommand("o");
		client->sendCommand("a");
	}
}

void ConnectionManager::strobeChanged()
{
	//qDebug() << "Strobe changed!";
	QVector<QPair<qreal, QPointF>> pairs;
	auto strobes = context->firstWidget->getStrobes();
	for (auto* strobe : strobes) {
		QPointF point = { strobe->getLPoint().x(), strobe->getRPoint().x() };
		pairs << QPair<qreal, QPointF> { strobe->getLPoint().y(), point };
	}

	if (context->channelSelected)
	{
		client->sendStrobes(context->selectedChannel, width, pairs);
	}
	else {
		client->sendStrobes(0, width, pairs);
	}
}