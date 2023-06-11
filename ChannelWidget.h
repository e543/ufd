#pragma once
#include "InteractiveStrobe.h"

class ChannelWidget : public QChartView
{
    Q_OBJECT
protected:
    QChart* chart;
    QVector<QXYSeries*> seriesVector;
    QVector<QColor> colors = {
            QColor(Qt::red) ,
            QColor(Qt::yellow) ,
            QColor(Qt::blue) ,
            QColor(Qt::cyan) ,
            QColor(Qt::magenta) };
    QLineSeries *upperSeries, *lowerSeries;
public slots:
    void strobesChanged();
public:
    ChannelWidget(QWidget* parent = 0, QChart* chart = 0);
    ~ChannelWidget();
    QChart* getChart();
    QVector<QXYSeries*> getSeriesVector();
    QVector<InteractiveStrobe*> getStrobes();
    void setStrobes(QVector<InteractiveStrobe*>);
    void resetChart();
};
