#pragma once
#include <QtWidgets>
#include <QtCharts>
#include <QChartView>
#include "ApplicationController.h"

struct Settings
{
    QSize windowSize;
    QSize availableSize;
    QSize primarySize;
    QRect graphicsRect;
    QRect secondaryRect;

    const float percentage = 0.7;
    const unsigned int channelsCount = 10;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class Application;

    Settings* settings;
    Context* context;

    ColorSchemeDialog* colorScheme = nullptr;
    UnitSettingsDialog* unitSettings = nullptr;

    void initMainWindow();
    void initColorSchemeDialog();
    void initUnitSettingsDialog();
    void initGraphicsView();
    void initSecondaryView();
    void initMultiChannelScanning();
    void updateSettings();
    void bindContext();
    Context* getContext();
    Settings* getSettings();

    void resizeEvent(QResizeEvent* event) override;

    Ui::MainWindow ui_MainWindow;
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};