#pragma once
#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QUdpSocket;
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget* parent = nullptr);

private slots:
    void startBroadcasting();
    void broadcastDatagram();

private:
    QLabel* statusLabel = nullptr;
    QPushButton* startButton = nullptr;
    QUdpSocket* udpSocket = nullptr;
    QTimer timer;
    quint8 osc[256];
    int messageNo = 1;
private:
    void disconnect();
};

