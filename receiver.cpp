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

amp_struct_t Receiver::fetchData()
{
    return data;
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

        in >> data.ampl_tact[0].ampl_us[0].ampl[0].ampl 
            >> data.ampl_tact[0].ampl_us[1].ampl[0].ampl
            >> data.ampl_tact[1].ampl_us[0].ampl[0].ampl
            >> data.ampl_tact[1].ampl_us[1].ampl[0].ampl
            >> data.ampl_tact[2].ampl_us[0].ampl[0].ampl
            >> data.ampl_tact[2].ampl_us[1].ampl[0].ampl
            >> data.ampl_tact[3].ampl_us[0].ampl[0].ampl
            >> data.ampl_tact[3].ampl_us[1].ampl[0].ampl;

        emit dataReceived();
    }
}