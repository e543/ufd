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
    bool channelSelected;
    bool channelChanged;
    amp_struct_t data;
    ClickedLabel* currentLabel;
    quint8 selectedChannel;
    QVector<QXYSeries*> channelSeries;
};

class ConnectionManager : public QObject
{
    Q_OBJECT
	quint8 osc[256];
    Receiver* receiver = nullptr;
    Context* context;
    qreal delta;
    qreal x;
    qreal width;
    void resetChart();
private slots:
    void handleData();
    void toggleConnection();
public:
    ConnectionManager(Context* context);
};

