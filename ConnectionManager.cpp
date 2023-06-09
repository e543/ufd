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

	if (result.command == "o") {
		static int frameCount = 0;
		static QString labelText = QStringLiteral("FPS: %1");

		auto* fpsTimer = context->fpsTimer;
		auto* fpsLabel = context->ui_MainWindow->fpsLabel;

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

		frameCount++;
		int elapsed = fpsTimer->elapsed();
		if (elapsed >= 1000) {
			elapsed = fpsTimer->restart();
			qreal fps = qreal(0.1 * int(10000.0 * (qreal(frameCount) / qreal(elapsed))));
			fpsLabel->setText(labelText.arg(QString::number(fps, 'f', 1)));
			fpsLabel->adjustSize();
			frameCount = 0;
		}
		return;
	}

	if (result.command == "a") {

		auto num = context->selectedChannel;
		auto& views = context->channelViews;
		QChartView* channel;
		switch (num) {
		default:
		case 0:
			channel = views["11"];
			break;
		case 1:
			channel = views["12"];
			break;
		case 2:
			channel = views["21"];
			break;
		case 3:
			channel = views["22"];
			break;
		case 4:
			channel = views["31"];
			break; 
		case 5:
			channel = views["32"];
			break;
		case 6:
			channel = views["41"];
			break;
		case 7:
			channel = views["42"];
			break;
		}

		auto* chart = channel->chart();
		auto seriesList = chart->series();
		auto strb = result.data.ampl_tact[num / 2].ampl_us[num % 2].ampl;
		if (cx > width) {
			for (auto* series : seriesList) {
				auto* xyseries = qobject_cast<QXYSeries*>(series);
				xyseries->clear();
			}
			cx = 0;
		}


		for (auto* series : seriesList) {
			auto* xyseries =  qobject_cast<QXYSeries*>(series);
			
			*xyseries << QPointF{ cx,  qreal(strb[num].ampl) };
		}
		cx += delta;
		return;
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