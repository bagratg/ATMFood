#include "networkmanager.h"
#include "controller.h"

NetworkManager::NetworkManager()
{
    m_pSocket = new QTcpSocket(this);
    QObject::connect(m_pSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    QObject::connect(m_pSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    bool bOK = QObject::connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    Q_ASSERT(bOK);
}

NetworkManager::~NetworkManager()
{
    delete m_pSocket;
}

bool NetworkManager::connectToHost()
{
    m_pSocket->connectToHost(m_sServerIp,1674);
    return m_pSocket->waitForConnected(3000);
}

void NetworkManager::onReadyRead()
{
      QJsonDocument oDoc;
      QJsonParseError mDocError;

      QByteArray aCurrentData = m_pSocket->readAll();
      oArrayData.append( aCurrentData );


      qDebug() << oArrayData.size();

      QString sArr(oArrayData);

      if( !sArr.endsWith( "\r\n\r\n" ) )
      {
          // partial data, wait fow the next readyRead
          return;
      }

      QStringList lstOArr = sArr.split("\r\n\r\n",QString::SkipEmptyParts);


      for(int i = 0; i< lstOArr.size();i++)
      {
          QByteArray arrTmp = lstOArr[i].toUtf8();

          oDoc  = QJsonDocument::fromJson(arrTmp,&mDocError);

          if(mDocError.errorString().toInt() == QJsonParseError::NoError)
          {
               Controller::getInstance().handleResponse(oDoc);
          }

      }

      // clear buffer
      oArrayData.clear();
}

void NetworkManager::onDisconnected()
{
  //  TODO!!!
    m_pSocket->deleteLater();
}

void NetworkManager::onSocketError(QAbstractSocket::SocketError eSocketError)
{
    if( eSocketError == QAbstractSocket::OperationError )
        return;

    QString sErrMsg = m_pSocket->errorString();
    emit sigError( sErrMsg );
}

void NetworkManager::writeServer(Request oRequest)
{
    //auto eState = m_pSocket->state();
    if( m_pSocket->state() != QAbstractSocket::ConnectedState )
    {
        emit sigError( "Սերվերը չի պատասխանում" );
        return;
    }

    QJsonObject oJsonObject;

    if(oRequest.oData.isArray())
    {
        oJsonObject["data"] = oRequest.oData.array();
    }
    else
    {
        oJsonObject["data"] = oRequest.oData.object();
    }

    oJsonObject["command"] = oRequest.sCommand;
    oJsonObject["token"] = oRequest.sToken;
    QJsonDocument oJsonDoc(oJsonObject);

    QByteArray oByteArray = oJsonDoc.toJson();
    oByteArray.append("\r\n\r\n");
    m_pSocket->write(oByteArray);

}

void NetworkManager::setServerIp(QString sIp)
{
      m_sServerIp = sIp;
}
