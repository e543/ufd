#include "Receiver.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>


Receiver::Receiver()
{
    osc = new quint8[256];
}
Receiver::~Receiver()
{
    udpSocket->close();
    delete udpSocket;
}

void Receiver::setConnection(const QHostAddress& address, quint16 port)
{
    udpSocket = new QUdpSocket(this);

    if (!udpSocket->bind(QHostAddress::LocalHost, port))
    {
        qDebug() << udpSocket->errorString();
        return;
    }
    connect(udpSocket, &QUdpSocket::readyRead,
        this, &Receiver::processPendingDatagrams);
}

void Receiver::disconnect()
{
    QObject::disconnect(udpSocket, &QUdpSocket::readyRead,
        this, &Receiver::processPendingDatagrams);
    delete udpSocket;
}

quint8* Receiver::fetchData()
{
    return osc;
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

        for (int i = 0; i < 256; ++i) {
            in >> osc[i];
        }
        emit dataReceived();
    }
}