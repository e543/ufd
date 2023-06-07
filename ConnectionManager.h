#pragma once

#include "Client.h"
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
    QElapsedTimer* fpsTimer;
    bool connectionActive;
    bool channelSelected;
    bool channelChanged;
    quint8* osc;
    ClickedLabel* currentLabel;
    quint8 selectedChannel;
    QVector<QXYSeries*> channelSeries;
};

class ConnectionManager : public QObject
{
    Q_OBJECT
    QVector<quint8> osc;
    int i = 0;
    Client* client = nullptr;
    Context* context;
    qreal delta;
    qreal x;
    qreal width;
    QXYSeries* series;
    QVector<QPointF> points;
    void resetChart();
private slots:
    void handleData();
    void toggleConnection();
    void dataTimer();
    void strobeChanged();
public:
    ConnectionManager(Context* context);
};

