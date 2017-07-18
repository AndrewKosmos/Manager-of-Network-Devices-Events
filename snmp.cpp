#include "snmp.h"
#include <QString>
#include <QDebug>
#include <QProcess>
#include <sys/time.h>



Snmp::Snmp()
{

}

void Snmp::setIP (QString ip)
{
    _ip = ip;
}

void Snmp::setCommunity (QString community)
{
    _community = community;
}

QStringList Snmp::getRawData(QString ip, QString community, QString oid)
{
    //qDebug () << "Snmp_getData";
    QString program = "snmpwalk";
    QStringList arguments;
    arguments << "-v" << "2c" << "-r" << "1" << "-c"  << community << ip << oid;

    QProcess *myProcess = new QProcess();

    myProcess->start(program, arguments);
    myProcess->waitForFinished();

    QByteArray return_snmp (myProcess->readAllStandardOutput());

    QStringList list = QString(return_snmp).split('\n', QString::SkipEmptyParts);
    return list;
}

QString Snmp::getMetaInfo(QString ip, QString community, QString oid)
{
    QString program = "snmpget";
    QStringList arguments;
    arguments << "-v" << "2c" << "-r" << "1" << "-c"  << community << ip << oid;

    QProcess *myProcess = new QProcess();

    myProcess->start(program, arguments);
    myProcess->waitForFinished();

    QByteArray return_snmp (myProcess->readAllStandardOutput());

    QString res_snmp = QString(return_snmp);
    QStringList res_snmp_typeValue = res_snmp.split("=",QString::SkipEmptyParts);
    QStringList res_snmp_value = res_snmp_typeValue.at(1).split(':',QString::SkipEmptyParts);
    QStringList res_final = res_snmp_value.at(1).split("\r\n",QString::SkipEmptyParts);
    QString res = res_final.at(0);
    return res;
}

QStringList Snmp::getSTP_MAC ()
{
    qDebug () << "getSTP_MAC";
    QString oid = "1.3.6.1.4.1.2011.5.25.42.4.1.20.1.7";
    QStringList list = getRawData (_ip, _community, oid);
    QStringList listSTP_MAC;
    foreach (QString qstLine, list)
    {
        QStringList sort_list = qstLine.split(" ", QString::SkipEmptyParts);
        if (sort_list.count() < 11)
            continue;
        QString mac;
        for (int i = 5; i < 11; ++i)
        {
            mac.append(sort_list.at(i));
            //if (!(i==10))
            //mac.append(":");
        }
        listSTP_MAC.append(mac);
    }
    //qDebug () << listSTP_MAC;
    return listSTP_MAC;
}

QString Snmp::getName ()
{
    qDebug () << "getName" << _ip;
    QString oid = "1.3.6.1.2.1.1.5.0";
    QStringList list = getRawData (_ip, _community, oid);
    QString name;

    if (list.isEmpty())
        return QString("");
    foreach (QString qstLine, list)
    {
        QStringList sort_list = qstLine.split(" ", QString::SkipEmptyParts);
        if (sort_list.count() < 4)
            continue;
        name = sort_list.at(3);
        //qDebug () << "Name: " << name;
    }
    nameDevice = name;
    return name;
}


QString Snmp::getOwnMAC ()
{
    qDebug () << "getOwnMAC";
    QString oid = "1.3.6.1.4.1.2011.5.25.41.1.3.7.0";
    QStringList list = getRawData (_ip, _community, oid);
    QString mac;
    foreach (QString qstLine, list)
    {
        QStringList sort_list = qstLine.split(" ", QString::SkipEmptyParts);
        if (sort_list.count() < 4)
            continue;

        for (int i = 3; i < 9; ++i)
        {
            mac.append(sort_list.at(i));
        }
        //qDebug () << "OwnMAC: " << mac;
    }
    return mac;
}


int  Snmp::getInterfaceStatus (int number)
{
    //qDebug () << "getInterfaceStatus";
    QString oid = "iso.3.6.1.2.1.2.2.1.7.%1";
    QStringList list = getRawData (_ip, _community, oid.arg(number));
    QString mac;
    bool *ok = 0;
    int activ;

    foreach (QString qstLine, list)
    {
        QStringList sort_list = qstLine.split(" ", QString::SkipEmptyParts);
        if (sort_list.count() < 4)
            continue;

        QString qstInterfaceName = sort_list.at(3);
        //qDebug() << "getInterfaceStatus id:" << sort_list.at(3);
        activ = sort_list.at(3).toInt(ok, 10);
    }
    return activ;
}

QStringList Snmp::setRawData (QString ip, QString community, QList <OID> lstOids)
{
    QString program = "snmpset";
    QStringList arguments;
    arguments << "-v" << "2c" << "-c" << community << ip;
    foreach (OID oid, lstOids)
    {
        arguments << oid.oid << oid.type << oid.value;
    }
    qDebug () << "setRawData" <<  arguments;
    QProcess *myProcess = new QProcess();
    myProcess->start(program, arguments);
    myProcess->waitForFinished();
    QByteArray return_snmp (myProcess->readAllStandardOutput());
    QStringList list = QString(return_snmp).split('\n', QString::SkipEmptyParts);
    return list;
}

int Snmp::setSaveConfig(void)
{
    QList <OID> lstOID;
    OID typeSaving;
    OID rowStatus;
    rowStatus.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.9.2";
    rowStatus.type = "i";
    rowStatus.value = "6";
    lstOID << rowStatus;

    setRawData (_ip, _community, lstOID);

    lstOID.clear();
    typeSaving.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.2.2";
    typeSaving.type = "i";
    typeSaving.value = "1";
    rowStatus.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.9.2";
    rowStatus.type = "i";
    rowStatus.value = "4";
    lstOID << typeSaving << rowStatus;

    setRawData (_ip, _community, lstOID);
}

int Snmp::setSaveConfigToNet(ModeSaving mode)
{
    QList <OID> lstOID;
    OID typeSaving;
    OID protocol;
    OID ServerAddress;
    OID fileName;
    OID SftpLogin;
    OID SftpPassword;
    OID rowStatus;
    rowStatus.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.9.1";
    rowStatus.type = "i";
    rowStatus.value = "6";
    lstOID << rowStatus;

    setRawData (_ip, _community, lstOID);

    /*
SNMPv2-SMI::enterprises.2011.6.10.1.2.4.1.2.4 = INTEGER: 3
SNMPv2-SMI::enterprises.2011.6.10.1.2.4.1.3.4 = INTEGER: 3
SNMPv2-SMI::enterprises.2011.6.10.1.2.4.1.4.4 = STRING: "vr.cfg"
SNMPv2-SMI::enterprises.2011.6.10.1.2.4.1.5.4 = IpAddress: 10.0.0.102
SNMPv2-SMI::enterprises.2011.6.10.1.2.4.1.6.4 = STRING: "ssh"
SNMPv2-SMI::enterprises.2011.6.10.1.2.4.1.7.4 = STRING: "1q2w3e4r"
SNMPv2-SMI::enterprises.2011.6.10.1.2.4.1.9.4 = INTEGER: 4
*/

    lstOID.clear();
    typeSaving.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.2.1";
    typeSaving.type = "i";
    typeSaving.value = "3";
    protocol.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.3.1";
    protocol.type = "i";
    protocol.value = QString::number(mode, 10);


    fileName.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.4.1";
    fileName.type = "s";
    fileName.value = "backup/" + _ip + ".cfg";
    ServerAddress.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.5.1";
    ServerAddress.type = "a";
    ServerAddress.value = "10.0.0.2";
    SftpLogin.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.6.1";
    SftpLogin.type = "s";
    SftpLogin.value = "ssh";
    SftpPassword.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.7.1";
    SftpPassword.type = "s";
    SftpPassword.value = "1q2w3e4r";
    rowStatus.oid = "1.3.6.1.4.1.2011.6.10.1.2.4.1.9.1";
    rowStatus.type = "i";
    rowStatus.value = "4";
    switch (mode)
    {
    case Snmp::ftp:
    case Snmp::sftp:
        lstOID << typeSaving << protocol << fileName << ServerAddress << SftpLogin << SftpPassword << rowStatus;
        break;
    case Snmp::tftp:
        lstOID << typeSaving << protocol << fileName << ServerAddress << rowStatus;
        break;
    }
    setRawData (_ip, _community, lstOID);
}

int Snmp::getSyslogStatus ()
{
    QString oidGetSyslogStatus ("1.3.6.1.4.1.2011.5.25.212.1.1");
    QStringList returnList = getRawData (_ip, _community, oidGetSyslogStatus);
    qDebug() << "Snmp::getSyslogStatus" << isValueValid(returnList, 1, 3);
    return isValueValid(returnList, 1, 3);
}

int Snmp::setSyslogStatus (bool status)
{
    QString oidGetSyslogStatus;
    oidGetSyslogStatus = "1.3.6.1.4.1.2011.5.25.212.1.1.0";

    OID oid;
    oid.oid = oidGetSyslogStatus;
    oid.type = QString ("i");
    if (status)
        oid.value = QString ("1");
    else
        oid.value = QString ("2");

    QList <OID> lstOids;
    lstOids.append(oid);
    QStringList returnList = setRawData (_ip, _community, lstOids);
    qDebug() << "Snmp::setSyslogStatus" << oidGetSyslogStatus;
    return 1;
}

QStringList Snmp::getSyslogServers ()
{
    qDebug() << "Snmp::getSyslogServers";
    QStringList lstSyslogServers;
    QString oidGetSyslogServers  ("1.3.6.1.4.1.2011.5.25.212.1.2.2.1.2");
    QStringList returnList = getRawData (_ip, _community, oidGetSyslogServers);
    foreach (QString qstLine, returnList)
    {

        QStringList sort_list_item = qstLine.split(" ", QString::SkipEmptyParts);
        if (sort_list_item.count() > 4)
            continue;
        lstSyslogServers.append(QString(sort_list_item.at(3)).remove(QChar('"'), Qt::CaseInsensitive));
        qDebug() << "Snmp::getSyslogServers" << sort_list_item.at(3);
    }

    return lstSyslogServers;


    //return isValueValid(returnList, 1, 3);
}

int Snmp::setSyslogAddServer(QString ipLoghost)
{
    qDebug() << "Snmp::setSyslogConfig";
    //    QString oidGetSyslogTable ("1.3.6.1.4.1.2011.5.25.212.1.2.2.1.2");

    //    if (!getSyslogStatus())
    //        return 0;
    //    QStringList returnList;
    //    QStringList lstLoghost;
    //    returnList = getRawData (_ip, _community, oidGetSyslogTable);
    //    lstLoghost = valueParseToList(returnList, 3);

    //    if (lstLoghost.count() >= 8)
    //        return 0;
    OID oidSetLoghost =  {"1.3.6.1.4.1.2011.5.25.212.1.2.2.1.7.1", "i", "4"};
    //QString ipLoghost ("1.4.3.5");
    QString vpnInstance ("_public_");
    oidSetLoghost.oid.append(toDottedString(ipLoghost));
    oidSetLoghost.oid.append(toDottedString(vpnInstance));

    QList<OID> lstOID;
    lstOID.append(oidSetLoghost);
    setRawData(_ip, _community, lstOID);
    qDebug () << oidSetLoghost.oid ;
}

int Snmp::setSyslogDelServer(QString ipLoghost)
{
    qDebug() << "Snmp::setSyslogConfig";

    OID oidSetLoghost =  {"1.3.6.1.4.1.2011.5.25.212.1.2.2.1.7.1", "i", "6"};
    //QString ipLoghost ("1.4.3.5");
    QString vpnInstance ("_public_");
    oidSetLoghost.oid.append(toDottedString(ipLoghost));
    oidSetLoghost.oid.append(toDottedString(vpnInstance));

    QList<OID> lstOID;
    lstOID.append(oidSetLoghost);
    setRawData(_ip, _community, lstOID);
    qDebug () << oidSetLoghost.oid ;
}

QStringList Snmp::getMyIPaddress ()
{
    qDebug() << "Snmp::getMyHostIPaddress";
    QString program = "ifconfig";
    QStringList arguments;

    QProcess *myProcess = new QProcess();

    myProcess->start(program, arguments);
    myProcess->waitForFinished();

    QByteArray return_snmp (myProcess->readAllStandardOutput());
    QStringList list = QString(return_snmp).split('\n', QString::SkipEmptyParts);

    QStringList lstIP;
    foreach (QString qstLine, list)
    {
        //if (qstLine.contains("eth"))
        {
            if (qstLine.contains("inet addr:"))
            {
                qstLine = qstLine.mid(qstLine.indexOf(':') + 1);
                QString myIP = qstLine.remove(qstLine.indexOf(' '), qstLine.length() - qstLine.indexOf(' '));
                //qDebug() << "My OWN IP-address: " <<  myIP;
                if (myIP != "127.0.0.1")
                    lstIP.append(myIP);
            }
        }
    }
    return lstIP;
}



bool Snmp::isValueValid(QStringList lstStr, int value, int pos = 3){
    foreach (QString qstLine, lstStr)
    {
        QStringList sort_list = qstLine.split("\n", QString::SkipEmptyParts);
        foreach (QString qstLine, sort_list)
        {
            QStringList sort_list_item = qstLine.split(" ", QString::SkipEmptyParts);
            QString qstOID = sort_list_item.at(pos);
            if (qstOID.toInt(NULL, 10) == value)
                return true;
        }
        return false;
    }
}

QString Snmp::toDottedString(QString str)
{
    QString dottedStr;
    dottedStr.append(".");
    dottedStr.append (QString::number(str.count(), 10));

    for (int i=0; i < str.count(); ++i)
    {
        dottedStr.append(".");
        dottedStr.append (QString::number(QString(str.at(i)).toLocal8Bit().toHex().toInt(NULL, 16), 10));
    }
    return dottedStr;
}

QStringList Snmp::valueParseToList(QStringList parsingLst, int value)
{
    QStringList tmpList;
    foreach (QString qstLine, parsingLst)
    {
        QStringList sort_list = qstLine.split("\n", QString::SkipEmptyParts);
        foreach (QString qstLine, sort_list)
        {
            QStringList sort_list_item = qstLine.split(" ", QString::SkipEmptyParts);
            QString qstOID = sort_list_item.at(value);
            tmpList.append(qstOID);
        }
    }
    return tmpList;
}

void Snmp::run ()
{
    QThread::msleep(::rand()%11);
    if (!(getName().isEmpty()))
    {
        qDebug () << "isNotEmpty" << _ip;
        emit sigGetName (nameDevice, _ip, QString::number(getSyslogStatus(), 10));
    }

}
