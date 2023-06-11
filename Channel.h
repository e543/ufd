#pragma once
#include "ChannelWidget.h"

class Channel : public ChannelWidget
{
    Q_OBJECT
public:
    Channel(QWidget* parent = 0, QChart* chart = 0);
    ~Channel();
};
