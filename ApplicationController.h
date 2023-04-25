#pragma once
#include <QtWidgets>
#include <QtCharts>
#include <QChartView>

#include "ConnectionManager.h"
#include "ColorSchemeWidget.h"
#include "UnitSettingsWidget.h"

struct Context
{
    QWidget* centralWidget = nullptr;
    ColorSchemeWidget* coloreScheme = nullptr;
    UnitSettingsWidget* unitSettings = nullptr;
    QWidget* MultiChannelScanningWidget = nullptr;
    QMap<QString, QChartView*> channels;

    QChartView* graphicsView = nullptr;
    QChartView* secondaryView = nullptr;
    QVBoxLayout* MultiChannelVerticalLayout = nullptr;
    QHBoxLayout* MultiChannelHorizontalLayout = nullptr;
};

class ApplicationController
{
    Context* context;
	ConnectionManager connectManager;
public:
    ApplicationController(Context* context) : context(context) 
    {
        RenderContext();
    }
    void RenderContext();
};

