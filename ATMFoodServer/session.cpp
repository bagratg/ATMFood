
#include "session.h"

Session::Session()
{

}

void Session::setID(QString sId)
{
    this->m_ID = sId;
}

void Session::setToken(QString sToken)
{
    this->m_Token = sToken;
}

void Session::setDate(int sDate)
{
    this->m_Date = sDate;
}

QString Session::getID()
{
    return m_ID;
}

QString Session::getToken()
{
    return m_Token;
}

int Session::getDate()
{
    return m_Date;
}
