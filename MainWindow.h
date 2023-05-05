#pragma once
#include <QtWidgets>
#include "ApplicationController.h"
#include <QtCharts>

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
    QMap<QString,QChart*> charts;
    QVector<QChartView*> chartViews;

    UnitSettingsDialog* unitSettings = nullptr;

    void initMainChartViews();
    void initMainWindow();
    void initUnitSettingsDialog();
    void initChartViews();
    void addChartView(QBoxLayout* layout, QString name);
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