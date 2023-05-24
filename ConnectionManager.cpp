#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(QTimer* timer) : timer(timer)
{
	dataSource = new DataSource(timer);
}

void ConnectionManager::setConnection(QChart* chart, QXYSeries* series)
{
	dataSource->generateData(20, 200);

	dataSource->startUpdates(series);
}
