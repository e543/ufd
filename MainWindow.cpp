#include "MainWindow.h"


void MainWindow::closeEvent(QCloseEvent* event)
{
}

bool MainWindow::eventFilter(QObject* obj, QEvent* e)
{
    return false;
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
    for (auto* chart : charts) delete chart;
    for (auto* chartView : chartViews) delete chartView;
    delete ui;
}

void MainWindow::initMainWindow()
{
    ui->setupUi(this);

    settings->availableSize = QDesktopWidget().availableGeometry(this).size();
    updateSettings();

    initMainChartViews();
    initChartViews();
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
    addChartView(ui->horizontalLayout_4pov, "_4pov");
    addChartView(ui->horizontalLayout_4pon, "4pon");
    addChartView(ui->horizontalLayout_5rl, "5rl");
    addChartView(ui->horizontalLayout_6rl, "6rl");
}



void MainWindow::initMainChartViews()
{
    addChartView(ui->verticalLayout_main, "first");
    addChartView(ui->verticalLayout_main, "second");

    auto* first = charts["first"];
    auto* second = charts["second"];

    QValueAxis* firstAxisX = new QValueAxis;
    firstAxisX->setRange(0, 10);
    QValueAxis* firstAxisY = new QValueAxis;
    firstAxisY->setRange(0, 255);

    first->addAxis(firstAxisX, Qt::AlignBottom);
    first->addAxis(firstAxisY, Qt::AlignLeft);
    first->legend()->hide();

    QValueAxis* secondAxisX = new QValueAxis(firstAxisX);
    secondAxisX->setRange(0, 10);
    QValueAxis* secondAxisY = new QValueAxis(firstAxisY);
    secondAxisY->setRange(0, 255);
    second->addAxis(secondAxisX, Qt::AlignBottom);
    second->addAxis(secondAxisY, Qt::AlignLeft);
    second->legend()->hide();
}

void MainWindow::addChartView(QBoxLayout* layout, QString name)
{
    QChart* chart = new QChart();
    QChartView* chartView = new QChartView(ui->centralwidget);
    charts[name] = chart;
    chartViews.append(chartView);
    layout->addWidget(chartView);

    chartView->setObjectName("chartView_" + name);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
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