#include "Application.h"
#include <iostream>
#include <QtNetwork>

int main(int argc, char* argv[])
{
    Application app(argc, argv);
    auto& w = app.getMainWindow();
    w.show();

    app.setAttribute(Qt::AA_DisableWindowContextHelpButton);

    QList<QHostAddress> broadcastAddresses;
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
    }

    try
    {
        return app.exec();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "something failed" << std::endl;
    }
}
