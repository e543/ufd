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
        QColor color = colors[i];

        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);

        QPen pen;
        color.setAlphaF(0.0);
        pen.setColor(color);
        pen.setWidth(1);
        series->setPen(pen);
        seriesVector.append(series);

        /*QLineSeries* upperSeries = new QLineSeries;

        color.setAlphaF(0.0);
        pen.setColor(color);
        upperVector.append(upperSeries);
        chart->addSeries(upperSeries);
        upperVector[i]->attachAxis(axisX);
        upperVector[i]->attachAxis(axisY);*/

        QLineSeries* helpSeries = new QLineSeries;

        chart->addSeries(helpSeries);
        helpSeries->attachAxis(axisX);
        helpSeries->attachAxis(axisY);


        QGraphicsLineItem* line = new QGraphicsLineItem;
        pen.setStyle(Qt::DashLine);
        color.setAlphaF(1.0);
        pen.setColor(color);
        pen.setWidth(2);
        line->setPen(pen);
        chart->scene()->addItem(line);
        strobeLines.append(line);

        color.setAlphaF(0.);
        pen.setColor(color);
        pen.setStyle(Qt::SolidLine);
        helpSeries->setPen(pen);
        helpVector.append(helpSeries);

        QAreaSeries* areaSeries = new QAreaSeries(series, helpSeries);

        areaVector.append(areaSeries);
        color.setAlphaF(0.5);
        areaSeries->setBrush(color);
        chart->addSeries(areaSeries);
        areaSeries->attachAxis(axisX);
        areaSeries->attachAxis(axisY);
        axisX->setRange(0, 255);
        axisY->setRange(0, 255);
    }

    width = qAbs(axisX->max() - axisX->min());
    delta = width / 256;

    resetChart();
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
    return seriesVector;
}

void ChannelWidget::setPosWidth(QVector<QPair<QPointF, qreal>> posWidthes)
{
    this->posWidthes = posWidthes;
    for (int i = 0; i < strobeLines.size() && i < posWidthes.size(); ++i) {
        qreal y = posWidthes[i].first.y();
        QPointF left = chart->mapToPosition(QPointF{ 0,  y });
        QPointF right = chart->mapToPosition(QPointF{ 255,  y });
        chart->mapToPosition(left);
        strobeLines[i]->setLine(left.x(), left.y(), right.x(), right.y());
    }
}

QVector<QPair<QPointF, qreal>> ChannelWidget::getPosWidthes()
{
    return posWidthes;
}

void ChannelWidget::resetChart()
{
    for (int i = 0; i < 5; ++i) {
        seriesVector[i]->clear();
        helpVector[i]->clear();
    }
}

void ChannelWidget::appendPoint(quint8 x, amp_strob_struct_t* strob)
{
    for (int i = 0; i < 5; ++i) {
        auto* series = seriesVector[i];
        QPointF point{ qreal(x),  qreal(strob[i].ampl) };
        qreal y = strobeLines[i]->line().p1().y();
        QPointF left = chart->mapToValue(QPointF{ 0,  y });
        QPointF right = left;
        right.setX(x);

        series->append(point);
        helpVector[i]->clear();
        *helpVector[i] << left << ri
    }
}

void ChannelWidget::resizeEvent(QResizeEvent* event)
{
    QChartView::resizeEvent(event);
    setPosWidth(posWidthes);
}