#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QMessageBox>
#include <QDebug>
#include <QList>
#include <QPixmap>

#include "networkmanager.h"
#include "requestresponse.h"
#include "loginInformation.h"
#include "foodinformation.h"
#include "cartinformation.h"
#include "userinformation.h"
#include "foodmodel.h"

#include "order.h"
#include "treeitem.h"



class Controller : public QObject
{

    Q_OBJECT


public:
    static Controller& getInstance();

public:
    void handleResponse(const QJsonDocument &doc);
    void login(QString sUsername, QString sPassword);
    void logout(QString sToken);
    void loadFoodData(QString sToken, int iSkipPoint, QString sFilter, QString sValue,int iCount);
    void loadOrders(QString sToken);
    void submitCart(QList<CartInfo> lCartInfo, QString sToken);
    void canceleCart(QString sUserID,QString sToken);
    void LoadOrderHistory(QString sToken);
    void loadUsers(QString sToken);
    void deleteUsers(QString sUsername,QString sToken);
    void changePassword(QString sUserID, QString sToken, QString sLastPass, QString sNewPass);

    void ping(QString sToken);


    UserInfo getUserInfo();
    // Admin methods
    void addUser(QString sToken,
                 QString sFname,
                 QString sLname,
                 QString sUsername,
                 QString sPassword,
                 QString sRoom,
                 QString sCategory);
    void addFoodinDB(QString sFoodName,
                     QString sFoodPrice,
                     QString sFoodVendor,
                     QString sFoodType,
                     QPixmap PMImage,
                     QString sToken);

    void loadUserCart(QString sToken, QString sUserId);
    void AdminSubmited(QString sToken);


signals:
    void sigError( QString sErrorMessage );
    void sigErrLogout(QString sErrorMsg);
    // STE PTI AVELACNEM VOR &doc UXXARKI TE CHE SENC CHEXAV !!!
    void sigLoginFinished(bool bSucceeded,
                          QString sErorrMessage = QString(),
                          UserInfo LoginInfo =UserInfo()
                          );
    void sigLoadData(bool bError, QList<FoodInfo> foodInformations,int iMsgSize,QString sErrorText);
    void sigLoadUserList(bool bError, QList<UserInforMation> userInformations);
    void sigUserAdded(bool bError,QString sText);
    void sigFoodAdded(bool bError,QString sText);
    void sigCartSubmited(bool bError, QString sText);
    void sigCartCanceled(bool bError, QString sText);
    void sigPasswordChanged(bool bError,QString sText);
    void sigUserDeleted(bool bError,QString sText);
    void sigLoadedOrders(bool bError, TreeItem *root);
    void sigOrderedProducts(bool bError,QList<SinglOrder>);
    void sigUserCartReloaded(bool bError,QList<FoodInfo> foodInformations);
    void sigLoadedOrdersHistory(bool bError, TreeItem *root);

   // void sigOrderedProducts(bool bError,QList<FoodInfo> AllOrderedProducts);
    //void sigFoodLoaded(bool bError,QString )
    // local signals
    void sErrLogout(bool bError);


private:
    Controller();

private:
    NetworkManager m_nManger;
    UserInfo m_iLogin;
};

#endif // CONTROLLER_H
