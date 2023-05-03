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

    UnitSettingsDialog* unitSettings = nullptr;

    void initMainWindow();
    void initUnitSettingsDialog();
    void initGraphicsView();
    void initSecondaryView();
    void updateSettings();
    void bindContext();
    Context* getContext();
    Settings* getSettings();

    void resizeEvent(QResizeEvent* event) override;

    Ui::MainWindow* ui;
private slots:
    void on_SettingsClicked();

protected:
    void closeEvent(QCloseEvent* event);
    bool eventFilter(QObject* obj, QEvent* e);
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};