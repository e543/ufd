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

struct Result
{
    QString command;
    QVector<quint8> osc;
    amp_struct_t data;
};

class Client : public QObject
{
    Q_OBJECT
    Result result;
    quint16 port = 8080;
public:
    Client();
    ~Client();
    void sendCommand(QString command);
    void setConnection();
    void disconnect();
    Result fetchData();
signals:
    void dataReceived();
private slots:
    void processPendingDatagrams();
private:
    QUdpSocket* udpSocket = nullptr;
};

