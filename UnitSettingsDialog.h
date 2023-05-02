#pragma once
#include <QtWidgets>
#include "ui_UnitSettings.h"

class UnitSettingsDialog : public QDialog
{
	Ui::UnitSettings ui_UnitSettings;
public:
	UnitSettingsDialog(QWidget* parent = nullptr) : QDialog(parent) 
	{
		ui_UnitSettings.setupUi(this);
	};
	Ui::UnitSettings& getUi();
};

