#include "Receiver.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>


Receiver::Receiver()
{
}
Receiver::~Receiver()
{
    udpSocket->close();
    delete udpSocket;
}

void Receiver::setConnection(const QHostAddress& address, quint16 port)
{
    udpSocket = new QUdpSocket(this);

    if (!udpSocket->bind(address, port))
    {
        qDebug() << udpSocket->errorString();
        return;
    }
    connect(udpSocket, &QUdpSocket::readyRead,
        this, &Receiver::processPendingDatagrams);
    
    /*QList<QHostAddress> broadcastAddresses;
    QList<QHostAddress> ipAddresses; 
    broadcastAddresses.clear();
    ipAddresses.clear();
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& interface : interfaces) {
        const QList<QNetworkAddressEntry> entries = interface.addressEntries();
        for (const QNetworkAddressEntry& entry : entries) {
            QHostAddress broadcastAddress = entry.broadcast();
            if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost) {
                broadcastAddresses << broadcastAddress;
                ipAddresses << entry.ip();
            }
        }
    }

    for (auto ip : ipAddresses) {
        qDebug() << ip.toString();
    }*/
}

void Receiver::disconnect()
{
    QObject::disconnect(udpSocket, &QUdpSocket::readyRead,
        this, &Receiver::processPendingDatagrams);
    delete udpSocket;
}

void Receiver::processPendingDatagrams()
{
    QByteArray datagram;
    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream in(&datagram, QIODevice::ReadOnly);

        qint64 size = -1;
        if (in.device()->size() > sizeof(qint64)) {
            in >> size;
        }
        else return;
        if (in.device()->size() - sizeof(qint64) < size) return;

        qint8 signal1 = 0;
        qint8 signal2 = 0;
        qint8 signal3 = 0;
        qint8 signal4 = 0; 
        qint8 signal5 = 0;
        qint8 signal6 = 0;
        qint8 signal7 = 0;
        qint8 signal8 = 0;

        in >> signal1 >> signal2 >> signal3 >> signal4 >> signal5 >> signal6 >> signal7 >> signal8;

        qDebug() << signal1 << signal2 << signal3 << signal4 << signal5 << signal6 << signal7 << signal8;
    }
}