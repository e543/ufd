#pragma once

#include "Client.h"
#include "Ruler.h"
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
    bool strobesAdded;
    quint8* osc;
    ClickedLabel* currentLabel;
    quint8 selectedChannel;
    QHash<QString, QChartView*> chartViews;
    QVector<ChannelWidget*> channels;
    QVector<QVector<QXYSeries*>> channelSeries;
    QVector<QPair<QPointF, qreal>> posWidthes;
    Ruler* ruler;
    QThreadPool* threadPool;
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
    QVector<QVector<QPair<qreal, QPointF>>> limits;
    QThreadPool* threadPool;
    bool oscObtained = true;
    bool amplObtained = true;
    bool strobesInitialized = false;
    void resetChart();
private slots:
    void handleData();
    void toggleConnection();
    void dataTimer();
    void strobeChanged();
    void sendStrobe();
public:
    ConnectionManager(Context* context);
    void strobeInit();
};

