#include "networkmanager.h"

NetworkManager::NetworkManager()
{

}

void NetworkManager::startServer()
{
    if(this->listen(QHostAddress::Any, PORT))
    {
        qDebug() << "Listening on port" << PORT;
    }
    else
    {
        qDebug() << "Failed to start server";
    }
     QObject::connect(this, SIGNAL(newConnection()), this, SLOT(newSocketConnet()));
}


void NetworkManager::toReadyRead()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    //int idusersocs = clientSocket->socketDescriptor();


       QByteArray oArrayData;
       QJsonDocument oDoc;
       QJsonParseError mDocError;

       oArrayData = clientSocket->readAll();


       QString sByteArray(oArrayData);

       QStringList lstSByteArray  =  sByteArray.split("\r\n\r\n",QString::SkipEmptyParts);

       for(int i = 0; i < lstSByteArray.size(); i++)
       {

          QByteArray arrTmp = lstSByteArray[i].toUtf8();

          oDoc  = QJsonDocument::fromJson(arrTmp,&mDocError);

          if(mDocError.errorString().toInt() == QJsonParseError::NoError)
          {
               qDebug() << "Request:"<< QTime::currentTime().toString() << oDoc;
               Response tmp = mControl.proccessCommand(oDoc);
               qDebug() << "Response: "<< QTime::currentTime().toString() << " :Error =" << tmp.oError ;
               WriteServer(tmp,clientSocket);
          }

       }


}

void NetworkManager::toDisconnect()
{
  /*  clientSocket->deleteLater()*/;
}

void NetworkManager::newSocketConnet()
{
        QTcpSocket* clientSocket = this->nextPendingConnection();
        int idusersocs = clientSocket->socketDescriptor();
        m_mpClients[idusersocs] = clientSocket;


        QObject::connect(m_mpClients[idusersocs],SIGNAL(readyRead()),this,SLOT(toReadyRead()));
        QObject::connect(m_mpClients[idusersocs],SIGNAL(disconnected()),this,SLOT(toDisconnect()));

        qDebug() << "New Connection !" << idusersocs;
}

void NetworkManager::WriteServer(Response oResponse, QTcpSocket*clientSocket)
{
       QJsonObject oJsonObject;
       if(oResponse.oData.isArray())
       {
           oJsonObject["data"] = oResponse.oData.array();
       }
       else
       {
           oJsonObject["data"] = oResponse.oData.object();
       }

       oJsonObject["error"] = oResponse.oError;
       oJsonObject["command"] = oResponse.sCommand;
       oJsonObject["size"] = oResponse.iMsgSize;
       oJsonObject["errorText"] = oResponse.sErrorText;

       QJsonDocument oJsonDoc(oJsonObject);
       QByteArray oByteArray = oJsonDoc.toJson();

       oByteArray.append("\r\n\r\n");

       clientSocket->write(oByteArray);
       clientSocket->waitForBytesWritten();

}
