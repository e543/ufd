#pragma once
#include <QUdpSocket>
#include <QDebug>



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

class Receiver : public QObject
{
    Q_OBJECT
    quint8 osc[256];
    amp_struct_t data;
public:
    Receiver();
    ~Receiver();
    void setConnection(const QHostAddress& address, quint16 port);
    void disconnect();
    amp_struct_t fetchData();
signals:
    void dataReceived();
private slots:
    void processPendingDatagrams();
private:
    QUdpSocket* udpSocket = nullptr;
};

