#include "ChartWidget.h"

ChartWidget::ChartWidget(QWidget* parent, QChart* chart) : QChartView(chart, parent) , chart(chart)
{
	setDragMode(QGraphicsView::NoDrag);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	if (!chart) {
		chart = new QChart();
		this->chart = chart;
		setChart(chart); 
	}

	chart->layout()->setContentsMargins(0, 0, 0, 0);
	chart->setBackgroundRoundness(0);
	chart->setMargins(QMargins{ 0 , 0 ,0 , 0 });

	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(200, 300));
	setMaximumSize(QSize(3000, 3000));
	setRenderHint(QPainter::Antialiasing);

	QValueAxis* axisX = new QValueAxis;
	QValueAxis* axisY = new QValueAxis;

	chart->addAxis(axisX, Qt::AlignBottom);
	chart->addAxis(axisY, Qt::AlignLeft);
	chart->legend()->hide();

	axisX->setRange(0, 80.6);
	axisY->setRange(0, 255);

	series = new QLineSeries;
	chart->addSeries(series);
	series->attachAxis(axisX);
	series->attachAxis(axisY);
	QPen pen;
	pen.setColor("green");
	pen.setWidth(3);
	series->setPen(pen);
	//series->setUseOpenGL(true);
};


ChartWidget::~ChartWidget()
{

}

void ChartWidget::addStrobe(QPointF initPos)
{
	auto* strobe = new InteractiveStrobe(chart, initPos);
	strobes.append(strobe);
}

QChart* ChartWidget::getChart()
{
	return chart;
}

QXYSeries* ChartWidget::getSeries()
{
	return series;
}

void ChartWidget::mousePressEvent(QMouseEvent* event)
{
	QChartView::mousePressEvent(event);
}

void ChartWidget::mouseMoveEvent(QMouseEvent* event)
{
	QChartView::mouseMoveEvent(event);
}

void ChartWidget::resizeEvent(QResizeEvent* event)
{
	if (scene()) {
		scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
		chart->resize(event->size());
		for (auto* strobe : strobes)
			strobe->updateGeometry();
	}
	QChartView::resizeEvent(event);
}