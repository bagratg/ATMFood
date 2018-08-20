#include <QCoreApplication>
#include "networkmanager.h"
#include "connectdb.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("ATMFood");
    a.setApplicationVersion("v1");
    a.setOrganizationName("Dushman Production");

    if( !connectDb::getInstance().connect() )
    {
        qDebug() << "error connection database";
        return 0;
    }

    NetworkManager mServer;
    mServer.startServer();

    return a.exec();
}
