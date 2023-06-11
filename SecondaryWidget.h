#pragma once
#include "ChannelWidget.h"

class SecondaryWidget : public ChannelWidget
{
    Q_OBJECT
    QVector<InteractiveStrobe*> strobes;
    InteractiveStrobe* selectedStrobe;
    bool strobeIsChanged = false;
public:
    SecondaryWidget(QWidget* parent = 0, QChart* chart = 0);
    ~SecondaryWidget();
};