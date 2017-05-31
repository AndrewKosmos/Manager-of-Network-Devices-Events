#include "networklineedit.h"
#include <QDebug>

NetworkLineEdit::NetworkLineEdit()
{
    setMaxLength(18);
    correctIP = false;
    readyIP = false;
    mask = false;
    iRange = 0;
    connect (this, SIGNAL (textEdited(QString)), this, SLOT(getEditor(QString)));
    connect (this, SIGNAL (editingFinished()), this, SLOT(finishedEdit()));
}

void NetworkLineEdit::getEditor(QString text)
{
    QString modText;
    QString modMask;
    int countDot = 0;
    int countDegit = 0;
    int countMaskDegit = 0;
    prefix = 0;
    int octet = 0;
    if (1)
        backgroundIncorrect(28);

    //обработка удаления
    if (prevText.length() > text.length())
    {
        if (prevText.at(prevText.length() - 1 )  == '.')
        {
            updateText(text);
            prevText = text;
            countDot--;
            countDegit = 3;
            return;
        }
        if ((text.indexOf('/') > 0 ) && mask)
        {
            mask = false;
            return;
        }
    }

    prevText = text;
    QString strIP, strPrefix;
    if (1)
    {
        strIP = text.left(text.indexOf('/'));

        if (text.indexOf('/') > 0)
        {
            strPrefix = text.mid(text.indexOf('/') + 1);
        }
        else
            strPrefix = "";

    }
    foreach (char c, strIP.toUtf8()) {
        if ((c >= '0' && c <= '9') || c == '.')
        {
            if (c == '0' && octet == 0) //
            {
                if (countDot == 0)
                {
                    continue;
                }

                if (countDot < 3)
                {
                    modText.append('0');
                    modText.append('.');
                    countDot++;
                    countDegit = 0;
                    octet = 0;
                    continue;
                }
            }

            if (c == '.')
            {
                if (countDot == 3)
                {
                    if (countDegit == 0)
                    {
                        backgroundIncorrect(59);
                    }
                }
                if (countDot > 3)
                {
                    continue;
                }
                else
                {
                    if (countDegit == 0)
                    {
                        backgroundIncorrect(70);
                        continue;
                    }
                    else
                    {
                        countDegit = 0;
                        if (countDot < 3)
                        {
                            modText.append(c);
                            countDot++;
                            octet = 0;
                        }
                    }
                }
            }


            if (countDegit == 3)//== 3
            {
                if (countDot == 3)
                {
                    backgroundCorrect(90);
                    continue;
                }
                else
                {
                    modText.append('.');
                    countDot++;
                    countDegit = 0;
                    octet = 0;
                    backgroundIncorrect(99);
                }
            }

            if (c >= '0' && c <= '9')
            {
                if (countDot == 3 && octet >= 0)
                {
                    backgroundCorrect(108);
                }

                if (convertOctet(c, octet) >= 0)
                {
                    if (octet == 0 && c == '0')
                    {
                        if (countDegit == 0)
                        {
                            modText.append(c);
                            countDegit++;
                        }
                        continue;
                    }

                    if (octet == 0 && countDegit == 1)
                        continue;

                    octet = convertOctet(c, octet);
                    modText.append(c);

                    if ((countDegit == 3) && countDot < 3)
                    {
                        modText.append('.');
                        countDot++;
                        countDegit = 0;
                        octet = 0;
                    }
                }
                countDegit++;
            }
        }
    }
    updateText(modText);
    prevText = modText;


    if (text.indexOf('/') > 0 && (countDot == 3) && (countDegit > 0) && !mask)
    {
        mask = true;
        modMask.append('/');
    }
    else if (mask && text.indexOf('/') > 0)
    {
        modMask.append('/');

    }
    if (text.indexOf('/') < 0)
    {
        mask = false;
    }

    foreach (char c, strPrefix.toUtf8()) {
        if ((c >= '0' && c <= '9'))
        {
            //qDebug () << "Prefix parser ";

            if (convertOctet(c, prefix) >= 0 && convertOctet(c, prefix) <= 32)
            {

                if (countMaskDegit > 0 && prefix == 0)
                    continue;
                                prefix = convertOctet(c, prefix);
                modMask.append(c);
                //qDebug () << "Prefix: " << prefix;
                countMaskDegit++;
            }
            else
                continue;
        }
    }
    updateTextMask(modMask);
    prevText.append(modMask);
    //qDebug () << modMask;


}

void NetworkLineEdit::updateText(QString text)
{
    setText(text);
    if (parsingIP() == 0)
        backgroundIncorrect(133);
}

void NetworkLineEdit::updateTextMask(QString text)
{
    setText(this->text() + text);
}

void NetworkLineEdit::backgroundCorrect(int i)
{
    //qDebug () << "NetworkLineEdit::backgroundCorrect" << i;
    setStyleSheet("QLineEdit { background-color: white }");
    correctIP = true;
}
void NetworkLineEdit::backgroundIncorrect(int i)
{
    //qDebug () << "NetworkLineEdit::backgroundIncorrect" << i;
    setStyleSheet("QLineEdit { background-color: yellow }");
    correctIP = false;
}

void NetworkLineEdit::finishedEdit ()
{
    parsingIP();
}

int NetworkLineEdit::parsingIP ()
{
    ip = 0;
    QString parseIP = this->text();

    foreach (QString oct, this->text().split('.'))
    {
        //qDebug () << "OCT:" << oct;
        ip *= 256;
        ip += oct.toInt(NULL, 10);
    }
    unAddress unIP;
    unIP.ip = ip;
    if (unIP.octet[3] == 0)
        return -1;
    //    qDebug () << "IP-address:";
    //    qDebug () << unIP.octet[3]
    //            << unIP.octet[2]
    //            << unIP.octet[1]
    //            << unIP.octet[0];
    getPrefix();
    return ip;
}

int NetworkLineEdit::getPrefix()
{
    unsigned char ch = 128;
    //qDebug () << "___: " << QString::number((ip & ~(ch - 1)), 2);
    //qDebug () << "___: " << QString::number((ch - 1), 2);
    return 0;
}

int NetworkLineEdit::convertOctet(char c, int i)
{
    if ((i * 10 + ::atoi(&c)) > 255)
    {
        return -1;
    }
    else
    {
        i *= 10;
        i += ::atoi(&c);
        return i;
    }
}

QString NetworkLineEdit::getIP()
{
    if (correctIP)
    {
        unAddress unIP;
        unIP.ip = parsingIP();
        return converToStringIp(unIP.ip);
    }
}

int NetworkLineEdit::getIntIP()
{
    return ip;
}


QString NetworkLineEdit::converToStringIp(int ip)
{
    QString strIP;
    if (correctIP)
    {
        unAddress unIP;
        unIP.ip = ip;
        strIP.append(QString::number(unIP.octet[3], 10));
        strIP.append('.');
        strIP.append(QString::number(unIP.octet[2], 10));
        strIP.append('.');
        strIP.append(QString::number(unIP.octet[1], 10));
        strIP.append('.');
        strIP.append(QString::number(unIP.octet[0], 10));
    }
    return strIP;
}

QStringList NetworkLineEdit::getRangeIP (NetworkLineEdit* endIp)
{
    QStringList lstRangeIP;
    if (this->getIntIP() >= endIp->getIntIP())
        lstRangeIP.append(getIP());
    else
    {
        int range = endIp->getIntIP() - this->getIntIP();
        for (int i = this->getIntIP(); i <= endIp->getIntIP(); ++i)
        {
            lstRangeIP.append(NetworkLineEdit::converToStringIp(i));
        }
        iRange = range;
    }
    return lstRangeIP;
}

bool NetworkLineEdit::isCorrect()
{
    if (correctIP)
        return true;
    else
        return false;
}

int NetworkLineEdit::getRange()
{

    return iRange;
}
