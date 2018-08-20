#include "connectdb.h"

connectDb::connectDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

connectDb::~connectDb()
{
      db.close();
}


bool connectDb::connect()
{
    QString sAppDataPath = QStandardPaths::writableLocation( QStandardPaths::DataLocation );
    QString sDbPath = sAppDataPath + QDir::separator() + "ATMfood.db";
    QFile oDbFile( sDbPath );
    if( !oDbFile.exists() )
    {
        InitDatabase();
    }

    db.setDatabaseName(sDbPath);
    return db.open();
}

void connectDb::InitDatabase()
{
   QString sDataPath = QStandardPaths::writableLocation( QStandardPaths::DataLocation );
   QDir oDir;
   if( !oDir.mkpath(sDataPath) )
   {
       return;
   }
   QString sDbPath = sDataPath + QDir::separator() + "ATMfood.db";
   QFile oDbFile( sDbPath );
   if(!oDbFile.open(QIODevice::WriteOnly))
       return;

   db.setDatabaseName(sDbPath);
   if( db.open() )
   {
       QSqlQuery q;
     q.exec("CREATE TABLE user ("
            "id	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "name	TEXT,"
            "lname	TEXT,"
            "room	TEXT,"
            "category	INTEGER,"
            "username	TEXT,"
            "password	TEXT"
        ");");


     q.exec("CREATE TABLE food ("
            "id	INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL,"
            "type TEXT,"
            "vendor TEXT,"
            "price	INTEGER, "
            "imgPath TEXT"
            ");");


     q.exec("CREATE TABLE cart ("
            "FoodId	INTEGER,"
            "UserId	INTEGER,"
            "count	INTEGER"
        ");");

     q.exec("CREATE TABLE ordershistory ("
            "foodid	TEXT NOT NULL,"
            "userid	TEXT NOT NULL,"
            "count	TEXT NOT NULL,"
            "date	TEXT"
             ");");
   }
}
