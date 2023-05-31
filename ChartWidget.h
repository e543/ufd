#pragma once

#include <QtCharts>
#include "InteractiveStrobe.h"

class ChartWidget : public QChartView
{
    Q_OBJECT
    QVector<InteractiveStrobe*> strobes;
    InteractiveStrobe* selectedStrobe;
    QChart* chart;
    QLineSeries* series;
public:
    ChartWidget(QWidget* parent = 0, QChart* chart = 0);
    ~ChartWidget();
    QChart* getChart();
    QXYSeries* getSeries(); 
    void setSeries(QLineSeries* series);
    void addStrobe(QPointF initPos);
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
};

