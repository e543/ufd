#pragma once

#include "Receiver.h"
#include "Server.h"
#include "ColorSchemeDialog.h"
#include "UnitSettingsDialog.h"
#include "ChartWidget.h"
#include <ui_MainWindow.h>

#define NUM_RESULT_STRBS    5
#define NUM_USS             2
#define NUM_TACTS            4

struct amp_strob_struct_t
{
    quint16 time;
    quint8 ampl;
};

struct amp_us_struct_t
{
    amp_strob_struct_t ampl[NUM_RESULT_STRBS];
};

struct amp_tact_struct_t
{
    amp_us_struct_t ampl_us[NUM_USS];
};

struct amp_struct_t
{
    amp_tact_struct_t ampl_tact[NUM_TACTS];
};

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
private slots:
    void toggleConnection();
public:
    ConnectionManager(Context* context);
};

