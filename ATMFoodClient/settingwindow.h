#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>
#include <controller.h>
#include <userinformation.h>

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = 0);
    ~SettingWindow();

public:
    void setUserInfo(UserInfo infUser);


private slots:
    void on_SavePassChangeBtn_clicked();

private:

    UserInfo m_infUser;

    Ui::SettingWindow *ui;
};

#endif // SETTINGWINDOW_H
