#pragma once

#include <QtCharts>
#include <QtWidgets>
#include <QtCore>

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QTimer* timer = 0);
    
    void startUpdates(QXYSeries* series);
    void stopUpdates(QXYSeries* series);

public slots:
    void generateData(int rowCount, int colCount);
    void update();

private:
    quint32 rowCount;
    QVector <QVector<QPointF>> m_data;
    QXYSeries* m_series;
    QTimer* m_dataUpdater;
};

