#ifndef CONNECTDB_H
#define CONNECTDB_H
#include <QtSql>
#include <QString>

class connectDb
{
public:
    connectDb();
    ~connectDb();

static connectDb& getInstance()
{
    static connectDb connectInstance;
    return connectInstance;
}

QSqlDatabase &getDB()
{
    return db;
}

bool connect();

private:
    void InitDatabase();

private:
    QSqlDatabase db;

};

#endif // CONNECTDB_H
