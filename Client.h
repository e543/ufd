#pragma once
#include <QUdpSocket>
#include <QDebug>
#include "Server.h"


struct Result
{
    QString command;
    QVector<quint8> osc;
    amp_struct_t data;
    Result() {
        command = "";
        osc.resize(256);

        for (int i = 0; i < 4; ++i) {
            auto& ampl_tact = data.ampl_tact[i];
            for (int j = 0; j < 2; ++j) {
                auto& ampl_us = ampl_tact.ampl_us[j];
                for (int k = 0; k < 5; ++k) {
                    auto& ampl = ampl_us.ampl[k];
                    ampl.ampl = 0;
                    ampl.time = 0;
                }
            }
        }
    }
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

