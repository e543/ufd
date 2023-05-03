#include "UnitSettingsDialog.h"

UnitSettingsDialog::UnitSettingsDialog(QWidget* parent = nullptr) : QDialog(parent), ui(new Ui::UnitSettings)
{
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

	ui->setupUi(this);
	colorScheme = new ColorSchemeDialog(this);
	connect(ui->ColorSchemeButton, SIGNAL(clicked()), this, SLOT(on_ColorSchemeClicked()));
};

UnitSettingsDialog::~UnitSettingsDialog()
{
	if (colorScheme) delete colorScheme;
	delete ui;
}

Ui::UnitSettings* UnitSettingsDialog::getUi()
{
	return ui;
}

ColorSchemeDialog* UnitSettingsDialog::getColorSchemeDialog()
{
	return colorScheme;
}

void UnitSettingsDialog::on_ColorSchemeClicked()
{
	if (colorScheme)
		colorScheme->exec();
}
