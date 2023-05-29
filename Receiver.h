#pragma once
#include <QUdpSocket>
#include <QDebug>

class Receiver : public QObject
{
    Q_OBJECT
public:
    Receiver();
    ~Receiver();
    void setConnection(const QHostAddress& address, quint16 port);
    void disconnect();
private slots:
    void processPendingDatagrams();

private:
    QUdpSocket* udpSocket = nullptr;
};

