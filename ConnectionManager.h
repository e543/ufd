#pragma once

#include "Client.h"
#include "ColorSchemeDialog.h"
#include "UnitSettingsDialog.h"
#include "ChartWidget.h"
#include "SecondaryWidget.h"
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
    QHash<QString, QChartView*> chartViews;
    QVector<ChannelWidget*> channels;
    QVector<QVector<QXYSeries*>> channelSeries;
};

class ConnectionManager : public QObject
{
    Q_OBJECT
    QVector<quint8> osc;
    Client* client = nullptr;
    Context* context;
    qreal delta;
    qreal x;
    qreal cx;
    qreal width;
    QXYSeries* series;
    QVector<QPointF> points;
    bool oscObtained = true;
    bool amplObtained = true;
    void resetChart();
private slots:
    void handleData();
    void toggleConnection();
    void dataTimer();
    void strobeChanged();
public:
    ConnectionManager(Context* context);
};

