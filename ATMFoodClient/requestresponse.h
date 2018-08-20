#ifndef RESPONSE_H
#define RESPONSE_H
#include <QJsonDocument>
#include <QString>
#include <QPixmap>

struct Response
{
    bool oError;
    QJsonDocument oData;
};
struct Request
{
    QString sCommand;
    QJsonDocument oData;
    QString sToken;
    QPixmap image;

};


#endif // RESPONSE_H
