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
        pen.setWidth(2);
        series->setPen(pen);
        seriesVector.append(series);

        QLineSeries* upperSeries = new QLineSeries;

        chart->addSeries(upperSeries);
        upperSeries->attachAxis(axisX);
        upperSeries->attachAxis(axisY);

        QColor color = colors[i];
        color.setAlphaF(0.5);
        pen.setColor(colors[i]);
        pen.setWidth(2);
        upperSeries->setPen(pen);
        upperVector.append(upperSeries);

        QGraphicsLineItem* line = new QGraphicsLineItem;
        pen.setStyle(Qt::DashLine);
        color.setAlphaF(1.0);
        pen.setColor(color);
        line->setPen(pen);
        chart->scene()->addItem(line);
        strobeLines.append(line);
    }

    width = qAbs(axisX->max() - axisX->min());
    delta = width / 256;
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

void ChannelWidget::updateLines(QVector<qreal> strobes)
{
    this->strobes = strobes;
    for (int i = 0; i < strobeLines.size(); ++i) {
        QPointF left = chart->mapToPosition(QPointF{ 0,  strobes[i] });
        QPointF right = chart->mapToPosition(QPointF{ 255,  strobes[i] });
        chart->mapToPosition(left);
        strobeLines[i]->setLine(left.x(),left.y(), right.x(),right.y());
    }
}

void ChannelWidget::resetChart()
{
    for (int i = 0; i < 5; ++i) {
        seriesVector[i]->clear();
        upperVector[i]->clear();
    }
}

void ChannelWidget::appendPoint(quint8 x, amp_strob_struct_t* strob)
{
    for (int i = 0; i < 5; ++i) {
        QPointF point{ qreal(x),  qreal(strob[i].ampl) };
        seriesVector[i]->append(point);
    }
}
