#pragma once
#include "ConnectionManager.h"
#include "ColorSchemeDialog.h"
#include "UnitSettingsDialog.h"
#include "ui_MainWindow.h"
#include "ChartWidget.h"

struct Context
{
    Ui::MainWindow* ui_MainWindow;
    Ui::UnitSettings* ui_UnitSettings;
    Ui::ColorScheme* ui_ColorScheme;
    ChartWidget* firstWidget;
    bool isAskanVisible;
    bool isRazvVisible;
    QTimer* timer;
};

class ApplicationController
{
    Context* context;
	ConnectionManager* connectManager;
//private slots:
    void updateSecondSeries();
public:
    ApplicationController(Context* context) : context(context) 
    {
        context->timer = new QTimer();
        connectManager = new ConnectionManager(context->timer);
    }
    void connectDataSeries();
    void startUpdateSecondSeries();
};

