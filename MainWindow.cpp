#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), context(new Context) , settings(new Settings)
{
    initMainWindow();
    initUnitSettingsDialog();
    bindContext();
}

MainWindow::~MainWindow()
{
    if (colorScheme) delete colorScheme;
    if (unitSettings) delete unitSettings;
}

void MainWindow::initMainWindow()
{
    ui_MainWindow.setupUi(this);

    settings->availableSize = QDesktopWidget().availableGeometry(this).size();
    updateSettings();
}

void MainWindow::initColorSchemeDialog()
{
    if (unitSettings)
        colorScheme = new ColorSchemeDialog(unitSettings);
}

void MainWindow::initUnitSettingsDialog()
{
    unitSettings = new UnitSettingsDialog(this);
    initColorSchemeDialog();
}

void MainWindow::updateSettings()
{

}

void MainWindow::bindContext()
{
    context->ui_MainWindow = ui_MainWindow;
    context->ui_UnitSettings = unitSettings->getUi();
    context->ui_ColorScheme = colorScheme->getUi();
}

void MainWindow::initGraphicsView()
{

}

void MainWindow::initSecondaryView()
{

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
