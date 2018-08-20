#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QString>
#include <QSqlQuery>
#include <QByteArray>
#include <QDebug>
#include "controller.h"

class NetworkManager:public QTcpServer
{
    Q_OBJECT

public:
    NetworkManager();
   // QTcpSocket* mTcpSocket;
    Controller mControl;


public slots:
    // Server slots
    void startServer();
//    void incomingConnection(int socketDiscinnect);
    void toReadyRead();
    void toDisconnect();

    // test
    void newSocketConnet();

public:
    void WriteServer(Response oResponse, QTcpSocket *clientSocket);

private:
    QMap<int,QTcpSocket*> m_mpClients;
    const int PORT = 1674;
};

#endif // NETWORKMANAGER_H
