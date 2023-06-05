#pragma once
#include "ConnectionManager.h"

class ApplicationController
{
    Context* context;
	ConnectionManager* connectManager;
public:
    ApplicationController(Context* context) : context(context) 
    {
        context->timer = new QTimer();
        context->fpsTimer = new QElapsedTimer();
        connectManager = new ConnectionManager(context);
    }
    void connectDataSeries();
};

