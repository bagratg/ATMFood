#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"
#include "userwindow.h"
#include <QObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_loginBtn_clicked();
    void onLoginFinished(bool bSucceeded, QString sErorrMessage,UserInfo m_info);
    void onError( QString sMsg );

    void onErrLogout(QString sErrorMsg);

    void onLogOut(bool bError);

private:
    Ui::MainWindow *ui;
    UserWindow* pWindow;

};

#endif // MAINWINDOW_H
