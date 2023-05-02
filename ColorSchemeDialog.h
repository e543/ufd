#pragma once
#include <QtWidgets>
#include "ui_ColorScheme.h"

class ColorSchemeDialog : public QDialog
{
	Ui::ColorScheme ui_ColorScheme;
public:
	ColorSchemeDialog(QWidget* parent = nullptr) : QDialog(parent) 
	{
		ui_ColorScheme.setupUi(this);
	};

	Ui::ColorScheme& getUi();
};

