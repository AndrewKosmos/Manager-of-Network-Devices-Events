#include "networking.h"
#include <QMessageBox>

Networking::Networking(int Port)
{
    udpSocket = new QUdpSocket(this);
    if(udpSocket->bind(Port))
    {
        connect(udpSocket,SIGNAL(readyRead()),this,SLOT(ReadPendingDatagram()));
        /*QMessageBox msg;
        msg.setText("Bind successfull " + QString::number(Port) + " " + udpSocket->localAddress().toString());
        msg.exec();*/
    }
    else {
        QMessageBox msg;
        msg.setText("Error! Port" + QString::number(Port) + " is in use");
        msg.exec();
    }
}

void Networking::ReadPendingDatagram()
{
    QByteArray datagram;

    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);

        QString m(datagram);

        emit DatagramReaded(m,sender.toString());
    }
}
