#pragma once

#include "Receiver.h"
#include "Server.h"
#include "ColorSchemeDialog.h"
#include "UnitSettingsDialog.h"
#include "ChartWidget.h"
#include <ui_MainWindow.h>

struct Context
{
    Ui::MainWindow* ui_MainWindow;
    Ui::UnitSettings* ui_UnitSettings;
    Ui::ColorScheme* ui_ColorScheme;
    ChartWidget* firstWidget;
    bool isAskanVisible;
    bool isRazvVisible;
    QTimer* timer;
    bool connectionActive;
};


class ConnectionManager : public QObject
{
    Q_OBJECT
	quint8 osc[256];
    amp_struct_t data;
    Receiver* receiver = nullptr;
    Context* context;
    qreal delta;
    qreal x;
    qreal width;
private slots:
    void handleData();
    void toggleConnection();
public:
    ConnectionManager(Context* context);
};

