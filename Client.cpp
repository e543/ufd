#include "Client.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>


Client::Client()
{
    result.osc.resize(256);
}
Client::~Client()
{
    udpSocket->close();
    delete udpSocket;
}

void Client::sendCommand(QString command)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    out << qint64(0);
    out << command;
    out.device()->seek(qint64(0));
    out << qint64(data.size() - sizeof(qint64));

    udpSocket->writeDatagram(data, QHostAddress::LocalHost, 8080);
}

void Client::setConnection()
{
    udpSocket = new QUdpSocket(this);
    this->port = port;

    if (!udpSocket->bind(QHostAddress::LocalHost, 8081))
    {
        qDebug() << udpSocket->errorString();
        return;
    }
    connect(udpSocket, &QUdpSocket::readyRead,
        this, &Client::processPendingDatagrams);
}

void Client::disconnect()
{
    QObject::disconnect(udpSocket, &QUdpSocket::readyRead,
        this, &Client::processPendingDatagrams);
    delete udpSocket;
}

Result Client::fetchData()
{
    return result;
}

void Client::processPendingDatagrams()
{
    QByteArray datagram;
    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream in(&datagram, QIODevice::ReadOnly);

        result.osc.clear();
        result.osc.resize(256);

        qint64 size = -1;
        if (in.device()->size() > sizeof(qint64)) {
            in >> size;
        }
        else return;
        if (in.device()->size() - sizeof(qint64) < size) return;

        in >> result.command;
        if (result.command == "o") {
            for (int i = 0; i < 256; ++i) {
                in >> result.osc[i];
                //qDebug() << osc[i];
            }
            emit dataReceived();
        }
        else 
        if (result.command == "a") {
            // --- code
            emit dataReceived();
        }
    }
}