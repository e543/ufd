#pragma once
#include <QtWidgets>
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
    QHash<QString, QChartView*> chartViews;
    QHash<QString, QLabel*> channelLabels;
    quint8 channelCount;
    ChartWidget* firstWidget;
    Server* server = nullptr;

    UnitSettingsDialog* unitSettings = nullptr;

    void initMainChartViews();
    void initMainWindow();
    void initUnitSettingsDialog();
    void initChartViews();
    void addChartView(QBoxLayout* layout, QString name);
    void bindChannelLabels();
    void updateSettings();
    void bindContext();
    Context* getContext();
    Settings* getSettings();

    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    Ui::MainWindow* ui;
private slots:
    void on_SettingsClicked();
    void readASkanRazvTables(int i);
    void start();
    void startStopButton();
    void initServer();
    void channelClicked();
protected:
    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);
    bool eventFilter(QObject* obj, QEvent* e);
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};