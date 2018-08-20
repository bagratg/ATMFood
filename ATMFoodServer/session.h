#ifndef SESSION_H
#define SESSION_H

#include <QString>



class Session
{
public:
    Session();
    void setID(QString sId);
    void setToken(QString sToken);
    void setDate(int sDate);
    int getDate();
    QString getID();
    QString getToken();


private:
    QString m_ID;
    QString m_Token;
    int m_Date;

};

#endif // SESSION_H
