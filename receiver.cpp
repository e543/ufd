#include "Receiver.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>


Receiver::Receiver() : udpSocket(new QUdpSocket(this))
{
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead,
        this, &Receiver::processPendingDatagrams);
}
Receiver::~Receiver()
{
    udpSocket->close();
    delete udpSocket;
}

void Receiver::setConnection(const QHostAddress& address, quint16 port = 45454)
{
    udpSocket->bind(address, port, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead,
        this, &Receiver::processPendingDatagrams);
}

void Receiver::disconnect()
{
    QObject::disconnect(udpSocket, &QUdpSocket::readyRead,
        this, &Receiver::processPendingDatagrams);
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

        qint8 type = 0;
        in >> type;

        qDebug() << type;
    }
}