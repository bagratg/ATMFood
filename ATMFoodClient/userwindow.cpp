#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserWindow),
    m_ShopModel(nullptr),
    m_CartModel(nullptr),
    m_UserModel(nullptr)
{
    ui->setupUi(this);

    // create shop model
    m_ShopModel = new FoodModel( this );

    // Set Header Data
   // m_ShopModel->setHeaderData(1, Qt::Vertical, "Summe", Qt::DisplayRole);

    ui->FoodTable->setModel(m_ShopModel);

    // create orders model

    m_OrdersModel = new OrdersModel( this );
    ui->AllOrdersView->setModel(m_OrdersModel);

    // create orders hstory model

    m_OrdersHistory = new OrdersModel( this );
    ui->treeViewHistory->setModel(m_OrdersHistory);

    // create cart model
    m_CartModel = new CartModel( this );
    ui->CartView->setModel(m_CartModel);

    // create user loist view;
    m_UserModel = new UserModel(this);
    ui->usersTableView->setModel(m_UserModel);

    // view config

    // connect signal slot
    connect(&Controller::getInstance(), &Controller::sigLoadData,
            this,&UserWindow::onLoadData, Qt::UniqueConnection);
    connect(&Controller::getInstance(), &Controller::sigLoadUserList,
            this,&UserWindow::onLoadUserList, Qt::UniqueConnection);
    connect(&Controller::getInstance(), &Controller::sigUserAdded,
            this,&UserWindow::onUserAdded, Qt::UniqueConnection);
    connect(&Controller::getInstance(),&Controller::sigFoodAdded,
            this,&UserWindow::onFoodAdded, Qt::UniqueConnection);
    connect(&Controller::getInstance(),&Controller::sigCartSubmited,
            this,&UserWindow::onCartSubmited, Qt::UniqueConnection);
    connect(&Controller::getInstance(),&Controller::sigCartCanceled,
            this,&UserWindow::onCartCanceled, Qt::UniqueConnection);
    connect(&Controller::getInstance(),&Controller::sigLoadedOrders,
            this,&UserWindow::onLoadOrders, Qt::UniqueConnection);
    connect(&Controller::getInstance(),&Controller::sigOrderedProducts,
            this,&UserWindow::onOrderedProducts, Qt::UniqueConnection);
    connect(&Controller::getInstance(),&Controller::sigUserCartReloaded,
            this,&UserWindow::onUserCartReloaded, Qt::UniqueConnection);
    connect(&Controller::getInstance(),&Controller::sigUserDeleted,
            this,&UserWindow::onUserDeleted, Qt::UniqueConnection);
    connect(&Controller::getInstance(),&Controller::sigPasswordChanged,
            this,&UserWindow::onPasswordChanged, Qt::UniqueConnection);

    connect(this,&UserWindow::sigLoaded,this,&UserWindow::onUpdateBtns);

    connect(&Controller::getInstance(),&Controller::sigLoadedOrdersHistory,
            this,&UserWindow::onLoadOrdersHistory, Qt::UniqueConnection);

    m_timer->start(500000); // pzdec shat sec
    connect(m_timer,SIGNAL(timeout()),this, SLOT(onTimeOutPing()));



}

UserWindow::~UserWindow()
{
    delete m_timer;
    delete ui;
}

void UserWindow::setLoginInfo(UserInfo info)
{
  QString a = m_loginInfo.sCategory;
  m_loginInfo = info;
  setFirstWiew();
}

void UserWindow:: onLoadData(bool bError, QList<FoodInfo> foodInformations,int iMsgSize,QString sErrorText)
{
    m_ShopModel->clear();

    if(bError && foodInformations.count()> m_ShopModel->GetSize())
    {
        for(int i = 0;i<foodInformations.size();i++)
        {
            m_ShopModel->addItem(foodInformations[i]);
        }

        m_NavigateInfo.iMsgSize = iMsgSize;

        if(m_NavigateInfo.iCurrentPage + M_SKIP >= iMsgSize)
        {
            ui->NextBtn->setEnabled(false);
        }

        if(m_NavigateInfo.iCurrentPage < m_NavigateInfo.iMsgSize)
        {
            int lastPage = m_NavigateInfo.iCurrentPage + m_ShopModel->GetSize();
            QString sCurrentPage = QString::number(m_NavigateInfo.iCurrentPage);
            QString sLastPage = QString::number(lastPage);

            ui->pageNumbLabel->setText(sCurrentPage+"-"+sLastPage+" / "+QString::number(m_NavigateInfo.iMsgSize));
        }
    }
    else
    {
        QMessageBox::information(this,"Ուշադրություն",sErrorText);
    }
}


void UserWindow::onUserCartReloaded(bool bError, QList<FoodInfo> foodInformations)
{
    if(bError)
    {
        if(foodInformations.size() == 0)
        {
            return;
        }

        for(int i = 0; i < foodInformations.size();i++)
        {
             m_CartModel->addItem(foodInformations[i]);
        }
        double dPrice = m_CartModel->calculatePrice();

        ui->FullPriceLabel->setText(tr("Ընդհանուր արժեք՝ %1").arg(dPrice));
        emit onUpdateBtns(true);
    }
    else
    {
        return;
    }
}

void UserWindow::onLoadUserList(bool bError, QList<UserInforMation> userInformations)
{
    if(bError)
    {
        for(UserInforMation uInfo:userInformations)
        {
            m_UserModel->addItem(uInfo);
        }
    }
}

bool UserWindow::isAdmin()
{
    if(m_loginInfo.sCategory == "1")
    {
        return true;
    }
  return false;
}

void UserWindow::setFirstWiew()
{
    // user informations
    ui->fnameLabel->setText(m_loginInfo.sFName);
    ui->lnameLabel->setText(m_loginInfo.sLName);
    ui->roomLabel->setText(m_loginInfo.sRoom);
    // isAdmin
    if(!isAdmin())
    {
        //setTabEnabled(1, false)
        ui->toolBar->actions().at(1)->setVisible(false);
        ui->toolBar->actions().at(2)->setVisible(false);
        ui->mainTab->removeTab(2);
    }

}

void UserWindow::on_logOutBtn_clicked()
{
    QMessageBox::StandardButton msgBoxReply;
    msgBoxReply = QMessageBox::question(this, "Ուշադրություն", "Ցանկանում եք դուրս գալ?", QMessageBox::Yes|QMessageBox::No);

      if (msgBoxReply == QMessageBox::Yes)
      {
        Controller::getInstance().logout(m_loginInfo.sToken);
      }
      else
      {
        return;
      }

}

void UserWindow::on_addUserBtn_clicked()
{
    QString sFname = ui->FnameEdit->text();
    QString sLname = ui->LnameEdit->text();
    QString sRoom = ui->RoomEdit->text();
    QString sUsername = ui->UserNameEdit->text().toLower();
    QString sCategory;
    QString sPassword;
    if(ui->PasswordEdit->text() == ui->RePasswordEdit->text())
    {
        sPassword = ui->PasswordEdit->text();
    }
    else
    {
        ui->RePasswordEdit->setStyleSheet("border-bottom: 2px solid red;");
        return;
    }
    ui->RePasswordEdit->setStyleSheet("border-bottom: 2px solid black;");
    if(sFname.isEmpty()|| sLname.isEmpty() ||sRoom.isEmpty()||sUsername.isEmpty()||sPassword.isEmpty())
    {
        QMessageBox::warning(this,"Ուշադրություն խնդիր !","պետք է լրացնել բոլոր դաշտերը");
    }
    if(ui->adminChBoX->isChecked())
    {
        sCategory = "1";
    }
    else
    {
       sCategory  = "0";
    }

    QString sToken = m_loginInfo.sToken;
    Controller::getInstance().addUser(sToken,sFname,sLname,sUsername,sPassword,sRoom,sCategory);
}

void UserWindow::on_AddFoodBtn_clicked()
{
    QString sFoodName = ui->FoodNameEdit->text();
    QString sFoodType = ui->FoodTypeBox->currentText();
    QString sFoodPrice = ui->FoodPriceSpinBox->text();
    QString sFoodVendor = ui->FoodVendorBox->currentText();
    QString sFoodImagePath = ui->PathLabel->text();
    if(sFoodName.isEmpty() || sFoodPrice == "0")
    {
        QMessageBox::warning(this,"Ուշադրություն տեղի է ունեցել սխալ","պետք է լրացնել բոլոր դաշտերը և գումարը 0 դրամից բարձր լինի");
        return;
    }
    QString sToken = m_loginInfo.sToken;
    Controller::getInstance().addFoodinDB(sFoodName,sFoodPrice,sFoodVendor,sFoodType,sFoodImagePath,sToken);

}

void UserWindow::onUserAdded(bool bError, QString sText)
{
        if(bError)
        {
            QMessageBox::information(this,"Ուշադրություն",sText);
        }
        else
        {
            QMessageBox::critical(this,"Ուշադրություն",sText);
        }
}

void UserWindow::onFoodAdded(bool bError, QString sText)
{

    if(bError)
    {
        QMessageBox::information(this,"Ուշադրություն",sText);
    }
    else
    {
        QMessageBox::critical(this,"Ուշադրություն",sText);
    }
}

void UserWindow::onLoadOrders(bool bError, TreeItem *root)
{
    if (bError)
    {
        m_OrdersModel->clear();
        m_OrdersModel->setItems(root);
    }
}

void UserWindow::onLoadOrdersHistory(bool bError, TreeItem *root)
{
    if (bError)
    {
        m_OrdersHistory->clear();
        m_OrdersHistory->setItems(root,true);
    }

}

void UserWindow::onCartSubmited(bool bError, QString sText)
{
    if(bError)
    {
        QMessageBox::information(this,"Ուշադրություն",sText);
    }
    else
    {
        QMessageBox::critical(this,"Ուշադրություն",sText);
    }
}

void UserWindow::onCartCanceled(bool bError, QString sText)
{
    if(bError)
    {
        QMessageBox::information(this,"Ուշադրություն",sText);
    }
    else
    {
        QMessageBox::critical(this,"Ուշադրություն",sText);
    }
}

void UserWindow::onUserDeleted(bool bError, QString sText)
{
    if(bError)
    {
        QMessageBox::information(this,"Ուշադրություն",sText);
        m_UserModel->clear();
        loadUsers();
    }

    else
    {
        QMessageBox::critical(this,"Ուշադրություն",sText);
    }
}

void UserWindow::onPasswordChanged(bool bError, QString sText)
{
    if(bError)
    {
        QMessageBox::information(this,"Ուշադրություն",sText);
    }
    else
    {
        QMessageBox::critical(this,"Ուշադրություն",sText);
    }
}

void UserWindow::onOrderedProducts(bool bError, QList<SinglOrder> lstSinglOrder)
{
    if(bError)
    {
        m_AllOrderedProducts = lstSinglOrder;
    }
    else
    {
        QMessageBox::critical(this,"Ուշադրություն","Տեղի է ունեցել սխալ Տվյալների բեռնման ժամանակ");
    }
}

void UserWindow::on_selectPicBtn_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                );
    ui->PathLabel->setText(imagePath);
    QPixmap img(imagePath);
    ui->ImageFoodLbel->setPixmap(img.scaled(100,100,Qt::KeepAspectRatio));
}


void UserWindow::on_addCardBtn_clicked()
{
    QItemSelectionModel *pSelect = ui->FoodTable->selectionModel();
    bool bOk = ui -> FoodTable->currentIndex().isValid();
    if( !pSelect || !bOk )
    {
        QMessageBox::warning(this, "Ուշադրություն","Պետք է ընտրել ինչ-որ բան, \n"
                                                   "Այնուհետև ավելացնել զամբյուղ" );
        return;
    }

    if(!ui->CanceleOrderBtn->isEnabled())
    {
        int idx = ui->FoodTable->currentIndex().row();

        FoodInfo Food = m_ShopModel->GetFoodInfo(idx);

        m_CartModel->addItem(Food);

        m_iPrice = m_CartModel->calculatePrice();

        ui->statusbar->showMessage(tr("Ընդհանուր արժեք՝ %1").arg(m_iPrice),3000);
        ui->FullPriceLabel->setText(tr("Ընդհանուր արժեք՝ %1").arg(m_iPrice));

        ui->SendCardBtn->setEnabled(true);
        ui->DeleteCardBtn->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this,"Ուշադրություն","Չեղարկեք պատվերը, այնուհետև պատվիրեք !");
       // loadBacket();
        ui->mainTab->setCurrentIndex(1);

    }

}

void UserWindow::on_DeleteCardBtn_clicked()
{
    QItemSelectionModel *pSelect = ui->CartView->selectionModel();
    bool bOk  = ui->CartView->currentIndex().isValid();
    if(!bOk || !pSelect)
    {
        QMessageBox::warning(this, "Ուշադրություն","Ընտրեք կամ Վերաթարմացրեք Զամբյուղը, \n"
                                                   "այնուհետև ջնջեք" );
        return;
    }
    int idx = ui->CartView->currentIndex().row();
    m_CartModel->removeItem(idx);
    if(m_CartModel->GetSize() == 0)
    {
        ui->SendCardBtn->setEnabled(false);
    }

    m_iPrice = m_CartModel->calculatePrice();


    ui->statusbar->showMessage(tr("Ընդհանուր արժեք՝ %1").arg(m_iPrice),3000);
    ui->FullPriceLabel->setText(tr("Ընդհանուր արժեք՝ %1").arg(m_iPrice));

}

void UserWindow::on_SendCardBtn_clicked()
{
    QString sToken = m_loginInfo.sToken;
    QList<CartInfo> lCartInfo;
    for(int i = 0; i  < m_CartModel->GetSize(); i++)
    {
        CartInfo tmp;

        tmp.sFoodID =  m_CartModel->GetFoodInfo(i).foodId;
        tmp.sCount =  QString::number(m_CartModel->getCount(i));
        tmp.sUserID = m_loginInfo.sUserId;

        lCartInfo.append(tmp);
    }

    Controller::getInstance().submitCart(lCartInfo,sToken);
    //Set Btns
    ui->SendCardBtn->setEnabled(false);
    ui->DeleteCardBtn->setEnabled(false);
    ui->CanceleOrderBtn->setEnabled(true);
}

void UserWindow::on_viewCartsBtn_clicked()
{
    m_OrdersModel->clear();
    QString sToken = m_loginInfo.sToken;
    Controller::getInstance().loadOrders(sToken);
    ui->SubmitCartsBtn->setEnabled(true);
}

void UserWindow::on_CanceleOrderBtn_clicked()
{
   QString sUserID = m_loginInfo.sUserId;
   QString sToken = m_loginInfo.sToken;
   Controller::getInstance().canceleCart(sUserID,sToken);
   //Set Btns
   ui->SendCardBtn->setEnabled(true);
   ui->DeleteCardBtn->setEnabled(true);
   ui->CanceleOrderBtn->setEnabled(false);
}

void UserWindow::on_SubmitCartsBtn_clicked()
{
    if(!m_AllOrderedProducts.isEmpty())
    {
        pCartDialog = new CartDialog(this);
        pCartDialog->show();
        pCartDialog->generateDialog(m_AllOrderedProducts);


        QMessageBox::StandardButton msgBoxReply;
        msgBoxReply = QMessageBox::question(this, "Ուշադրություն", "Սեղմեք Yes Վերջնական հաստատելու համար", QMessageBox::Yes|QMessageBox::No);

          if (msgBoxReply == QMessageBox::Yes)
          {
             Controller::getInstance().AdminSubmited(m_loginInfo.sToken);
          }
          else
          {
              return;
          }
    }
    else
    {
        QMessageBox::warning(this, "Ուշադրություն","Տեղի է ունեցել սխալ, \n"
                                                   "Հնարավոր չէ բեռնել ուտեստների տվյալները" );
        return;
    }
}

void UserWindow::generateUI()
{
    loadFoods(m_NavigateInfo.sSkip,"","",8);

    ui->mainTab->setCurrentIndex(0);
    ui->CanceleOrderBtn->setEnabled(false);
    loadBacket();
    loadUsers();

}

void UserWindow::loadHistory()
{
    QString sToken = m_loginInfo.sToken;
    Controller::getInstance().LoadOrderHistory(sToken);
}


void UserWindow::loadBacket()
{
     Controller::getInstance().loadUserCart(m_loginInfo.sToken, m_loginInfo.sUserId);
     ui->SendCardBtn->setEnabled(false);
}

void UserWindow::loadUsers()
{
   QString sToken = m_loginInfo.sToken;
   Controller::getInstance().loadUsers(sToken);
}

void UserWindow::loadFoods(int iSkipPointFoodData, QString sFilter, QString sValue,int iCount)
{
    QString sToken = m_loginInfo.sToken;

    Controller::getInstance().loadFoodData(sToken,iSkipPointFoodData,sFilter,sValue,iCount);
}

void UserWindow::errLogout()
{
        Controller::getInstance().logout(m_loginInfo.sToken);
}

void UserWindow::on_mainTab_tabBarClicked(int index)
{
    if(index == 2)
    {
       m_OrdersModel->clear();
       on_viewCartsBtn_clicked();
    }
}

void UserWindow::on_AddUserAction_triggered()
{
    ui->mainTab->setCurrentIndex(2);
    ui->adminTab->setCurrentIndex(1);
}

void UserWindow::on_home_triggered()
{
    ui->mainTab->setCurrentIndex(0);
}

void UserWindow::on_AddFoodAction_triggered()
{
    ui->mainTab->setCurrentIndex(2);
    ui->adminTab->setCurrentIndex(2);
}

void UserWindow::on_Basket_triggered()
{
    ui->mainTab->setCurrentIndex(1);
}

void UserWindow::on_exitAction_triggered()
{
    on_logOutBtn_clicked();
}

void UserWindow::onUpdateBtns(bool bLoaded)
{
    if(bLoaded)
    {
        if(m_CartModel->GetSize() != 0)
        {
            ui->CanceleOrderBtn->setEnabled(true);
            ui->DeleteCardBtn->setEnabled(false);
            ui->SendCardBtn->setEnabled(false);
        }
    }
}


//void UserWindow::on_test_clicked()
//{
//    Controller::getInstance().loadUserCart(m_loginInfo.sToken, m_loginInfo.sUserId);
//}


void UserWindow::on_adminTab_tabBarClicked(int index)
{
    switch (index) {
    case 0:
        on_viewCartsBtn_clicked();
    case 3:
        loadHistory();

        break;
    default:
        break;
    }
}

void UserWindow::on_NextBtn_clicked()
{
    m_NavigateInfo.sSkip += M_SKIP;

    int iItemCount = ui->ItemCountCombo->currentText().toInt();
    loadFoods(m_NavigateInfo.sSkip,m_NavigateInfo.sFilter,m_NavigateInfo.sValue,iItemCount);


    ui->BackBtn->setEnabled(true);


    if(m_NavigateInfo.iCurrentPage < m_NavigateInfo.iMsgSize )
    {
        m_NavigateInfo.iCurrentPage += M_SKIP;
    }

}

void UserWindow::on_BackBtn_clicked()
{
    if(m_NavigateInfo.iCurrentPage == 0 || M_SKIP == 0)
    {
        ui->BackBtn->setEnabled(false);
    }
    else
    {
         m_NavigateInfo.sSkip -= M_SKIP;

         int iItemCount = ui->ItemCountCombo->currentText().toInt();
         loadFoods(m_NavigateInfo.sSkip,m_NavigateInfo.sFilter,m_NavigateInfo.sValue,iItemCount);
    }
    ui->NextBtn->setEnabled(true);

    if(m_NavigateInfo.iCurrentPage != 0)
    {
       m_NavigateInfo.iCurrentPage -= M_SKIP;
    }


}

void UserWindow::on_searchBtn_clicked()
{
   m_NavigateInfo.sSkip = 0;
   m_NavigateInfo.sValue = ui->searchEdit->text();

   QString sCurrenText = ui->comboBox->currentText();

   if(sCurrenText == "Անուն")
   {
        m_NavigateInfo.sFilter = "name";
   }
   else if(sCurrenText == "Տեսակ")
   {
        m_NavigateInfo.sFilter = "type";
   }
   else if(sCurrenText == "Մատակարար")
   {
        m_NavigateInfo.sFilter = "vendor";
   }

   int iItemCount = ui->ItemCountCombo->currentText().toInt();
   loadFoods(m_NavigateInfo.sSkip,m_NavigateInfo.sFilter,m_NavigateInfo.sValue,iItemCount);


   if(m_ShopModel->GetSize()>0)
   {
       ui->NextBtn->setEnabled(true);
   }

}

void UserWindow::onTimeOutPing()
{
    QString sToken = m_loginInfo.sToken;
    Controller::getInstance().ping(sToken);
}

void UserWindow::on_DeleteUserBtn_clicked()
{
    QItemSelectionModel *pSelect = ui->usersTableView->selectionModel();
    bool bOk  = ui->usersTableView->currentIndex().isValid();
    if(!bOk || !pSelect)
    {
        QMessageBox::warning(this, "Ուշադրություն","Ընտրեք Օգտատիրոջը ջնջելու համար");
        return;
    }


    int idx = pSelect->currentIndex().row();

    QString sUsername = m_UserModel->GetUserInfo(idx).sUsername;

    QString sToken = m_loginInfo.sToken;

    QMessageBox::StandardButton msgBoxReply;
    msgBoxReply = QMessageBox::question(this, "Ուշադրություն", "Դուք համոզված ե՞ք", QMessageBox::Yes|QMessageBox::No);

      if (msgBoxReply == QMessageBox::Yes)
      {
            Controller::getInstance().deleteUsers(sUsername,sToken);
      }
      else
      {
          return;
      }


}

void UserWindow::on_settingBtn_clicked()
{
    pSettingWindow = new SettingWindow(this);
    pSettingWindow->setUserInfo(m_loginInfo);
    pSettingWindow->show();
}
