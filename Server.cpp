#include "Server.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QDebug>

Server::Server(QWidget* parent)
    : QWidget(parent)
{
    statusLabel = new QLabel(tr("Ready to broadcast datagrams"));
    statusLabel->setWordWrap(true);

    startButton = new QPushButton(tr("&Start"));
    auto quitButton = new QPushButton(tr("&Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);


    connect(startButton, &QPushButton::clicked, this, &Server::startListening);
    connect(quitButton, &QPushButton::clicked, this, &Server::disconnect);

    numChannel = -1;
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    for (int i = 0; i < 256; ++i) {
        osc[i] = i;
    }

    setWindowTitle(tr("Server"));
}

void Server::startListening()
{
    udpSocket = new QUdpSocket(this);
    startButton->setEnabled(false);
    if (!udpSocket->bind(QHostAddress::LocalHost, 8080))
    {
        qDebug() << udpSocket->errorString();
        return;
    }
    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::callBackDatagram);
}

void Server::callBackDatagram()
{
    QByteArray datagram;
    QHostAddress address;

    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size(),&address, &port);

        QDataStream in(&datagram, QIODevice::ReadOnly);

        qint64 size = -1;
        if (in.device()->size() > sizeof(qint64)) {
            in >> size;
        }
        else return;
        if (in.device()->size() - sizeof(qint64) < size) return;

        in >> command;
        sendCallBack(in);
    }

}

void Server::dataOsc(QDataStream& out)
{
    for (int i = 0; i < 256; ++i) {
        out << osc[i];
    }

    for (int i = 0; i < 256; ++i) {
        osc[i] += 1;
    }
}

void Server::dataStrobe(QDataStream& out)
{
    auto& channel = input.limits[numChannel];
    int i = 0;
    for (auto& strobe : channel) {
        int xmax = 0;
        auto y = strobe.first;
        qreal ymax = y;
        auto& point = strobe.second;
        bool found = false;
        for (int i = 0; i < 256; ++i) {
            if (osc[i] > ymax && i >= point.x() && i <= point.y()) {
                ymax = osc[i];
                xmax = i;
                found = true;
            }
        }
        if (found) {
            auto& strb = data.ampl_tact[numChannel / 2].ampl_us[numChannel % 2].ampl[i];
            strb.ampl = ymax;
            strb.time = input.time / xmax;
            out << quint8(i) << QPointF{ qreal(xmax), ymax };
            //qDebug() << i << QPointF{ qreal(xmax), ymax };
        }
        ++i;
    }
}

void Server::strobesReceived(QDataStream& in)
{
    in >> numChannel;
    in >> input.time;

    quint8 i = numChannel;
    for (auto& strobe : input.limits[i]) {
        in >> strobe;
    }
}

void Server::sendCallBack(QDataStream& in)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << qint64(0);

    if (command == "o") {
        out << QString("o");
        dataOsc(out);
    }
    else 
    if (command == "a") {
        if (numChannel == 255)
            return;
        out << QString("a");
        dataStrobe(out);
    }
    else
    if (command == "s") {
        out << QString("s");
        strobesReceived(in);
    }

    out.device()->seek(qint64(0));
    out << qint64(data.size() - sizeof(qint64));
    udpSocket->writeDatagram(data, QHostAddress::LocalHost, port);
}


void Server::disconnect()
{
    QObject::disconnect(udpSocket, &QUdpSocket::readyRead, this, &Server::callBackDatagram);
    if (udpSocket) 
        delete udpSocket;
    Server::close();
}
