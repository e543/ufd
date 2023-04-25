#pragma once
#include <QtWidgets>
#include <QtWidgets/QMainWindow>
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

    ColorSchemeWidget* colorScheme;
    UnitSettingsWidget* unitSettings;

    QWidget* centralWidget = nullptr;
    QWidget* MultiChannelScanningWidget = nullptr;
    QMap<QString, QChartView*> channels;

    QChartView* graphicsView = nullptr;
    QChartView* secondaryView = nullptr;
    QVBoxLayout* MultiChannelVerticalLayout = nullptr;
    QHBoxLayout* MultiChannelHorizontalLayout = nullptr;

    

    void initMainWindow();
    void initColorSchemeWidget();
    void initUnitSettingsWidget();
    void initGraphicsView();
    void initSecondaryView();
    void initMultiChannelScanning();
    void updateSettings();
    void bindContext();
    Context* getContext();
    Settings* getSettings();

    void resizeEvent(QResizeEvent* event) override;
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};