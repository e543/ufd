#include "Channel.h"

Channel::Channel(QWidget* parent, QChart* chart) : QChartView(chart, parent), chart(chart)
{
	setDragMode(QGraphicsView::NoDrag);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	if (!chart) {
		chart = new QChart();
		this->chart = chart;
		setChart(chart);
	}

	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	this->setSizePolicy(sizePolicy);
	this->setMaximumSize(QSize(3000, 3000));
	chart->setMinimumHeight(0);

	chart->layout()->setContentsMargins(0, 0, 0, 0);
	chart->setBackgroundRoundness(0);
	chart->setMargins(QMargins{ 0 , 0 ,0 , 0 });
};


Channel::~Channel()
{
}

QChart* Channel::getChart()
{
	return chart;
}