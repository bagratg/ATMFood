#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <requestresponse.h>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager();
    ~NetworkManager();

public:
    bool connectToHost();
    void writeServer(Request oRequest);

    void setServerIp(QString sIp);

signals:
    void sigError( QString sErrorMessage );

private slots:
    void onReadyRead();
    void onDisconnected();
    void onSocketError(QAbstractSocket::SocketError eSocketError);

private:
    QTcpSocket* m_pSocket;
    QByteArray oArrayData;
    QString m_sServerIp;

};

#endif // NETWORKMANAGER_H
