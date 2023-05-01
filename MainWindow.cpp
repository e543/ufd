#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), context(new Context) , settings(new Settings)
{
    initMainWindow();
    initColorSchemeWidget();
    initUnitSettingsWidget();
    bindContext();
}

MainWindow::~MainWindow()
{
    if (graphicsView) delete graphicsView;
    if (secondaryView) delete secondaryView;
    if (centralWidget) delete centralWidget;
    if (colorScheme) delete colorScheme;
    if (unitSettings) delete unitSettings;
    if (MultiChannelScanningWidget) delete MultiChannelScanningWidget;
    channels.clear();
}

void MainWindow::initMainWindow()
{
    ui.setupUi(this);

    settings->availableSize = QDesktopWidget().availableGeometry(this).size();
    updateSettings();

    /*initGraphicsView(); 
    initSecondaryView();
    initMultiChannelScanning();*/

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::initColorSchemeWidget()
{

}

void MainWindow::initUnitSettingsWidget()
{
}

void MainWindow::updateSettings()
{
   auto const size = this->size();

   settings->windowSize = size;
   settings->graphicsRect = QRect(
       settings->availableSize.width() * 0.3, 
       settings->availableSize.height() * 0.02, 
       settings->availableSize.width() * 0.3, 
       settings->availableSize.height() * 0.3);
   settings->secondaryRect = QRect(
       settings->availableSize.width() * 0.3,
       settings->availableSize.height() * 0.36,
       settings->availableSize.width() * 0.3,
       settings->availableSize.height() * 0.3);
}

void MainWindow::bindContext()
{
    context->centralWidget = centralWidget;
    context->coloreScheme = colorScheme;
    context->unitSettings = unitSettings;
    context->MultiChannelScanningWidget = MultiChannelScanningWidget;
    context->channels = channels;

    context->graphicsView = graphicsView;
    context->secondaryView = secondaryView;
    context->MultiChannelVerticalLayout = MultiChannelVerticalLayout;
    context->MultiChannelHorizontalLayout = MultiChannelHorizontalLayout;
}

void MainWindow::initMultiChannelScanning()
{
    MultiChannelScanningWidget = new QWidget(this);
    MultiChannelScanningWidget->setGeometry(QRect(50, 0, 500, 700));

    MultiChannelVerticalLayout = new QVBoxLayout(MultiChannelScanningWidget);
    MultiChannelVerticalLayout->setSpacing(0);
    MultiChannelVerticalLayout->setContentsMargins(0, 10, 0, 10);
    MultiChannelVerticalLayout->setObjectName(QString::fromUtf8("Multi Channel Vertical Layout"));
    MultiChannelVerticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

    for (int i = 0; i < settings->channelsCount; ++i) {
        channels[QString::number(i)] = new QChartView(MultiChannelScanningWidget);
        auto channel = channels[QString::number(i)];
        channel->setObjectName(QString::fromUtf8("channel ") + QString::number(i));
        channel->setStyleSheet(QString::fromUtf8("border:  2px dashed blue;"));

        MultiChannelVerticalLayout->addWidget(channel);
    }
}

void MainWindow::initGraphicsView()
{
    graphicsView = new QChartView(this);
    graphicsView->setMinimumSize(settings->graphicsRect.size());
    graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
    graphicsView->setGeometry(settings->graphicsRect);
    graphicsView->setAutoFillBackground(false);
    graphicsView->setStyleSheet(QString::fromUtf8("border:  3px dashed blue;"));
    graphicsView->setInteractive(true);
    graphicsView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::initSecondaryView()
{
    secondaryView = new QChartView(this);
    secondaryView->setMinimumSize(settings->secondaryRect.size());
    secondaryView->setObjectName(QString::fromUtf8("secondaryView"));
    secondaryView->setGeometry(settings->secondaryRect);
    secondaryView->setAutoFillBackground(false);
    secondaryView->setStyleSheet(QString::fromUtf8("border:  3px dashed red;"));
    secondaryView->setInteractive(true);
    secondaryView->setRenderHint(QPainter::Antialiasing);
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
