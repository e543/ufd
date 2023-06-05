#include "MainWindow.h"




void MainWindow::showEvent(QShowEvent* e)
{
    QMainWindow::showEvent(e);
    static bool firstStart = true;
    if (firstStart)
    {
        emit start();
        firstStart = false;
    }
}

void MainWindow::start()
{
    firstWidget->addStrobe({ 110,110 });
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (server) delete server;
    server = nullptr;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* e)
{
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F3)
    {
        emit ui->StopButton->clicked(); 
    }
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), context(new Context) , settings(new Settings)
{
    initMainWindow();
    initUnitSettingsDialog();
    bindContext();
}

MainWindow::~MainWindow()
{
    
    if (unitSettings) delete unitSettings;
    for (auto* view : chartViews) delete view;
    if (server) delete server;
    delete ui;
}

void MainWindow::initMainWindow()
{
    ui->setupUi(this);

    settings->availableSize = QDesktopWidget().availableGeometry(this).size();
    updateSettings();

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);
    ui->splitter->setMinimumSize(QSize(300, 300));

    bindChannelLabels();
    initMainChartViews();
    initChartViews();
    indexLabels();

    context->connectionActive = false;
    connect(ui->ASkanRazvComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(readASkanRazvTables(int)));
    connect(ui->StopButton, SIGNAL(clicked()), this, SLOT(startStopButton()));
    connect(ui->ServerButton, SIGNAL(clicked()), this, SLOT(initServer()));

    connect(channelLabels["11"], SIGNAL(clicked()), this, SLOT(channelClicked()));
    connect(channelLabels["12"], SIGNAL(clicked()), this, SLOT(channelClicked()));
    connect(channelLabels["21"], SIGNAL(clicked()), this, SLOT(channelClicked()));
    connect(channelLabels["22"], SIGNAL(clicked()), this, SLOT(channelClicked()));
    connect(channelLabels["31"], SIGNAL(clicked()), this, SLOT(channelClicked()));
    connect(channelLabels["32"], SIGNAL(clicked()), this, SLOT(channelClicked()));
    connect(channelLabels["41"], SIGNAL(clicked()), this, SLOT(channelClicked()));
    connect(channelLabels["42"], SIGNAL(clicked()), this, SLOT(channelClicked()));
}

void MainWindow::initUnitSettingsDialog()
{
    if (unitSettings)
        return;

    unitSettings = new UnitSettingsDialog(this);
    connect(ui->SettingsButton, SIGNAL(clicked()), this, SLOT(on_SettingsClicked()));
}

void MainWindow::updateSettings()
{

}

void MainWindow::bindContext()
{
    context->ui_MainWindow = ui;
    context->ui_UnitSettings = unitSettings->getUi();
    context->ui_ColorScheme = unitSettings->getColorSchemeDialog()->getUi();
    context->firstWidget = firstWidget;
    context->currentLabel = nullptr;
    context->channelSelected = false;
    context->channelChanged = false;
}

void MainWindow::initChartViews()
{
    addChartView(ui->horizontalLayout_sum, "sum");
    addChartView(ui->horizontalLayout_11, "11");
    addChartView(ui->horizontalLayout_12, "12");
    addChartView(ui->horizontalLayout_21, "21");
    addChartView(ui->horizontalLayout_22, "22");
    addChartView(ui->horizontalLayout_31, "31");
    addChartView(ui->horizontalLayout_32, "32");
    addChartView(ui->horizontalLayout_41, "41");
    addChartView(ui->horizontalLayout_42, "42");
}

void MainWindow::initSeries()
{

}

void MainWindow::bindChannelLabels()
{
    channelLabels["11"] = ui->label_11;
    channelLabels["12"] = ui->label_12;
    channelLabels["21"] = ui->label_21;
    channelLabels["22"] = ui->label_22;
    channelLabels["31"] = ui->label_31;
    channelLabels["32"] = ui->label_32;
    channelLabels["41"] = ui->label_41;
    channelLabels["42"] = ui->label_42;
}



void MainWindow::initMainChartViews()
{
    QSplitter* splitter = new QSplitter(Qt::Vertical, ui->frame_3);

    ui->verticalLayout_main->addWidget(splitter);

    firstWidget = new ChartWidget(ui->centralwidget);
    auto* firstChart = firstWidget->getChart(); 
    firstWidget->setObjectName("chartView_first");

    splitter->addWidget(firstWidget);

    addChartView(ui->verticalLayout_main, "second");
    auto* secondView = chartViews["second"];
    auto* secondChart = secondView->chart();
    secondView->setMinimumWidth(300);
    secondView->setMinimumHeight(250);

    splitter->addWidget(secondView);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    splitter->setChildrenCollapsible(false);

    QValueAxis* secondAxisX = new QValueAxis;
    secondAxisX->setRange(0, 255);
    secondAxisX->setTickCount(7);
    QValueAxis* secondAxisY = new QValueAxis;
    secondAxisY->setRange(0, 255);
    secondAxisY->setTickCount(7);
    secondChart->addAxis(secondAxisX, Qt::AlignLeft);
    secondChart->addAxis(secondAxisY, Qt::AlignRight);

}

void MainWindow::addChartView(QBoxLayout* layout, QString name)
{
    auto* chart = new QChart();
    auto* chartView = new QChartView(ui->centralwidget);
    chartViews[name] = chartView;
    layout->addWidget(chartView);

    chartView->setObjectName("chartView_" + name);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHeightForWidth(chartView->sizePolicy().hasHeightForWidth());
    chartView->setSizePolicy(sizePolicy);
    chartView->setMaximumSize(QSize(3000, 3000));
    chart->setMinimumHeight(0);

    chartView->setChart(chart);
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);
    chart->setMargins(QMargins{ 0 , 0 ,0 , 0 });
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    updateSettings();
}

Context* MainWindow::getContext()
{
	return context;
}

Settings* MainWindow::getSettings()
{
	return settings;
}

void MainWindow::on_SettingsClicked()
{
    if (unitSettings) 
        unitSettings->exec();
}


void MainWindow::readASkanRazvTables(int i)
{
    switch (i)
    {
    default:
    case 0:
        firstWidget->setVisible(true);
        chartViews["second"]->setVisible(true);
        break;
    case 1:
        firstWidget->setVisible(true);
        chartViews["second"]->setVisible(false);
        break;
    case 2:
        firstWidget->setVisible(false);
        chartViews["second"]->setVisible(true);
        break;
    }

    context->isAskanVisible = firstWidget->isVisible();
    context->isRazvVisible = chartViews["second"]->isVisible();
}

void MainWindow::startStopButton()
{
    auto* timer = context->timer;
    if (!timer) {
        return;
    }

    if (context->connectionActive) {
        timer->stop();
        ui->StopButton->setText(QString::fromUtf16(u"Старт F3"));
        context->connectionActive = false;
        ui->label_7->setStyleSheet("QLabel { background-color : red; }");
    }
    else {
        /*timer->setInterval(0);
        timer->setSingleShot(true);*/
        timer->start(1);
        ui->StopButton->setText(QString::fromUtf16(u"Стоп F3"));
        context->connectionActive = true;
        ui->label_7->setStyleSheet("QLabel { background-color : green; }");
    }
}

void MainWindow::initServer()
{
    if (server)
    {
        delete server;
    }

    server = new Server;
    server->show();
}

void MainWindow::indexLabels()
{
    channelLabels["11"]->setIndex(0);
    channelLabels["12"]->setIndex(1);
    channelLabels["21"]->setIndex(2);
    channelLabels["22"]->setIndex(3);
    channelLabels["31"]->setIndex(4);
    channelLabels["32"]->setIndex(5);
    channelLabels["41"]->setIndex(6);
    channelLabels["42"]->setIndex(7);
}

void MainWindow::channelClicked()
{
    auto* label = qobject_cast<ClickedLabel*>(sender());

    if (label->isClicked()){
        label->setNotClicked();
        label->setStyleSheet("");
        firstWidget->setStyleSheet("");
        context->currentLabel = nullptr;
        context->channelSelected = false;
    }
    else {
        label->setClicked();
        label->setStyleSheet("border:  3px dashed blue;");
        firstWidget->setStyleSheet("border:  3px dashed blue;");
        context->selectedChannel = label->getIndex();
        context->channelSelected = true;

        if (context->currentLabel) {
            context->currentLabel->setNotClicked();
            context->currentLabel->setStyleSheet("");
        }
        context->currentLabel = label;
        context->channelChanged = true;
    }
}
