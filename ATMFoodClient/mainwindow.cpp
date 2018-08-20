#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&Controller::getInstance(), &Controller::sigLoginFinished,
            this,&MainWindow::onLoginFinished);
    connect(&Controller::getInstance(), &Controller::sigError,
            this, &MainWindow::onError);
    connect(&Controller::getInstance(),&Controller::sErrLogout,
            this,&MainWindow::onLogOut);
    connect(&Controller::getInstance(),&Controller::sigErrLogout,
            this,&MainWindow::onErrLogout, Qt::UniqueConnection);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loginBtn_clicked()
{
    QString username = ui->usernameEdit->text().toLower();
    QString password = ui->passwordEdit->text();
    if(username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this,"Սխալ ինֆորմացիա","Սխալ է լրացվել մուտքանունը կամ գաղտնաբառը");
        return;
    }
    Controller::getInstance().login(username,password);
}

void MainWindow::onLoginFinished(bool bSucceeded, QString sErorrMessage, UserInfo m_info)
{
    if( bSucceeded )
    {
        this->hide();
        pWindow = new UserWindow(this);
        pWindow->setLoginInfo(m_info);
        pWindow->show();
        pWindow->generateUI();
    }
    else
    {
       QMessageBox::information(this,"Error",sErorrMessage);
    }
}

void MainWindow::onError(QString sMsg)
{
    QMessageBox::critical(this,"Error", sMsg);
}

void MainWindow::onErrLogout(QString sErrorMsg)
{
    QMessageBox::critical(this,"Error", sErrorMsg);
}

void MainWindow::onLogOut(bool bError)
{
    if(bError)
    {
        pWindow->hide();
        this->show();
        ui->usernameEdit->clear();
        ui->passwordEdit->clear();

        pWindow->deleteLater();
    }
    else
    {
        QMessageBox::critical(this,"Error","Տեղի է ունեցել սխալ");
    }
}

