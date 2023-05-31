#include "Server.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>

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


    connect(startButton, &QPushButton::clicked, this, &Server::startBroadcasting);
    connect(quitButton, &QPushButton::clicked, this, &Server::disconnect);
    connect(&timer, &QTimer::timeout, this, &Server::broadcastDatagram);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Sender"));
}

void Server::startBroadcasting()
{
    udpSocket = new QUdpSocket(this);
    startButton->setEnabled(false);
    timer.start();
}

void Server::broadcastDatagram()
{
    statusLabel->setText(tr("Now broadcasting datagram %1").arg(messageNo));

    //QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << qint64(0);
    out << qint8(messageNo) << qint8(255 - messageNo) << qint8(messageNo) << qint8(messageNo)
        << qint8(messageNo) << qint8(messageNo) << qint8(messageNo) << qint8(messageNo);
    out.device()->seek(qint64(0));
    out << qint64(data.size() - sizeof(qint64));

    udpSocket->writeDatagram(data, QHostAddress("192.168.1.64"), 8080);
    messageNo = ++messageNo % 256;
}

void Server::disconnect()
{
    QObject::disconnect(&timer, &QTimer::timeout, this, &Server::broadcastDatagram);
    if (udpSocket) 
        delete udpSocket;
    Server::close();
}
