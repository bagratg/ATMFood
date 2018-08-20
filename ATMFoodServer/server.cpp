#include "server.h"


Server::Server(){}
Server::~Server(){}

bool Server::checkLogin(QString username, QString password)
{
    int iterator = 0;
    QSqlQuery q;
    q.prepare("select * from  user where username= :username and password= :password");
    q.bindValue(":username",username);
    q.bindValue(":password",password);
    if(q.exec())
    {
        while(q.next())
        {
           iterator++;
        }
        if(iterator)
        {
            qDebug() << "Okay login: " + username;
            return true;
        }
        else
        {
           qDebug() << "Incorrect username or password";
           return false;
        }
    }
    return false;
}

void Server::WriteServer(QString x, QStringList StringsKey, QStringList StringsValue)
{
    QJsonObject Data;
    for(int i = 0; i< StringsValue.size();i++)
    {
        Data[StringsKey[i]] =  StringsValue[i];
    }
    Data["x"] = x;
    QJsonDocument oDoc(Data);
    QByteArray oArray = oDoc.toJson();
    mTcpSocket->write(oArray);
}

void Server::startServer()
{
    if(this->listen(QHostAddress::Any,4444))
    {
        qDebug() << "Liss";
    }
    else
    {
        qDebug() << "4liss";
    }

}

void Server::incomingConnection(int socketDis)
{
      mTcpSocket = new QTcpSocket(this);
      mTcpSocket->setSocketDescriptor(socketDis);

      connect(mTcpSocket,SIGNAL(readyRead()),this,SLOT(sockReady()));
      connect(mTcpSocket,SIGNAL(disconnected()),this,SLOT(sockDis()));


}

void Server::sockReady()
{
    if(mTcpSocket->waitForConnected(500))
    {
       QByteArray oArrayData;
       QJsonDocument oDoc;
       QJsonParseError mDocError;

       oArrayData = mTcpSocket->readAll();
       oDoc  = QJsonDocument::fromJson(oArrayData,&mDocError);

       if(mDocError.errorString().toInt() == QJsonParseError::NoError)
       {
           if(oDoc.object().value("x").toString() ==  "login")
           {

              QString username = oDoc.object().value("username").toString();
              QString password = oDoc.object().value("password").toString();

              QStringList Keys,Values;
              if(checkLogin(username, password))
              {
                Keys.append("status");
                Keys.append("username");
                Values.append("login");
                Values.append(username);
                WriteServer("login",Keys,Values);
              }
              else
              {
                  Keys.append("status");
                  Values.append("filed");
                  WriteServer("login",Keys,Values);
              }

              qDebug() << username << password;
           }
           else
           {
                qDebug() << "Error reading data";
           }
       }
    }
}

void Server::sockDis()
{
    mTcpSocket->deleteLater();
}
