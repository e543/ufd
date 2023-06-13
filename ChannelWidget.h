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
    InteractiveStrobe* selectedStrobe;
    QVector<QPair<QPointF,qreal>> posWidthes;
    QVector<QXYSeries*> seriesVector;
    QVector<QGraphicsLineItem*> strobeLines;
    QVector<QColor> colors = {
            QColor(Qt::red) ,
            QColor(Qt::green) ,
            QColor(Qt::blue) ,
            QColor(Qt::cyan) ,
            QColor(Qt::magenta) };
    QVector<QLineSeries*> upperVector;
    QVector<QLineSeries*> helpVector;
    QVector<QAreaSeries*> areaVector;
    qreal width, delta, left, right;
    void resizeEvent(QResizeEvent* event) override;
public:
    ChannelWidget(QWidget* parent = 0, QChart* chart = 0);
    ~ChannelWidget();
    QChart* getChart();
    QVector<QXYSeries*> getSeriesVector();
    void setPosWidth(QVector<QPair<QPointF, qreal>> strobes);
    void resetChart();
    void appendPoint(quint8 x, amp_strob_struct_t* strobes);
    QVector<QPair<QPointF, qreal>> getPosWidthes();
};
