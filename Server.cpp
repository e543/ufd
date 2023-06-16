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

    initOsc();
    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &Server::updateOsc);
    timer->start();

    setWindowTitle(tr("Server"));
}

void Server::startListening()
{
    udpSocket = new QUdpSocket(this);
    startButton->setEnabled(false);
    if (!udpSocket->bind(QHostAddress("192.168.1.164"), 8080))
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

        if (command == QString("n")) {
            in >> numChannel;
        } 
        else {
            sendCallBack(in);
        }
    }

}

void Server::initOsc()
{
    for (int i = 0; i < 256; ++i) {
        oscInit[0][i] = osc[0][i] = 2 * i;
        oscInit[1][i] = osc[1][i] = -2 * i;
        oscInit[2][i] = osc[2][i] = 4 * i;
        oscInit[3][i] = osc[3][i] = -4 * i;
        oscInit[5][i] = osc[5][i] = -6 * i;
        oscInit[4][i] = osc[4][i] = 6 * i;
        oscInit[6][i] = osc[6][i] = 8 * i;
        oscInit[7][i] = osc[7][i] = -8 * i;
    }
}

void Server::updateOsc()
{
    int gap = 2;
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 256; ++i) {
            if (osc[j][i] <= 255 - gap  || osc[j][i] >= gap)
                osc[j][i] = oscInit[j][i] + (- gap + QRandomGenerator::global()->generate64() % gap);
        }
    }

    /*for (int i = 0; i < 256; ++i) {
        osc[i] += 0;
    }*/
}

void Server::sendOsc(QDataStream& out)
{
    for (int i = 0; i < 256; ++i) {
        out << osc[numChannel][i];
    }

    /*for (int i = 0; i < 256; ++i) {
        osc[i] += 0;
    }*/
}

void Server::dataStrobe(QDataStream& out)
{
    for (int k = 0; k < 8; ++k) {
        auto& channel = input.limits[k];
        for (int j = 0; j < 5; ++j) {
            auto& strobe = channel[j];
            int xmax = 0;
            auto y = strobe.first;
            qreal ymax = 0;
            auto& point = strobe.second;
            qreal x = 0;
            for (int i = 0; i < 256; ++i, x += delta) {
                if (osc[k][i] > ymax && x >= point.x() && x <= point.y()) {
                    xmax = i;
                    ymax = osc[k][i];
                }
            }

            auto& strb = data.ampl_tact[k / 2].ampl_us[k % 2].ampl[j];

            strb.time = xmax;
            strb.ampl = ymax;

            out << quint8(k) << quint8(j) << QPointF{ qreal(strb.time), qreal(strb.ampl) };
        }
    }
}

void Server::strobesReceived(QDataStream& in)
{
    in >> input.time;
    while (!in.atEnd()) {
        quint8 i;
        in >> i;
        if (i > 8)
            qDebug() << i;
        for (auto& strobe : input.limits[i]) {
            in >> strobe;
            auto& test = strobe;
        }
    }
}

void Server::sendCallBack(QDataStream& in)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << qint64(0);

    
    if (command == "o") {
        if (numChannel == 255) {
            return;
        }
        out << QString("o");
        sendOsc(out);
    }
    else 
    if (command == "a") {
        if (numChannel == 255) {
            return;
        }
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
    udpSocket->writeDatagram(data, QHostAddress("192.168.1.164"), port);
    udpSocket->waitForBytesWritten();
}


void Server::disconnect()
{
    QObject::disconnect(udpSocket, &QUdpSocket::readyRead, this, &Server::callBackDatagram);
    if (udpSocket) 
        delete udpSocket;
    Server::close();
}
