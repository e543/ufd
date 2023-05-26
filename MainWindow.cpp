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
}

bool MainWindow::eventFilter(QObject* obj, QEvent* e)
{
    int i = 0;
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F3)
    {
        emit startStopButton(); 
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

    initMainChartViews();
    initChartViews(); 
}

void MainWindow::initUnitSettingsDialog()
{
    if (unitSettings)
        return;

    unitSettings = new UnitSettingsDialog(this);
    connect(ui->SettingsButton, SIGNAL(clicked()), this, SLOT(on_SettingsClicked()));
    connect(ui->ASkanRazvComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(readASkanRazvTables(int)));
    connect(ui->StopButton, SIGNAL(clicked()), this, SLOT(startStopButton()));
    connect(this, SIGNAL(key_f3_pressed_event), this, SLOT(startStopButton()));
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
}

void MainWindow::initChartViews()
{
    addChartView(ui->horizontalLayout_sum, "sum");
    addChartView(ui->horizontalLayout_1prv, "1prv");
    addChartView(ui->horizontalLayout_1prn, "1prn");
    addChartView(ui->horizontalLayout_2prv, "2prv");
    addChartView(ui->horizontalLayout_2prn, "2prn");
    addChartView(ui->horizontalLayout_3pov, "3pov");
    addChartView(ui->horizontalLayout_3pon, "3pon");
    addChartView(ui->horizontalLayout_4pov, "4pov");
    addChartView(ui->horizontalLayout_4pon, "4pon");
    addChartView(ui->horizontalLayout_5rl, "5rl");
    addChartView(ui->horizontalLayout_6rl, "6rl");
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

    if (timer->isActive()) {
        timer->stop();
        ui->StopButton->setText(QString::fromUtf16(u"����� F3"));
    }
    else {
        timer->start();
        ui->StopButton->setText(QString::fromUtf16(u"���� F3"));
    }
}