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
    void startListening();
    void callBackDatagram();
    void dataOsc(QDataStream& out);
    void dataStrobe(QDataStream& out);
    void strobesChanged(QDataStream& in);
private:
    QLabel* statusLabel = nullptr;
    QPushButton* startButton = nullptr;
    QUdpSocket* udpSocket = nullptr;
    QTimer timer;
    quint8 osc[256];
    QString command;
    quint16 port;
    int messageNo = 1;
private:
    void sendCallBack();
    void disconnect();
};

