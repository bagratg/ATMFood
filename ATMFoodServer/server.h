#ifndef SERVER_H
#define SERVER_H

#endif // SERVER_H


class Server
{
public:
    Server::Server(){}
    Server::~Server(){}


public:

    bool checkLogin(QString username, QString password);
    void WriteServer(QString x, QStringList StringsKey, QStringList StringsValue);
    void startServer();
    void incomingConnection(int socketDis);
    void sockReady();
    void sockDis();



}
