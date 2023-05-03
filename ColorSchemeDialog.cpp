#include "ColorSchemeDialog.h"

ColorSchemeDialog::ColorSchemeDialog(QWidget* parent = nullptr) : QDialog(parent), ui(new Ui::ColorScheme)
{
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

	ui->setupUi(this);

};

ColorSchemeDialog::~ColorSchemeDialog()
{
	delete ui;
}

Ui::ColorScheme* ColorSchemeDialog::getUi()
{
	return ui;
}
