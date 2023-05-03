#pragma once
#include <QtWidgets>
#include "ui_UnitSettings.h"
#include "ColorSchemeDialog.h"

class UnitSettingsDialog : public QDialog
{
	Q_OBJECT
	Ui::UnitSettings* ui;
	ColorSchemeDialog* colorScheme = nullptr;
private slots:
	void on_ColorSchemeClicked();
public:
	UnitSettingsDialog(QWidget* parent);
	~UnitSettingsDialog();
	Ui::UnitSettings* getUi();
	ColorSchemeDialog* getColorSchemeDialog();
};

