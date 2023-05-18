#pragma once

#include <QtCharts>
#include "InteractiveStrobe.h"

class ChartWidget : public QChartView
{
    Q_OBJECT
    QVector<InteractiveStrobe*> strobes;
    InteractiveStrobe* selectedStrobe;
    QChart* chart;
public:
    ChartWidget(QWidget* parent = 0, QChart* chart = 0);
    ~ChartWidget();
    QChart* getChart();
    void addStrobe(QPointF initPos);
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
};

