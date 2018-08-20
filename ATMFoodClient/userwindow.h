#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <controller.h>
#include <QObject>
#include <QFileDialog>
#include <QPixmap>
#include <QList>
#include <QTimer>
#include "loginInformation.h"
#include "optionsinformation.h"
#include "foodmodel.h"
#include "cartmodel.h"
#include "ordersmodel.h"
#include "usermodel.h"
#include "cartdialog.h"
#include "settingwindow.h"
#include "treeitem.h"


namespace Ui {
class UserWindow;
}

class UserWindow:public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = 0);
    ~UserWindow();

    void setLoginInfo(UserInfo info);

private:
    Ui::UserWindow *ui;

signals:
       void sigLoaded(bool bLoaded);

 private slots:


      void onLoadData(bool bError, QList<FoodInfo> foodInformations, int iMsgSize,QString sErrorText);

      void onUserCartReloaded(bool bError,QList<FoodInfo> foodInformations);

      void onLoadUserList(bool bError,QList<UserInforMation> userInformations);

      void on_logOutBtn_clicked();

      void on_addUserBtn_clicked();

      void on_AddFoodBtn_clicked();

      void onUserAdded(bool bError,QString sText);

      void onFoodAdded(bool bError,QString sText);

      void onLoadOrders(bool bError, TreeItem *root);

      void onLoadOrdersHistory(bool bError, TreeItem *root);

      void onCartSubmited(bool bError,QString sText);

      void onCartCanceled(bool bError,QString sText);

      void onUserDeleted(bool bError,QString sText);

      void onPasswordChanged(bool bError,QString sText);

      void onOrderedProducts(bool bError, QList<SinglOrder> lstSinglOrder);

      void on_selectPicBtn_clicked();

      void on_addCardBtn_clicked();

      void on_DeleteCardBtn_clicked();

      void on_SendCardBtn_clicked();

      void on_viewCartsBtn_clicked();

      void on_CanceleOrderBtn_clicked();

      void on_SubmitCartsBtn_clicked();

      void on_mainTab_tabBarClicked(int index);

      void on_AddUserAction_triggered();

      void on_home_triggered();

      void on_AddFoodAction_triggered();

      void on_Basket_triggered();

      void on_exitAction_triggered();

      void onUpdateBtns(bool bLoaded);

      void on_adminTab_tabBarClicked(int index);

      void on_NextBtn_clicked();

      void on_BackBtn_clicked();

      void on_searchBtn_clicked();

      void onTimeOutPing();

      void on_DeleteUserBtn_clicked();

      void on_settingBtn_clicked();



public:
      // Generate UI (Tables)
      void generateUI();
      void loadHistory();
      void loadBacket();
      void loadUsers();
      void loadFoods(int iSkipPointFoodData, QString sFilter, QString sValue, int iCount);
      void errLogout();


private:
    void setFirstWiew();
    bool isAdmin();


private:
    const int M_SKIP = 8;

    UserInfo m_loginInfo;
    int m_iPrice = 0;

    ShopNavigateInfo m_NavigateInfo;

    FoodModel* m_ShopModel;
    CartModel* m_CartModel;
    OrdersModel* m_OrdersModel;
    OrdersModel* m_OrdersHistory;
    UserModel* m_UserModel;


    QTimer * m_timer = new QTimer();


    CartDialog* pCartDialog;
    SettingWindow* pSettingWindow;

    QList<SinglOrder> m_AllOrderedProducts;




};

#endif // USERWINDOW_H
