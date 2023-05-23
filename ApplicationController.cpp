#include "ApplicationController.h"

void ApplicationController::RenderContext()
{
	auto* chart= context->firstWidget->getChart();
	auto* series = context->firstWidget->getSeries();
	connectManager->setConnection(chart, series);
}
