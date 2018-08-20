#ifndef RESPONSE_H
#define RESPONSE_H
#include <QJsonDocument>
#include <QString>

struct Response
{
    bool oError;
    QString sErrorText;
    QString sCommand;
    QJsonDocument oData;
    int iMsgSize;
};
//struct Request
//{
//    QString sCommand;
//    QJsonDocument oData;
//    QString sToken;

//};


#endif // RESPONSE_H
