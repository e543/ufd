#pragma once

#include "DataSource.h"
#include <QUdpSocket>

#define NUM_RESULT_STRBS    5
#define NUM_USS             2
#define NUM_TACTS            4

struct amp_strob_struct_t
{
    quint16 time;
    quint8 ampl;
};

struct amp_us_struct_t
{
    amp_strob_struct_t ampl[NUM_RESULT_STRBS];
};

struct amp_tact_struct_t
{
    amp_us_struct_t ampl_us[NUM_USS];
};

struct amp_struct_t
{
    amp_tact_struct_t ampl_tact[NUM_TACTS];
};




class ConnectionManager
{
	quint8 osc[256];
    amp_struct_t data;
    QUdpSocket* udpSocket;
    DataSource* dataSource;
    QTimer* timer;
public:
    ConnectionManager(QTimer* timer);
    void setConnection(QChart* chart, QXYSeries* series);
};

