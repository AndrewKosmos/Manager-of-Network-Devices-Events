#ifndef SNMP_H
#define SNMP_H
#include <QByteArray>
#include <QStringList>
#include <QThread>
#include <QMutex>

class Snmp : public QThread
{
    Q_OBJECT
public:
    Snmp();

    QString _ip;
    QString _community;
    QString _oid;

    QStringList rtnLstData;
    QString nameDevice;
    int idDevice;

    enum ModeSaving {ftp, tftp, sftp};
    typedef struct OID
    {
        QString oid;
        QString type;
        QString value;
    };

    QMutex mutex;
    void setIP (QString);
    void setCommunity (QString);

    QStringList getRawData(QString ip, QString community, QString oid);
    QStringList getSTP_MAC ();
    QString getName ();
    QString getOwnMAC ();


    int getInterfaceStatus (int number);
    QString getInterfaceName ();
    QString getInterfaceNumber ();
    QString getInterfaceVlan ();
    QString getInterfaceMode ();
    bool getInterfaceActive ();



    QStringList setRawData (QString ip, QString community, QList<OID>);
    int getSyslogStatus (void);
    int setSyslogStatus (bool);
    QStringList  getSyslogServers ();
    int setSaveConfig (void);
    int setSaveConfigToNet (ModeSaving);
    int setSyslogAddServer(QString);
    int setSyslogDelServer(QString);

    QStringList getMyIPaddress ();
    int insertSwitch (QString name, QString ip, QString mac);
    void insertSTP_MAC (QStringList mac);

    bool isValueValid(QStringList, int, int);
    QString toDottedString(QString str);
    QStringList valueParseToList(QStringList, int);

private:
    virtual void run();

signals:
    void sigGetName (QString, QString, QString);
};

#endif // SNMP_H
