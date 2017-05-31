#ifndef NETWORKLINEEDIT_H
#define NETWORKLINEEDIT_H
#include <QLineEdit>

class NetworkLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    NetworkLineEdit();

    bool isCorrect ();
    int getRange ();
    int setIP (int);
    QString getIP (void);
    int getIntIP (void);
    QStringList getRangeIP (NetworkLineEdit*);
    QString converToStringIp(int ip);
private:
    unsigned int ip;
    int prefix;
    int convertOctet (char, int);
    void updateText (QString);
    void updateTextMask(QString);
    int parsingIP ();
    int getPrefix ();
    QString prevText;
    bool correctIP;
    bool readyIP;
        bool mask;
    int iRange;

    enum mode {ipaddress, netmask, gateway};
    union unAddress
    {
        int ip;
        unsigned char octet[4];
    };
private slots:
    void getEditor(QString text);
    void finishedEdit ();
    void backgroundCorrect (int i);
    void backgroundIncorrect (int i);
};

#endif // NETWORKLINEEDIT_H
