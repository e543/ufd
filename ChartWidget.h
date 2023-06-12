#pragma once

#include <QtCharts>
#include "InteractiveStrobe.h"

class ChartWidget : public QChartView
{
    Q_OBJECT
    QVector<InteractiveStrobe*> strobes;
    InteractiveStrobe* selectedStrobe;
    QChart* chart;
    QXYSeries* series;
    bool strobeIsChanged = false;
signals:
    void strobesChanged();
public:
    ChartWidget(QWidget* parent = 0, QChart* chart = 0);
    ~ChartWidget();
    QChart* getChart();
    QXYSeries* getSeries();
    QVector<InteractiveStrobe*> getStrobes();
    void setPosWidth(QVector<QPair<QPointF, qreal>> posWidthes);
    void setSeries(QXYSeries* series);
    void addStrobe(QPointF initPos);
    void addStrobe(QColor color, QPointF initPos);
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void emitStrobesChanged();
};

