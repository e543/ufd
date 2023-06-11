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
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class Application;

    Settings* settings;
    Context* context;
    QVector<ChannelWidget*> channels;
    QHash<QString, QChartView*> chartViews;
    QHash<QString, ClickedLabel*> channelLabels;
    QVector<QVector<QXYSeries*>> channelSeries;
    ChartWidget* firstWidget;
    Server* server = nullptr;

    UnitSettingsDialog* unitSettings = nullptr;

    void initMainChartViews();
    void initMainWindow();
    void initUnitSettingsDialog();
    void initChartViews();
    void initSeries();
    void indexLabels();
    void addChartView(QBoxLayout* layout, QString name);
    void addChannel(QBoxLayout* layout, int ind);
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
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* e) override;
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};