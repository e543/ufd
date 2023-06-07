#pragma once
#include <QUdpSocket>
#include <QDebug>
#include "Server.h"


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
    void sendStrobes(quint8 channel, qreal time, const QVector<QPair<qreal, QPointF>>& points);
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

