#pragma once
#include "ConnectionManager.h"

class ApplicationController
{
    Context* context;
	ConnectionManager* connectManager;
//private slots:
    void updateSecondSeries();
public:
    ApplicationController(Context* context) : context(context) 
    {
        context->timer = new QTimer();
        connectManager = new ConnectionManager(context);
    }
    void connectDataSeries();
    void startUpdateSecondSeries();
};

