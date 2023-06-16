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
		delta = width / 256;
	}

	threadPool = context->threadPool;
	oscObtained = true;
	amplObtained = true;
	strobesInitialized = false;
	//limits.resize(8);

	QObject::connect(context->timer, &QTimer::timeout, this, &ConnectionManager::dataTimer);
	QObject::connect(context->ui_MainWindow->StopButton, SIGNAL(clicked()), this, SLOT(toggleConnection()));
	QObject::connect(context->firstWidget, &ChartWidget::strobesChanged, this, &ConnectionManager::strobeChanged);
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
		ChannelWidget* channel = context->channels[8];

		auto* chart = channel->getChart();
		//auto channelSeries = context->channelSeries[num];
		auto channelSeries = context->channelSeries[8];
		auto strb = result.data.ampl_tact[num / 2].ampl_us[num % 2].ampl;
		if (cx > width) {
			for (auto* channel : context->channels) {
				channel->resetChart();
			}
			cx = 0;
		}

		/*context->channels[num]->appendPoint(cx, strb);
		context->channels[8]->appendPoint(cx,strb);*/

		auto& channels = context->channels;
		channels[8]->appendPoint(cx, strb);
		for (int i = 0; i < 8; ++i) {
			auto strb = result.data.ampl_tact[i / 2].ampl_us[i % 2].ampl;
			channels[i]->appendPoint(cx, strb);
		}
	}
	cx += delta;
	context->ruler->run();
}

void ConnectionManager::toggleConnection()
{
	auto* timer = context->timer;
	if (context->connectionActive) {
		timer->start(5);
		context->fpsTimer->start();
		context->ruler->start();
		client->setConnection();
		strobeChanged();
		QObject::connect(client, SIGNAL(dataReceived()), this, SLOT(handleData()));
		QObject::connect(context->firstWidget, &ChartWidget::strobesChanged, this, &ConnectionManager::sendStrobe);
		oscObtained = true;
		amplObtained = true;
		resetChart();
	}
	else
	{
		timer->stop();
		context->ruler->stop();
		QObject::disconnect(client, SIGNAL(dataReceived()), this, SLOT(handleData()));
		QObject::disconnect(context->firstWidget, &ChartWidget::strobesChanged, this, &ConnectionManager::sendStrobe);
		client->disconnect();
	}
}

void ConnectionManager::dataTimer()
{
	if (context->strobesAdded) {
		strobeInit();
		context->strobesAdded = false; 
		strobesInitialized = true;
	}

	if (context->channelSelected) {
		if (context->channelChanged) {
			strobeChanged();
		}

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

	auto strobes = context->firstWidget->getStrobes();
	QVector<QPair<QPointF, qreal>> posWidthes;
	int numChannel = context->selectedChannel;

	if (context->channelChanged) {
		posWidthes = context->channels[numChannel]->getPosWidthes();
		context->firstWidget->setPosWidth(posWidthes);
		context->channelChanged = false;

		//context->channels[numChannel]->resetChart();
		/*for (auto* channel : context->channels) {
			channel->resetChart();
		}
		cx = 0;*/
		client->sendNumChannel(numChannel);
	}
	else {
		for (auto* strobe : strobes)
		{
			auto posWidth = strobe->getPosWidth();
			posWidthes.append(posWidth);
		}
	}

	if (posWidthes.size()) {
		context->posWidthes = posWidthes;
		context->channels[context->selectedChannel]->setPosWidth(posWidthes);
		context->channels[8]->setPosWidth(posWidthes);
	}

	if (strobesInitialized)
	{
		limits[numChannel].clear();
		for (auto* strobe : strobes) {
			qreal y = strobe->getLPoint().y();
			QPointF point = { strobe->getLPoint().x(), strobe->getRPoint().x() };
			limits[numChannel] << QPair<qreal, QPointF> { y, point };
		}
	}
}

void ConnectionManager::strobeInit() {
	limits.clear();
	limits.resize(8);

	auto strobes = context->firstWidget->getStrobes();

	for (int j = 0; j < 8; ++j){
		for (int i = 0; i < 5; ++i) {
			auto& strobe = strobes[i];
			qreal y = strobe->getLPoint().y();
			QPointF point = { strobe->getLPoint().x(), strobe->getRPoint().x() };
			limits[j] << QPair<qreal, QPointF> { y, point };
		}
	}
	sendStrobe();
}

void ConnectionManager::sendStrobe()
{
	if (limits.size())
		client->sendStrobes(width, limits);
}
