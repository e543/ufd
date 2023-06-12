#pragma once
#include "InteractiveStrobe.h"
#include "Server.h"

QT_BEGIN_NAMESPACE
struct amp_strob_struct_t;
struct amp_us_struct_t;
struct amp_tact_struct_t;
struct amp_struct_t;
QT_END_NAMESPACE

class ChannelWidget : public QChartView
{
    Q_OBJECT
protected:
    QChart* chart;
    QVector<qreal> strobes;
    InteractiveStrobe* selectedStrobe;
    QVector<QXYSeries*> seriesVector;
    QVector<QGraphicsLineItem*> strobeLines;
    QVector<QColor> colors = {
            QColor(Qt::red) ,
            QColor(Qt::yellow) ,
            QColor(Qt::blue) ,
            QColor(Qt::cyan) ,
            QColor(Qt::magenta) };
    QVector<QLineSeries*> upperVector;
    qreal width, delta, left, right;
public:
    ChannelWidget(QWidget* parent = 0, QChart* chart = 0);
    ~ChannelWidget();
    QChart* getChart();
    QVector<QXYSeries*> getSeriesVector();
    void updateLines(QVector<qreal> strobes);
    void resetChart();
    void appendPoint(quint8 x, amp_strob_struct_t* strobes);
};
