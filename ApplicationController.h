#pragma once
#include "ConnectionManager.h"
#include "ColorSchemeDialog.h"
#include "UnitSettingsDialog.h"

#include "ui_MainWindow.h"
#include "UnitSettingsDialog.h"
#include "ColorSchemeDialog.h"

struct Context
{
    Ui::MainWindow* ui_MainWindow;
    Ui::UnitSettings* ui_UnitSettings;
    Ui::ColorScheme* ui_ColorScheme;
};

class ApplicationController
{
    Context* context;
	ConnectionManager connectManager;
public:
    ApplicationController(Context* context) : context(context) 
    {
        RenderContext();
    }
    void RenderContext();
};

