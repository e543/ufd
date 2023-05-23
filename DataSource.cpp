#include "DataSource.h"

DataSource::DataSource(QTimer* timer) :
    m_dataUpdater(timer)
{
    if (!timer)
        m_dataUpdater = new QTimer();
    generateData(0, 0);
}

void DataSource::update()
{
    if (m_series) {
        quint64 i = QRandomGenerator::global()->generate64() % rowCount;
        m_series->replace(m_data.at(i));
    }
}

void DataSource::handleSceneChanged()
{
    m_dataUpdater->start();
}

void DataSource::startUpdates(QXYSeries* series)
{
    series->clear();
    m_series = series;

    m_dataUpdater->setInterval(20);
    m_dataUpdater->setSingleShot(true);
    QObject::connect(m_dataUpdater, &QTimer::timeout,
        this, &DataSource::update);

    update();
}

void DataSource::stopUpdates(QXYSeries* series = 0)
{
    if (!series)
        series = m_series;

    series->clear();

    QObject::disconnect(m_dataUpdater, &QTimer::timeout,
        this, &DataSource::update);
}

void DataSource::generateData(int rowCount, int colCount)
{
  m_data.clear();
  this->rowCount = rowCount;

  for (int i(0); i < rowCount; i++) {
      QVector<QPointF> points;
      points.reserve(colCount);
      for (int j(0); j < colCount; j++) {
          qreal x(0);
          qreal y(0);

          y = 6.0 * (j % 40) + 10* QRandomGenerator::global()->generateDouble();
          x = j * (qreal(80.6) / colCount);

          points.append(QPointF(x, y));
      }
      m_data.append(points);
  }  
}