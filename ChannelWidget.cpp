#include "ChannelWidget.h"

ChannelWidget::ChannelWidget(QWidget* parent, QChart* chart) : QChartView(chart, parent), chart(chart)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);

    if (!chart) {
        chart = new QChart();
        this->chart = chart;
        setChart(chart);
    }

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);
    setMaximumSize(QSize(3000, 3000));
    chart->setMinimumHeight(0);

    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);
    chart->setMargins(QMargins{ 0 , 0 ,0 , 0 });


    QValueAxis* axisX = new QValueAxis;
    QValueAxis* axisY = new QValueAxis;
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->hide();

    axisX->setRange(0, 255);
    axisY->setRange(0, 255);
    axisX->hide();
    axisY->hide();

    for (int i = 0; i < 5; ++i) {
        QLineSeries* series = new QLineSeries;

        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);

        QPen pen;
        pen.setColor(colors[i]);
        pen.setWidth(3);
        series->setPen(pen);
        seriesVector.append(series);
    }

    upperSeries = new QLineSeries;
    lowerSeries = new QLineSeries;

    QPen pen;
    QColor color = QColor("blue");
    color.setAlphaF(1.);
    pen.setColor(color);
    pen.setWidth(1);

    upperSeries->setPen(pen);
    lowerSeries->setPen(pen);
    chart->addSeries(upperSeries);
    chart->addSeries(lowerSeries);
    upperSeries->attachAxis(axisX);
    lowerSeries->attachAxis(axisY);
    upperSeries->attachAxis(axisX);
    lowerSeries->attachAxis(axisY);
};


ChannelWidget::~ChannelWidget()
{
}

QChart* ChannelWidget::getChart()
{
    return chart;
}

QVector<QXYSeries*> ChannelWidget::getSeriesVector()
{
    return  seriesVector;
}

void ChannelWidget::strobesChanged()
{
}

QVector<InteractiveStrobe*> ChannelWidget::getStrobes()
{
    return QVector<InteractiveStrobe*>();
}

void ChannelWidget::setStrobes(QVector<InteractiveStrobe*>)
{
}

void ChannelWidget::resetChart()
{
    for (auto* series : seriesVector) {
        series->clear();
    }
    upperSeries->clear();
    lowerSeries->clear();
}
