#pragma once
#include <QtWidgets>
#include "ui_ColorScheme.h"

class ColorSchemeDialog : public QDialog
{
	Q_OBJECT
	Ui::ColorScheme *ui;
public:
	ColorSchemeDialog(QWidget* parent);
	~ColorSchemeDialog();
	Ui::ColorScheme* getUi();
};

