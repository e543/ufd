#include "ApplicationController.h"

void ApplicationController::RenderContext()
{
    QLineSeries* series = new QLineSeries();
    series->append(0, 1);
    series->append(1, 4);
    series->append(2, 2);

    context->graphicsView->chart()->addSeries(series);

    QLineSeries* secondSeries = new QLineSeries();
    secondSeries->append(0, 1);
    secondSeries->append(1, 1);
    secondSeries->append(2, 2);

    context->secondaryView->chart()->addSeries(secondSeries);
}
