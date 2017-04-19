#ifndef NETWORKING_H
#define NETWORKING_H
#include <QUdpSocket>

class Networking : public QObject
{
    Q_OBJECT
public:
    Networking(int Port);
private:
    QUdpSocket* udpSocket;
private slots:
    void ReadPendingDatagram();
signals:
    void DatagramReaded(QString datagram, QString IP);
};

#endif // NETWORKING_H
