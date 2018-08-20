#include "sesioncontroller.h"
#include <QDebug>
#include <QUuid>
#include <QObject>

SesionController::SesionController()
{
    m_timer->start(1000); // 10 sec
    QObject::connect(m_timer,SIGNAL(timeout()),this,SLOT(checkConnection()));
}

SesionController::~SesionController()
{
    delete m_timer;
}

QString SesionController::createSession(QString sId)
{
    QString sToken = generateToken();
    Session tmpSession;
    tmpSession.setID(sId);
    tmpSession.setToken(sToken);
    mapSessions.insert(sToken,tmpSession);
    return sToken;
}

QString SesionController::generateToken()
{
    auto oGuid = QUuid::createUuid();
    QString sToken = oGuid.toString();

    if(checkToken(sToken))
    {
       return generateToken();
    }
    return sToken;
}

bool SesionController::checkToken(QString sToken)
{
    return mapSessions.contains(sToken);
}

bool SesionController::deleteToken(QString sToken)
{
    mapSessions.remove(sToken);
    return ! checkToken(sToken);
}

bool SesionController::userLoggedIn(QString sUserId)
{
    QList<QString> lstToken = mapSessions.keys();

    for(QString tmpToken:lstToken)
    {
        if(mapSessions[tmpToken].getID() == sUserId)
        {
            return true;
        }
    }
    return false;
}

QString SesionController::getToken(QString sUserID)
{
     QList<QString> lstToken = mapSessions.keys();
     for(QString tmpToken:lstToken)
     {
         if(mapSessions[tmpToken].getID() == sUserID)
         {
                return tmpToken;
         }
     }

     return QString();

}

void SesionController::dataChanged(QString sToken,int sDate)
{
    mapSessions[sToken].setDate(sDate);
}

void SesionController::checkConnection()
{
    for(QString sKey:mapSessions.keys())
    {
        int iTime = QTime::currentTime().second();
        int iTokenTime = mapSessions[sKey].getDate();

       if(iTime - iTokenTime > 20)
       {
           deleteToken(sKey);
       }
    }
}
