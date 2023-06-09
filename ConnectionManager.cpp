#include "ConnectionManager.h"
#include <QDebug>

ConnectionManager::ConnectionManager(Context* context) : context(context), client(new Client())
{
	series = context->firstWidget->getSeries();
	cx = 0;
	auto* chart = context->firstWidget->getChart();
	if (chart)
	{
		auto* axisX = qobject_cast<QValueAxis*>(chart->axisX());
		width = qAbs(axisX->max() - axisX->min());
		qreal toothCount = 5;
		delta = width / 256;
	}

	oscObtained = true;
	amplObtained = true;
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
	Result result = client->fetchData();

	if (result.command == "s") {
		return;
	}

	if (result.command == "o") {
		oscObtained = true;
		static int frameCount = 0;
		static QString labelText = QStringLiteral("FPS: %1");

		auto* fpsTimer = context->fpsTimer;
		auto* fpsLabel = context->ui_MainWindow->fpsLabel;

		osc = result.osc;
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

	if (result.command == "a") {
		amplObtained = true;
		auto num = context->selectedChannel;
		//Channel* channel = context->channels[num];
		Channel* channel = context->channels[8];


		auto* chart = channel->getChart();
		//auto channelSeries = context->channelSeries[num];
		auto channelSeries = context->channelSeries[8];
		auto strb = result.data.ampl_tact[num / 2].ampl_us[num % 2].ampl;
		if (cx > width) {
			for (int i = 0; i < 5; ++i) {
				channelSeries[i]->clear();
			}
			cx = 0;
		}

		for (int i = 0; i < 5; ++i) {
			//if (strb[i].ampl)
			*channelSeries[i] << QPointF{ cx,  qreal(strb[i].ampl) };
		}
	}
	cx += delta;
}

void ConnectionManager::toggleConnection()
{
	auto* timer = context->timer;
	if (context->connectionActive) {
		timer->start(5);
		context->fpsTimer->start();
		client->setConnection();
		strobeChanged();
		QObject::connect(client, SIGNAL(dataReceived()), this, SLOT(handleData()));
		QObject::connect(context->firstWidget, &ChartWidget::strobesChanged, this, &ConnectionManager::strobeChanged);
		oscObtained = true;
		amplObtained = true;
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
		if (oscObtained) {
			oscObtained = false;
			client->sendCommand("o");
		}
		if (amplObtained) {
			amplObtained = false;
			client->sendCommand("a");
		}
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