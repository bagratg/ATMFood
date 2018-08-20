#include "settingwindow.h"
#include "ui_settingwindow.h"

SettingWindow::SettingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);

}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::setUserInfo(UserInfo infUser)
{
    m_infUser = infUser;
}

void SettingWindow::on_SavePassChangeBtn_clicked()
{
    QString sLastPass =  ui->LastPass->text();
    QString sNewPass = ui->NewPass->text();
    QString sRePass = ui->RePass->text();

    if(sNewPass == sRePass && !sLastPass.isEmpty() && !sNewPass.isEmpty())
    {
          QString sNewPass = ui->NewPass->text();
          Controller::getInstance().changePassword(m_infUser.sUserId,m_infUser.sToken,sLastPass,sNewPass);
    }
    else
    {
        QMessageBox::warning(this,"Ուշադրություն","Տվյալները սխալ են լրացված !");
    }
}
