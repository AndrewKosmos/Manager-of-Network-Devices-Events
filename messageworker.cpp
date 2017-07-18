#include "messageworker.h"

MessageWorker::MessageWorker()
{
    messageTypesList << "Emergency" << "Alert" << "Critical" << "Error" << "Warning" << "Notice" << "Informational" <<"Debug";
    messageColorsList << "#9AC0CD" << "#CD3333" << "#8B0000" << "red" << "yellow" << "lightGray" << "white" << "white";
}


void MessageWorker::ParseSyslogMess(QString message, QString IP)
{
    QString messageCopy = message;
    QStringList messageInfo;

    //get part after '>'
    QStringList neededPartMessageForParseList = messageCopy.split('>');
    QString neededPartMessageForParse = neededPartMessageForParseList.at(1);

    //get message text
    QRegExp twoPartsOfMessageInfoRx(".*:");
    QStringList twoPartsOfMessageInfo = neededPartMessageForParse.split(twoPartsOfMessageInfoRx,QString::SkipEmptyParts);

    //capture datetime
    QRegExp rxDateTime("(.*\\d+:\\d+:\\d+)");
    int pos = 0;
    QStringList DateTimeList;

    while((pos = rxDateTime.indexIn(neededPartMessageForParse,pos)) != -1)
    {
        DateTimeList << rxDateTime.cap(1);
        pos += rxDateTime.matchedLength();
    }

    //Convert DateTime to right string format (for futher converting into QDateTime and <)
    QString DateTimeStringInitial = DateTimeList.at(0);
    QStringList partsOfDateTime = DateTimeStringInitial.split(" ",QString::SkipEmptyParts);

    QString DTfinish = partsOfDateTime.at(0) + " " + partsOfDateTime.at(1) + " " + partsOfDateTime.at(2) +
            + " " + partsOfDateTime.at(3);

    //remove message text
    int index2 = neededPartMessageForParse.lastIndexOf(":");
    neededPartMessageForParse.chop(neededPartMessageForParse.length() - index2);
    //remove datetime
    neededPartMessageForParse.remove(rxDateTime);
    neededPartMessageForParse.trimmed();

    QStringList facAndInfoList = neededPartMessageForParse.split(' ',QString::SkipEmptyParts);
    QString Facility = facAndInfoList.at(0);
    QString InfoForSplit = facAndInfoList.at(1);
    QStringList InfoList = InfoForSplit.split('/',QString::SkipEmptyParts);
    QString Priority = InfoList.at(1);
    QString Tag = InfoList.at(2);

    //Parse IP address
    QRegExp rxIP("([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})");
    pos = 0;
    QString senderIP;

    while((pos = rxIP.indexIn(IP,pos)) != -1)
    {
        senderIP = rxIP.cap(1);
        pos += rxIP.matchedLength();
    }

    int Priority_pos = Priority.toInt();

    messageInfo.append(DTfinish);
    messageInfo.append(senderIP);
    messageInfo.append(Facility);
    messageInfo.append(messageTypesList.at(Priority_pos));
    messageInfo.append(Tag);
    messageInfo.append(twoPartsOfMessageInfo);


    //set reaction on messages
    switch (Priority_pos) {
    case 1:
        qDebug() << "System got Alert message";
        break;
    case 3:
        qDebug() << "System got Error message!";
        break;
    case 4:
        qDebug() << "System got Warning message";
        break;
    default:
        break;
    }

    emit SyslogMessageParsed(messageInfo);
}
