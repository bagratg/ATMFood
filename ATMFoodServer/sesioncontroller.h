#ifndef SESIONCONTROLLER_H
#define SESIONCONTROLLER_H

#include <QString>
#include <QVariant>
#include <QMap>
#include <QStringlist>
#include <QTime>
#include <QTimer>
#include "session.h"

class SesionController:public QObject
{
    Q_OBJECT

public:

    SesionController();
    ~SesionController();

public:

    QString createSession(QString sId);
    bool checkToken(QString sToken);
    bool deleteToken(QString sToken);
    bool userLoggedIn(QString sUsername);
    QString getToken(QString sUserID);
    void dataChanged(QString sToken, int sDate);
    QMap<QString, Session> getMap();


public slots:

   void checkConnection();

private:
    QString generateToken();

private:
    QMap<QString, Session> mapSessions;
    QTimer * m_timer = new QTimer();

};

#endif // SESIONCONTROLLER_H
