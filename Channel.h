#pragma once
#include <QtCharts>

class Channel : public QChartView
{
    Q_OBJECT
    QChart* chart;
public:
    Channel(QWidget* parent = 0, QChart* chart = 0);
    ~Channel();
    QChart* getChart();
};
