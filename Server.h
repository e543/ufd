#pragma once
#include <QWidget>
#include <QThread>


QT_BEGIN_NAMESPACE
#define NUM_RESULT_STRBS    5
#define NUM_USS             2
#define NUM_TACTS            4

struct settings {
    qreal time = 0;
    QVector<QVector<QPair<qreal, QPointF>>> limits;
    settings() {
        reset();
    }
    void reset() {
        limits.resize(8);
        for (auto& channel : limits) {
            channel.resize(5);
            for (int i = 0; i < 5; ++i) {
                auto pair = QPair<qreal, QPointF>{ 0., { 0. , 0. } };
                channel[i] = pair;
            }
        }
    }
};

struct amp_strob_struct_t
{
    quint16 time;
    quint8 ampl;
};

struct amp_us_struct_t
{
    amp_strob_struct_t ampl[NUM_RESULT_STRBS];
};

struct amp_tact_struct_t
{
    amp_us_struct_t ampl_us[NUM_USS];
};

struct amp_struct_t
{
    amp_tact_struct_t ampl_tact[NUM_TACTS];
};


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
    void dataStrobe(QDataStream& out);
    void strobesReceived(QDataStream& in);
    void updateOsc();
    void initOsc();
private:
    QLabel* statusLabel = nullptr;
    QPushButton* startButton = nullptr;
    QUdpSocket* udpSocket = nullptr;
    quint8 osc[8][256];
    quint8 oscInit[8][256];
    QString command;
    quint16 port;
    settings input;
    quint8 numChannel;
    amp_struct_t data;
    QTimer* timer;
    static constexpr qreal width = 256;
    static constexpr qreal delta = width / 256;
private:
    void sendCallBack(QDataStream& in);
    void sendOsc(QDataStream& out);
    void disconnect();
};

