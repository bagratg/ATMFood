#include "controller.h"

Controller::Controller(){}
Controller::~Controller(){}



Response Controller::proccessCommand(const QJsonDocument &doc)
{
    QString sToken = doc.object().value("token").toString();
    QJsonObject oData = doc.object().value("data").toObject();
    QString sCommand =  doc.object().value("command").toString();

    if(!sCommand.isEmpty())
    {
       if(sCommand == "login")
            {
                QString username = oData.value("username").toString();
                QString password = oData.value("password").toString();
                return login(username,password);
            }

       if(session.checkToken(sToken))
        {
            if(sCommand == "ping")
            {
                return ping(sToken);
            }

            else if(sCommand == "logout")
            {
                 return logout(sToken);
            }

            else if(sCommand == "loadFoodData")
            {


                int iSkip =  oData.value("skip").toInt();
                QString sFilter = oData.value("filter").toString();
                QString sValue = oData.value("value").toString();
                int iItemCount = oData.value("count").toInt();

                return loadFoodData(iSkip,sFilter,sValue,iItemCount);
            }

            else if(sCommand == "createUser")
            {

                QString sUsername = oData.value("username").toString();
                QString sPassword = oData.value("password").toString();
                QString sRoom  = oData.value("room").toString();
                QString sFname = oData.value("fname").toString();
                QString sLname = oData.value("lname").toString();
                QString sCategory = oData.value("category").toString();
                return createUser( sUsername,
                                   sPassword,
                                   sRoom,
                                   sFname,
                                   sLname,
                                   sCategory,
                                   sToken);
            }

            else if(sCommand == "changePassword")
            {
                QString sLastPass = oData.value("lastPass").toString();
                QString sNewPass = oData.value("newPass").toString();
                QString sUserId = oData.value("userId").toString();

                return changePassword(sUserId,sLastPass, sNewPass);
            }

            else if(sCommand == "addFood")
            {

                QString sToken = doc.object().value("token").toString();
                QString sFoodName = oData.value("FoodName").toString();
                QString sFoodPrice  = oData.value("FoodPrice").toString();
                QString sFoodVendor  = oData.value("FoodVendor").toString();
                QString sFoodType  = oData.value("FoodType").toString();
                // Save Image

                return addFoodInfo(sFoodName,sFoodPrice,sFoodVendor,sFoodType);
            }

            else if(sCommand == "SubmitCart")
            {
                QList<OrderInfo> lCartInfo;
                auto vJson  = doc.object().value("data");
                if(vJson.isArray())
                {
                    QJsonArray oArray = vJson.toArray();
                    for(int i = 0; i < oArray.count(); i++)
                    {
                        if( oArray.at(i).isObject() )
                        {
                             QJsonObject oJson  = oArray.at(i).toObject();
                             OrderInfo tmp;
                             tmp.FoodCount = oJson.value("count").toString();
                             tmp.UserID = oJson.value("UserID").toString();
                             tmp.FoodID = oJson.value("FoodID").toString();
                             lCartInfo.append(tmp);
                        }
                    }
                }
                return handleOrder(lCartInfo);
            }

            else if(sCommand == "loadOrders")
            {
                return loadOrders();
            }

            else if(sCommand == "CanceleCart")
            {

                QString sUserID = oData.value("UserID").toString();
                return canceleOrder(sUserID);
            }

            else if(sCommand == "adminSubmited")
            {
                return adminSubmited(sToken);
            }

            else if(sCommand == "LoadUserList")
            {
                return loadUsers();
            }

            else if(sCommand == "loadUserCart")
            {

                QString sUserID = oData.value("UserID").toString();
                return loadUserCart(sUserID);
            }

            else if(sCommand == "deleteUser")
            {
                QString sUsername = oData.value("username").toString();
                return DeleteUser(sUsername);
            }

            else if(sCommand == "Loadhistory")
            {
                return loadOrdersHistory(sToken);
            }
        }
        else
        {
            Response tmp;
            tmp.sErrorText = "Տեղի է ունեցել մուտքի սխալ";
            tmp.sCommand = "ErrToken";
            tmp.oError  = false;
            return tmp;
        }
    }

    Response tmp;
    tmp.sCommand = "ErrCommand";
    tmp.sErrorText = "Չի գտնվել համապատասխան հրաման";
    tmp.oError  = true;
    return tmp;

}

Response Controller::login(QString sUsername, QString sPassword)
{
    Response tmp;
    tmp.sCommand = "login";
    tmp.oError = false;

    QSqlQuery q;


    q.prepare("select * from  user where username= :username and password= :password");
    q.bindValue(":username",sUsername);
    q.bindValue(":password",sPassword);

    if(q.exec())
    {
        if(q.next())
        {
            QJsonObject oJsonData;
            QString sUserID = q.value("id").toString();
            if(session.userLoggedIn(sUserID))
            {
                QString sToken = session.getToken(sUserID);
                session.deleteToken(sToken);
            }
            // Generate Token
            QString token = session.createSession(sUserID);

            oJsonData["token"] = token;
            oJsonData["id"] = q.value("id").toString();
            oJsonData["name"] = q.value("name").toString();
            oJsonData["lname"] = q.value("lname").toString();
            oJsonData["room"] = q.value("room").toString();
            oJsonData["category"] = q.value("category").toString();
            oJsonData["username"] = q.value("username").toString();

            QJsonDocument oJsonDoc(oJsonData);

            tmp.oData = oJsonDoc;
        }
        else
        {
            tmp.sErrorText = "սխալ օգտանուն կամ գաղտնաբառ";
        }
    }

    return tmp;

}

Response Controller::logout(QString sToken)
{
    Response temp;

    temp.sCommand = "logout";

    if (session.deleteToken(sToken))
    {
        temp.oError = false;
    }
    else
    {
        temp.oError = true;
    }

    return temp;
}

Response Controller::ping(QString sToken)
{
    Response tmp;
    int iCurentSecunde = QTime::currentTime().second();
    int iCurrentMinute = QTime::currentTime().minute() * 60;
    int iCurentTime  = iCurentSecunde + iCurrentMinute;

    session.dataChanged(sToken,iCurentTime);
    tmp.oError = false;
    tmp.sCommand = "ping";
    return tmp;
}

Response Controller::loadFoodData(int iSkip,QString sFilter,QString sValue,int iItemCount)
{
    Response tmp;
    tmp.oError = true;
    tmp.sCommand = "loadFoodData";
        QSqlQuery q;
        QSqlQuery qrySize;

        if(!sFilter.isEmpty())
        {
            // Bad linem Bada @!
            q.prepare("select * from food  where "+sFilter+" like :value order by name LIMIT :limit OFFSET :offset");
            q.bindValue(":value","%"+sValue+"%");

            qrySize.prepare("select count(*) FROM food  where "+sFilter+" like :value");
            qrySize.bindValue(":value","%"+sValue+"%");
        }

        else
        {
            q.prepare("select * from food order by name LIMIT :limit OFFSET :offset");
            qrySize.prepare("select count(*) FROM food");

        }

        q.bindValue(":offset",iSkip);
        q.bindValue(":limit",iItemCount);

        q.exec();
        qrySize.exec();

        qrySize.next();
        int iMsgSize =  qrySize.value("count(*)").toInt();

        QJsonArray oArray;
        while(q.next())
        {
            QJsonObject oJsonData;

            QString sID = q.value("id").toString();
            oJsonData["id"] = sID;
            QString sName = q.value("name").toString();
            oJsonData["name"] = sName;
            QString sType = q.value("type").toString();
            oJsonData["type"] = sType;
            QString sVendor = q.value("vendor").toString();
            oJsonData["vendor"] = sVendor;
            QString sPrice = q.value("price").toString();
            oJsonData["price"] = sPrice;
            QString path = q.value("imgPath").toString();

            // load image
            QString sAppDataPath = QStandardPaths::writableLocation( QStandardPaths::DataLocation );
            QString sImgPath = sAppDataPath + QDir::separator()+path;

            QFile* file = new QFile(sImgPath);
            file->open(QIODevice::ReadOnly);

            QByteArray aImage = file->readAll();
            QString sEncoded = QString(aImage.toBase64());
            file->close();
            oJsonData["image"] = sEncoded;

            // add array
            oArray.append(oJsonData);


        }

        if(oArray.isEmpty())
        {
            tmp.sErrorText = "Արդյունքներ չեն գտնվել";
        }

        QJsonDocument oJsonDoc(oArray);
        tmp.oError = false;
        tmp.oData = oJsonDoc;
        tmp.iMsgSize = iMsgSize;



    return tmp;
}

Response Controller::handleOrder(QList<OrderInfo> sCardList)
{
    Response tmp;
    tmp.sCommand = "SubmitCart";

        QSqlQuery q;
        for(int i = 0; i < sCardList.size();i++)
        {
            q.prepare("insert into cart (FoodId,UserId,count) VALUES(:FoodID,:UserID,:Count)");
            q.bindValue(":UserID",sCardList[i].UserID);
            q.bindValue(":FoodID",sCardList[i].FoodID);
            q.bindValue(":Count",sCardList[i].FoodCount);
            q.exec();
        }

    tmp.oError = false;
    return tmp;
}

Response Controller::loadOrders()
{
    Response tmp;
    tmp.sCommand = "loadOrders";
    tmp.oError = true;
    QJsonObject result;

    QSqlQuery q;
    q.prepare("select * from cart");
    q.exec();

    while(q.next())
    {
        QString sFoodId = q.value("FoodId").toString();
        QString sUserId = q.value("Userid").toString();
        QString sCount = q.value("count").toString();

        if (! result.contains(sUserId))
        {
            result[sUserId] = QJsonArray();
        }

        QJsonObject oFood;

        // Get UserName
        QSqlQuery qryUserInfo;
        qryUserInfo.prepare("select * from user where id = :UserID");
        qryUserInfo.bindValue(":UserID",sUserId);
        qryUserInfo.exec();
        qryUserInfo.next();

        QString sUserName = qryUserInfo.value("username").toString();

        // create Order Info

        QJsonObject orderInfo;
        orderInfo["username"] = sUserName;
        orderInfo["count"] = sCount;

        // Graph Food info
        QSqlQuery qry;
        qry.prepare("select * from food where id = :ID ");
        qry.bindValue(":ID",sFoodId);
        qry.exec();
        qry.next();

//        QString sIDdd = qry.value("id").toString();
//        QString sssfoodName = qry.value("name").toString();

        QString sID = qry.value("id").toString();
        oFood["id"] = sID;

        QString sfoodName = qry.value("name").toString();
        oFood["name"] = sfoodName;

        QString sType = qry.value("type").toString();
        oFood["type"] = sType;

        QString sVendor = qry.value("vendor").toString();
        oFood["vendor"] = sVendor;

        QString sPrice = qry.value("price").toString();
        oFood["price"] = sPrice;

      //  QString sImgPath = qry.value("imgPath").toString();



        // Add Food Data JsonObject
        orderInfo["food"] = oFood;


       auto tmpArr = result[sUserId].toArray();
       tmpArr.push_back(orderInfo);
       result[sUserId] = tmpArr;
       tmp.oError = false;
    }
//        db.getOrders();
//        Order ord;
//        ord.user

    QJsonDocument oJsonDoc(result);

    tmp.oData = oJsonDoc;

    return tmp;

}

Response Controller::loadUsers()
{
     Response tmp;
     tmp.sCommand = "LoadUserList";

     QSqlQuery q;
     q.exec("select * from user");
     QJsonArray aResult;

     while (q.next())
     {
       QJsonObject oJson;

       QString sName = q.value("name").toString();
       oJson["name"] = sName;

       QString sUserName  = q.value("username").toString();
       oJson["username"] = sUserName;

       QString sCategory;
       if(q.value("category").toString() == "1")
       {
           sCategory = "Ադմինիստրատոր";
       }
       else
       {
           sCategory = "Օգտատեր";
       }
       oJson["category"] = sCategory;

       aResult.append(oJson);

     }

     QJsonDocument oDat(aResult);
     tmp.oData = oDat;
     tmp.oError = false;
     return tmp;

}

Response Controller::DeleteUser(QString sUsername)
{
    Response tmp;

    tmp.sCommand = "deleteUser";
    QSqlQuery q;
    q.prepare("delete  from user where username = :user");
    q.bindValue(":user",sUsername);
    q.exec();
    q.next();

    tmp.oError = false;

    return tmp;
}

Response Controller::changePassword(QString sUserID, QString sLastPass, QString sNewPass)
{
    Response tmp;

    tmp.sCommand = "changePassword";
    tmp.oError = false;

    QSqlQuery q;
    q.prepare("select*  from user where id = :id");
    q.bindValue(":id",sUserID);
    q.exec();
    q.next();

    QString sPassword = q.value("password").toString();
    if(sPassword != sLastPass)
    {
       tmp.sErrorText = "Սխալ Գաղտնաբառ";
       return tmp;
    }

    QSqlQuery qry;

    qry.prepare("UPDATE user SET password = :pass WHERE id= :id");
    qry.bindValue(":id",sUserID);
    qry.bindValue(":pass",sNewPass);
    qry.exec();
    qry.next();

    return tmp;
}

Response Controller::loadUserCart(QString sUserID)
{

    Response tmp;

    tmp.sCommand = "loadUserCart";
    tmp.oError = true;

        // selcect user Foods User ID
        QSqlQuery q;
        q.prepare("select * from cart where UserID = :UserID ");
        q.bindValue(":UserID",sUserID);
        q.exec();


        QJsonArray oArray;

        while (q.next())
        {
          QString sFoodID = q.value("FoodId").toString();
          int iCount = q.value("count").toInt();
          QSqlQuery qryFood;
          qryFood.prepare("select * from food where id = :foodId ");
          qryFood.bindValue(":foodId",sFoodID);
          qryFood.exec();


          while (qryFood.next())
          {
              QJsonObject oFood;

              QString sID = qryFood.value("id").toString();
              oFood["id"] = sID;

              QString sfoodName = qryFood.value("name").toString();
              oFood["name"] = sfoodName;

              QString sType = qryFood.value("type").toString();
              oFood["type"] = sType;

              QString sVendor = qryFood.value("vendor").toString();
              oFood["vendor"] = sVendor;

              QString sPrice = qryFood.value("price").toString();
              oFood["price"] = sPrice;

              QString path = qryFood.value("imgPath").toString();

              // load image

              QString sAppDataPath = QStandardPaths::writableLocation( QStandardPaths::DataLocation );
              QString sImgPath = sAppDataPath + QDir::separator()+path;

              QFile* file = new QFile(sImgPath);
              file->open(QIODevice::ReadOnly);

              QByteArray aImage = file->readAll();
              QString sEncoded = QString(aImage.toBase64());
              file->close();
              oFood["image"] = sEncoded;


              // add Array

              for(int i = 0 ; i < iCount; i++)
              {
                  oArray.append(oFood);
              }

           }

        }

    QJsonDocument oDat(oArray);
    tmp.oData = oDat;
    tmp.oError = false;
    return tmp;

}

Response Controller::canceleOrder(QString sUserID)
{
    Response tmp;

    tmp.sCommand = "CanceleCart";
    tmp.oError = false;
    QSqlQuery qryChecker;
    qryChecker.exec("select * from cart");

    if(!qryChecker.next())
    {
        tmp.sErrorText = "Հարգելի օգտատեր,ադմինիստրատորը արդեն հաստատել է պատվերը, Դուք կարող եք նորից պատվիրել";
    }
    else
    {
        QSqlQuery q;
        q.prepare("delete  from cart where UserID = :UserID ");
        q.bindValue(":UserID",sUserID);
        q.exec();
    }

    return tmp;
}

Response Controller::createUser(QString sUsername,
                                QString sPassword,
                                QString sRoom,
                                QString sFname,
                                QString sLname,
                                QString sCategory,
                                QString sToken)
{

    Response tmp;
    tmp.oError = false;

    tmp.sCommand = "createUser";
        QSqlQuery qry;
        qry.prepare("select username from  user where username = :username");
        qry.bindValue(":username",sUsername);
        qry.exec();
        if(qry.next())
        {
            tmp.oError = true;
            return tmp;
        }

        QSqlQuery q;
        q.prepare("insert into  user (name,lname,room,category,username,password) "
                   "values ( "
                   ":Fname ,"
                   ":Lname ,"
                   ":Room ,"
                   ":Category ,"
                   ":Username ,"
                   ":Password )");

         q.bindValue(":Fname",sFname);
         q.bindValue(":Lname",sLname);
         q.bindValue(":Room",sRoom);
         q.bindValue(":Category",sCategory);
         q.bindValue(":Username",sUsername);
         q.bindValue(":Password",sPassword);
         if(!q.exec())
         {
            tmp.sErrorText = "Տեղի է ունեցել սխալ";
         }

    return tmp;

}

Response Controller::addFoodInfo(QString sFoodName,
                                 QString sFoodPrice,
                                 QString sFoodVendor,
                                 QString sFoodType
                                )
{
    Response tmp;
    tmp.sCommand = "addFood";

     QSqlQuery q;
     q.prepare("insert into  food (name,type,vendor,price) "
               "values ( "
               ":foodName ,"
               ":foodType ,"
               ":foodVendor ,"
               ":foodPrice )");

     q.bindValue(":foodName",sFoodName);
     q.bindValue(":foodType",sFoodType);
     q.bindValue(":foodVendor",sFoodVendor);
     q.bindValue(":foodPrice",sFoodPrice);
     if(q.exec())
     {
         tmp.sErrorText = "Error";
     }

     tmp.oError = false;


    return tmp;

}

Response Controller::adminSubmited(QString sToken)
{
    Response tmp;
    tmp.sCommand = "adminSubmited";
    tmp.oError = true;
    if(session.checkToken(sToken))
    {
        QSqlQuery q;
        q.exec(
               "INSERT INTO ordershistory (foodid, userid, count, date)"
               " SELECT FoodId, UserId,count, datetime('now') as date FROM cart"
               );
        q.exec("delete from cart");
        tmp.oError = false;
    }
    return tmp;


}

Response Controller::loadOrdersHistory(QString sToken)
{

    Response tmp;
    tmp.oError = true;
    tmp.sCommand = "loadOrdersHistory";

    if(! session.checkToken(sToken))
    {
       return tmp;
    }

    QJsonObject result;
    QJsonObject finalResult;

    QSqlQuery q;
    q.prepare("select * from ordershistory order by date,userid");
    q.exec();


    QString sTmpDate;
    int idx = 0;

    while(q.next())
    {

        QString sFoodId = q.value("foodid").toString();
        QString sUserId = q.value("userid").toString();
        QString sCount = q.value("count").toString();
        QString sDate = q.value("date").toString();

        if(!finalResult.contains(sDate) ||  idx == q.size() - 1)
        {

            if(idx != 0)
            {
                auto tmpFinalArray = finalResult[sTmpDate].toArray();
                tmpFinalArray.push_back(result);
                finalResult[sTmpDate] = tmpFinalArray;
            }
            if(idx != q.size() -1)
            {
                finalResult[sDate] = QJsonArray();
                result = QJsonObject();
            }

            sTmpDate = sDate;

        }

        QJsonObject oFood;

        // Get UserName
        QSqlQuery qryUserInfo;
        qryUserInfo.prepare("select username from user where id = :UserID");
        qryUserInfo.bindValue(":UserID",sUserId);
        qryUserInfo.exec();
        qryUserInfo.next();

        QString sUserName = qryUserInfo.value("username").toString();

        // create Order Info

        QJsonObject orderInfo;
        orderInfo["username"] = sUserName;
        orderInfo["count"] = sCount;


        // Graph Food info
        QSqlQuery qry;
        qry.prepare("select * from food where id = :ID ");
        qry.bindValue(":ID",sFoodId);
        qry.exec();
        qry.next();

        QString sID = qry.value("id").toString();
        oFood["id"] = sID;

        QString sfoodName = qry.value("name").toString();
        oFood["name"] = sfoodName;

        QString sType = qry.value("type").toString();
        oFood["type"] = sType;

        QString sVendor = qry.value("vendor").toString();
        oFood["vendor"] = sVendor;

        QString sPrice = qry.value("price").toString();
        oFood["price"] = sPrice;


//        QString path = qry.value("imgPath").toString();

//        // load image

//        QString sAppDataPath = QStandardPaths::writableLocation( QStandardPaths::DataLocation );
//        QString sImgPath = sAppDataPath + QDir::separator()+path;
//        QFile* file = new QFile(sImgPath);
//        file->open(QIODevice::ReadOnly);
//        QByteArray aImage = file->readAll();
//        QString sEncoded = QString(aImage.toBase64());
//        file->close();
//        oFood["image"] = sEncoded;


        // Add Food Data JsonObject
        orderInfo["food"] = oFood;


        if(!result.contains(sUserId))
        {
            result[sUserId] = QJsonArray();
        }

        auto tmpArray = result[sUserId].toArray();
        tmpArray.push_back(orderInfo);
        result[sUserId] =  tmpArray;

        tmp.oError = false;

        idx ++;
    }
    auto tmpFinalArray = finalResult[sTmpDate].toArray();
    tmpFinalArray.push_back(result);
    finalResult[sTmpDate] = tmpFinalArray;

    QJsonDocument oJsonDoc(finalResult);
    tmp.oData = oJsonDoc;

    return tmp;
}
