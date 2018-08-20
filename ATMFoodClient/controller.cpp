#include "controller.h"
#include "treeitem.h"

#include <QVariantList>


Controller::Controller()
{
    m_nManger.setServerIp("10.100.100.80");
    connect( &m_nManger, &NetworkManager::sigError, this, &Controller::sigError );
}

Controller &Controller::getInstance()
{
    static Controller ControllerInstance;
    return ControllerInstance;
}

void Controller::handleResponse(const QJsonDocument &doc)
{

    QString sCommand = doc.object().value("command").toString();
    QJsonObject oData  = doc.object().value("data").toObject();
    QString sErrorText = doc.object().value("errorText").toString();
    bool bError = doc.object().value("error").toBool();
  if(!bError)
  {

    if(sCommand == "login")
    {

        m_iLogin.sFName = oData.value("name").toString();
        m_iLogin.sLName =  oData.value("lname").toString();
        m_iLogin.sRoom  =  oData.value("room").toString();
        m_iLogin.sCategory =  oData.value("category").toString();
        m_iLogin.sUserName =  oData.value("username").toString();
        m_iLogin.sUserId =  oData.value("id").toString();
        m_iLogin.sToken = oData.value("token").toString();

        if(sErrorText.isEmpty())
        {
            emit sigLoginFinished(true,QString(),m_iLogin);
        }
        else
        {
          emit sigLoginFinished(false, sErrorText);
        }
    }

    if(sCommand == "logout")
    {
        if(!bError)
        {
             emit sErrLogout(true);
        }
        else
        {
            emit  sErrLogout(false);
        }

    }

    if(sCommand == "loadFoodData")
    {
        auto vJson  = doc.object().value("data");
        int iMsgSize = doc.object().value("size").toInt();
        if(vJson.isArray())
        {
            QJsonArray oArray = vJson.toArray();
            QList<FoodInfo> foodInformations;
            for(int i = 0; i < oArray.count(); i++)
            {
                if( oArray.at(i).isObject() )
                {
                     QJsonObject oJson  = oArray.at(i).toObject();
                     FoodInfo tmp;
                     tmp.foodId = oJson.value("id").toString();
                     tmp.foodName = oJson.value("name").toString();
                     tmp.foodType = oJson.value("type").toString();
                     tmp.foodVendor = oJson.value("vendor").toString();
                     tmp.foodPrice = oJson.value("price").toString();
                     // add image
                     QByteArray aImgbase64;
                     if(oJson.value("image").isString())
                     {
                         aImgbase64 = oJson.value("image").toString().toUtf8();
                     }



                     tmp.foodImage.loadFromData(QByteArray::fromBase64(aImgbase64));


                     foodInformations.append(tmp);
                }
                else
                {
                    // throw
                    continue;
                }
            }
            QString sErrorText = doc.object().value("errorText").toString();

            if(sErrorText.isEmpty())
            {
                emit sigLoadData(true,foodInformations,iMsgSize,sErrorText);
            }
            else
            {
                emit sigLoadData(false,foodInformations,iMsgSize,sErrorText);
            }

       }
    }

    if(sCommand == "createUser")
    {
        if(sErrorText.isEmpty())
        {
            emit sigUserAdded(true,"Օգտատերը հաջողությամբ ավելացվել է");
        }
        else
        {
          emit sigUserAdded(false,sErrorText);
        }
    }

    if(sCommand == "changePassword")
    {
        if(sErrorText.isEmpty())
        {
            emit sigPasswordChanged(true," Գաղտնաբառը հաջողությամբ փոփոխվել է");
        }
        else
        {
            emit sigPasswordChanged(false,sErrorText);
        }
    }

    if(sCommand == "addFood")
    {
        if(!bError)
        {
            emit sigFoodAdded(true,"ՈՒտեստը հաջողությամբ ավելացվել է");
        }
        else
        {
            emit sigUserAdded(false,"Տեղի է ունեցել սխալ");
        }
    }

    if(sCommand == "SubmitCart")
    {
        if(!bError)
        {
             emit sigFoodAdded(true,"ՈՒտեստը հաջողությամբ ուղղարկվել է։\n "
                                    "Դուք կարող եք չեղարկել այն քանի դեռ ադմինիստրատորը չի հաստատել պատվերը");
        }
        else
        {
             emit sigFoodAdded(true,"Տեղի Է ունեցել սխալ");
        }
    }

    if(sCommand == "CanceleCart")
    {
        if(sErrorText.isEmpty())
        {
            emit sigCartCanceled(true,"Ձեր պատվերը Հաջողությամբ չեղարկվել է");
        }
        else
        {
          emit sigCartCanceled(true,sErrorText);
        }
    }

    if(sCommand == "loadOrders")
    {
       QJsonObject oData = doc.object().value("data").toObject();
       QStringList lstKeys = oData.keys();
       //QList<FoodInfo> AllOrderedProducts;
       QList<SinglOrder> lstAllOrderedProducts;

       TreeItem *root = new TreeItem(QList<QVariant>());

       for(int i = 0; i < lstKeys.size(); i++)
       {
           QList<QVariant> lstVariant;

         //  TreeItem *userNode = new TreeItem(lstVariant, root);
          // root->appendChild(userNode);

           auto vJson  = oData.value(lstKeys[i]);

           if(vJson.isArray())
           {
               QJsonArray oArray = vJson.toArray();
               // Get USername QJsonObject oJson
               QJsonObject oJson  = oArray.at(0).toObject();
               QString sUsername =  oJson.value("username").toString();

               lstVariant.append(sUsername);
               lstVariant.append(0);


               TreeItem *userNode = new TreeItem(lstVariant, root);
               root->appendChild(userNode);

               //Get food with user order
               int iUserTotalprice = 0;
               for(int j = 0; j < oArray.count(); j++)
               {
                   if( oArray.at(j).isObject() )
                   {
                        QJsonObject oTmpJson  = oArray.at(j).toObject();

                        QJsonObject oJson = oTmpJson.value("food").toObject();
                        FoodInfo tmp;

                        tmp.foodId = oJson.value("id").toString();
                        tmp.foodName = oJson.value("name").toString();
                        tmp.foodType = oJson.value("type").toString();
                        tmp.foodVendor = oJson.value("vendor").toString();
                        tmp.foodPrice = oJson.value("price").toString();
                        QString count = oTmpJson.value("count").toString();

                        QList<QVariant> data;
                        data.append(tmp.foodName);
                        data.append(tmp.foodVendor);
                        data.append(tmp.foodPrice);
                        data.append(count);

                        // price
                        iUserTotalprice += tmp.foodPrice.toInt()*count.toInt();

                        TreeItem *foodNode = new TreeItem(data, userNode);
                        userNode->appendChild(foodNode);

                        SinglOrder tmpOrder;
                        tmpOrder.count = count.toInt();
                        tmpOrder.fInfo = tmp;
                     //   AllOrderedProducts.append(tmp);
                        lstAllOrderedProducts.append(tmpOrder);

                   }
                   else
                   {
                       // throw
                       continue;
                   }
               }
           //   replace price
               userNode->replaceData(1,iUserTotalprice);


           }
           emit sigOrderedProducts(true,lstAllOrderedProducts);
        // emit sigOrderedProducts(true,AllOrderedProducts);

       }
       // Calculate
       emit sigLoadedOrders(true, root);
     }

    if(sCommand == "LoadUserList")
    {
        if(!bError)
        {
             auto vJson  = doc.object().value("data");
             if(vJson.isArray())
             {
                 QJsonArray oArray = vJson.toArray();
                 QList<UserInforMation> userInformations;
                 for(int i = 0; i < oArray.count(); i++)
                 {
                     if( oArray.at(i).isObject() )
                     {
                          QJsonObject oJson  = oArray.at(i).toObject();
                          UserInforMation tmp;

                          tmp.sName = oJson.value("name").toString();
                          tmp.sUsername = oJson.value("username").toString();
                          tmp.sRole = oJson.value("category").toString();

                          userInformations.append(tmp);
                     }
                     else
                     {
                         // throw
                         continue;
                     }
                 }

                 emit sigLoadUserList(true,userInformations);
              }
        }
    }

    if(sCommand == "loadUserCart")
    {
        if(!bError)
        {
             auto vJson  = doc.object().value("data");
             if(vJson.isArray())
             {
                 QJsonArray oArray = vJson.toArray();
                 QList<FoodInfo> foodInformations;
                 for(int i = 0; i < oArray.count(); i++)
                 {
                     if( oArray.at(i).isObject() )
                     {
                          QJsonObject oJson  = oArray.at(i).toObject();
                          FoodInfo tmp;

                          tmp.foodId = oJson.value("id").toString();
                          tmp.foodName = oJson.value("name").toString();
                          tmp.foodType = oJson.value("type").toString();
                          tmp.foodVendor = oJson.value("vendor").toString();
                          tmp.foodPrice = oJson.value("price").toString();

                          // add image
                          QByteArray aImgbase64;
                          if(oJson.value("image").isString())
                          {
                              aImgbase64 = oJson.value("image").toString().toUtf8();
                          }



                          tmp.foodImage.loadFromData(QByteArray::fromBase64(aImgbase64));

                          foodInformations.append(tmp);
                     }
                     else
                     {
                         // throw
                         continue;
                     }
                 }
                 emit sigUserCartReloaded(true,foodInformations);
             }

        }
    }

    if(sCommand == "deleteUser")
    {
        if(!bError)
        {
            emit sigUserDeleted(true,"Օգտատերը հաջողությամբ Հեռացվել է ցուցակից");
        }
        else
        {
            emit sigUserDeleted(false,"Տեղի է ունեցել սխալ");
        }
    }

    if(sCommand == "loadOrdersHistory")
    {
       QJsonObject oData = doc.object().value("data").toObject();

       QStringList lstDates = oData.keys();

       TreeItem *root = new TreeItem(QList<QVariant>());
       for(QString date:lstDates)
       {
          auto tmpArr = oData[date];

          if(tmpArr.isArray())
          {
              QVariantList lstDateVariant;
              lstDateVariant.append(date);
              TreeItem *dateNode = new TreeItem(lstDateVariant, root);
              root->appendChild(dateNode);

              QJsonArray oUsersArrInfo = tmpArr.toArray();


              for(int i = 0; i < oUsersArrInfo.size(); i++)
              {

                     QJsonObject objFoods = oUsersArrInfo[i].toObject();
                     QStringList lstObj = objFoods.keys();

                     for(QString obj:lstObj)
                     {
                         QVariantList lstUserVariant;
                         lstUserVariant.append("UserId:" + obj);
                         lstUserVariant.append(0);
                         TreeItem *userNode = new TreeItem(lstUserVariant, dateNode);
                         dateNode->appendChild(userNode);


                         auto vJson = objFoods[obj];

                         if(vJson.isArray())
                         {
                             int iUserTotalprice = 0;
                             QJsonArray oArray = vJson.toArray();

                             for(int i = 0; i < oArray.count(); i++)
                             {
                                 if( oArray.at(i).isObject() )
                                 {

                                      QJsonObject oJson  = oArray.at(i).toObject();

                                      QString sUserName = oJson.value("username").toString();
                                      QString sCount = oJson.value("count").toString();
                                      userNode->replaceData(0, sUserName);

                                      QJsonObject tmpFood = oJson.value("food").toObject();

                                      FoodInfo tmp;
                                      tmp.foodId = tmpFood.value("id").toString();
                                      tmp.foodName = tmpFood.value("name").toString();
                                      tmp.foodType = tmpFood.value("type").toString();
                                      tmp.foodVendor = tmpFood.value("vendor").toString();
                                      tmp.foodPrice = tmpFood.value("price").toString();

                                      QVariantList lstFoodVariant;
                                      lstFoodVariant.append(tmp.foodName);
                                      lstFoodVariant.append(tmp.foodVendor);
                                      lstFoodVariant.append(tmp.foodPrice);
                                      lstFoodVariant.append(sCount);

                                      // price
                                      iUserTotalprice += tmp.foodPrice.toInt()*sCount.toInt();

                                      TreeItem *foodNode = new TreeItem(lstFoodVariant, userNode);
                                      userNode->appendChild(foodNode);


                                 }
                             }
                             userNode->replaceData(1, iUserTotalprice);
                         }
                     }
                }
            }
        }

       emit sigLoadedOrdersHistory(true, root);

    }

    if(sCommand == "ErrToken")
    {
        emit sigErrLogout(sErrorText);
    }
    if(sCommand == "ErrCommand")
    {
        emit sigErrLogout(sErrorText);
    }
  }
}


void Controller::login(QString sUsername, QString sPassword)
{
    Request login;
    QJsonObject oJson;
    if( m_nManger.connectToHost())
    {
        login.sCommand = "login";
        login.sToken == nullptr;
        oJson["username"] = sUsername;
        oJson["password"] = sPassword;
        QJsonDocument jsonDoc(oJson);
        login.oData = jsonDoc;
        m_nManger.writeServer(login);
    }
}

void Controller::logout(QString sToken)
{
    Request rLogout;
    if(m_nManger.connectToHost())
    {
        rLogout.sCommand = "logout";
        rLogout.sToken = sToken;
        m_nManger.writeServer(rLogout);
    }

}

void Controller::addUser(QString sToken,
                         QString sFname,
                         QString sLname,
                         QString sUsername,
                         QString sPassword,
                         QString sRoom,
                         QString sCategory)
{
    if(m_nManger.connectToHost())
    {
        Request rAddUser;
        QJsonObject oJson;
        rAddUser.sCommand = "createUser";
        rAddUser.sToken = sToken;
        oJson["fname"] =  sFname;
        oJson["lname"] = sLname;
        oJson["room"] = sRoom;
        oJson["room"] = sRoom;
        oJson["username"] = sUsername;
        oJson["password"] = sPassword;
        oJson["category"] = sCategory;
        QJsonDocument jsonDoc(oJson);
        rAddUser.oData = jsonDoc;
        m_nManger.writeServer(rAddUser);
    }

}

void Controller::addFoodinDB(QString sFoodName,
                             QString sFoodPrice,
                             QString sFoodVendor,
                             QString sFoodType,
                             QPixmap PMImage,
                             QString sToken)
{
    if(m_nManger.connectToHost())
    {
        Request rAddFoodInfo;
        rAddFoodInfo.image = PMImage;

        QJsonObject oJson;
        rAddFoodInfo.sCommand = "addFood";
        rAddFoodInfo.sToken = sToken;
        oJson["FoodName"] =  sFoodName;
        oJson["FoodType"] = sFoodType;
        oJson["FoodVendor"] = sFoodVendor;
        oJson["FoodPrice"] = sFoodPrice;

        QJsonDocument jsonDoc(oJson);
        rAddFoodInfo.oData = jsonDoc;
        m_nManger.writeServer(rAddFoodInfo);
    }
}

void Controller::loadUserCart(QString sToken, QString sUserId)
{
    if(m_nManger.connectToHost())
    {
        Request rLoadUserCart;

        QJsonObject oJson;
        rLoadUserCart.sCommand = "loadUserCart";
        rLoadUserCart.sToken = sToken;
        oJson["UserID"] =  sUserId;
        QJsonDocument jsonDoc(oJson);
        rLoadUserCart.oData = jsonDoc;
        m_nManger.writeServer(rLoadUserCart);
    }
}

void Controller::AdminSubmited(QString sToken)
{
    if(m_nManger.connectToHost())
    {
        Request rAdminSubmited;
        rAdminSubmited.sToken = sToken;
        rAdminSubmited.sCommand = "adminSubmited";

        m_nManger.writeServer(rAdminSubmited);
    }
}

void Controller::loadFoodData(QString sToken,int iSkipPoint,QString sFilter,QString sValue,int iCount)
{

    if( m_nManger.connectToHost())
    {
        Request rloadFoodData;
        rloadFoodData.sCommand = "loadFoodData";
        rloadFoodData.sToken = sToken;

        QJsonObject oJsonData;

        oJsonData["skip"] = iSkipPoint;
        oJsonData["filter"] = sFilter;
        oJsonData["value"] = sValue;
        oJsonData["count"] = iCount;

        QJsonDocument jsonDoc(oJsonData);

        rloadFoodData.oData = jsonDoc;


        m_nManger.writeServer(rloadFoodData);
    }

}

void Controller::loadOrders(QString sToken)
{
    if(m_nManger.connectToHost())
    {
        Request rLoadOrders;
        rLoadOrders.sCommand = "loadOrders";
        rLoadOrders.sToken = sToken;
        m_nManger.writeServer(rLoadOrders);
    }

}

void Controller::submitCart(QList<CartInfo> lCartInfo ,QString sToken)
{
    if(m_nManger.connectToHost())
    {
        Request rSubmitCart;
        QJsonArray oArray;
        rSubmitCart.sToken = sToken;
        rSubmitCart.sCommand = "SubmitCart";


        for(int i = 0; i < lCartInfo.size(); i++)
        {
           QJsonObject oJsonData;

            oJsonData["UserID"] = lCartInfo[i].sUserID;
            oJsonData["FoodID"] = lCartInfo[i].sFoodID;
            oJsonData["count"] = lCartInfo[i].sCount;

            oArray.append(oJsonData);

        }

        QJsonDocument jsonDoc(oArray);
        rSubmitCart.oData = jsonDoc;
        m_nManger.writeServer(rSubmitCart);
    }

}

void Controller::canceleCart(QString sUserID, QString sToken)
{
    if(m_nManger.connectToHost())
    {
        Request rCanceleCart;
        QJsonObject oJsonData;
        rCanceleCart.sToken = sToken;
        rCanceleCart.sCommand = "CanceleCart";

        oJsonData["UserID"] = sUserID;

        QJsonDocument jsonDoc(oJsonData);

        rCanceleCart.oData = jsonDoc;

        m_nManger.writeServer(rCanceleCart);
    }

}

void Controller::LoadOrderHistory(QString sToken)
{
    if(m_nManger.connectToHost())
    {
        Request rLoadUsers;
        rLoadUsers.sCommand = "Loadhistory";
        rLoadUsers.sToken = sToken;
        m_nManger.writeServer(rLoadUsers);
    }

}

void Controller::loadUsers(QString sToken)
{
    if(m_nManger.connectToHost())
    {
        Request rLoadUsers;
        rLoadUsers.sCommand = "LoadUserList";
        rLoadUsers.sToken = sToken;
        m_nManger.writeServer(rLoadUsers);
    }
}

void Controller::deleteUsers(QString sUsername, QString sToken)
{
    if(m_nManger.connectToHost())
    {
        Request rDeleteUser;
        rDeleteUser.sCommand = "deleteUser";
        rDeleteUser.sToken = sToken;
        QJsonObject oJsonData;
        oJsonData["username"] = sUsername;
        QJsonDocument jsonDoc(oJsonData);

        rDeleteUser.oData = jsonDoc;
        m_nManger.writeServer(rDeleteUser);
    }
}

void Controller::changePassword(QString sUserID,QString sToken ,QString sLastPass, QString sNewPass)
{
    Request rChangePassword;
    rChangePassword.sCommand = "changePassword";
    rChangePassword.sToken = sToken;

    QJsonObject oJsonData;
    oJsonData["lastPass"] = sLastPass;
    oJsonData["newPass"] = sNewPass;
    oJsonData["userId"] = sUserID;


    QJsonDocument jsonDoc(oJsonData);

    rChangePassword.oData = jsonDoc;
    m_nManger.writeServer(rChangePassword);
}

void Controller::ping(QString sToken)
{
    if(m_nManger.connectToHost())
    {
        Request rPing;
        rPing.sCommand = "ping";
        rPing.sToken = sToken;
        m_nManger.writeServer(rPing);
    }
}

UserInfo Controller::getUserInfo()
{
    return m_iLogin;
}





