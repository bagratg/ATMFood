#ifndef Controller_H
#define CONTROLLER_H

#include <QString>
#include <QSqlRecord>
#include <QJsonDocument>
#include <QSqlQuery>
#include <QJsonArray>
#include <QTime>
#include <QtGui/QPixmap>
#include <information.h>
#include <QObject>
#include "connectdb.h"
#include "requestresponse.h"
#include "sesioncontroller.h"

class Controller
{
public:
    Controller();
    ~Controller();

public:
    Response proccessCommand(const QJsonDocument &doc);
    // interface

    Response login(QString sUsername, QString sPassword); // login{"true/false","name","lname","room","category","token"}
    Response logout(QString sToken); // logout{error:false/true};

    Response ping(QString sToken);

    Response loadFoodData(int iSkip, QString sFilter, QString sValue, int iItemCount);
    Response handleOrder(QList<OrderInfo> sCardList);

    Response loadOrders();

    Response loadUsers();
    Response DeleteUser(QString sUsername);

    Response changePassword(QString sUserID, QString sLastPass, QString sNewPass);

    Response loadUserCart(QString sUserID);

    Response canceleOrder(QString sUserID);

//    // ADMIN FUNCTIONS INTEFACE
//    // all +
      Response createUser(QString sUsername,
                          QString sPassword,
                          QString sRoom,
                          QString sFname,
                          QString sLname,
                          QString sCategory,
                          QString sToken);

      Response addFoodInfo(QString sFoodName,
                           QString sFoodPrice,
                           QString sFoodVendor,
                           QString sFoodType);

      Response adminSubmited(QString sToken);

      Response loadOrdersHistory(QString sToken);



private:
     SesionController session;
};


#endif // Controller_H
