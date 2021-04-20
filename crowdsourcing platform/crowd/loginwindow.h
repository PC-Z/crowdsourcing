#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "personmainwindow.h"
#include "checkuserwindow.h"

namespace Ui {
class loginWindow;
}

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = 0);
    ~loginWindow();
    void sendLogbackToMain();
    //刷新login页面
    void loginWinRefresh();
    //用户界面
    void loginUser();
    //平台界面
    void loginServer();

signals:
    void LogbackToMain();

    //void LogforwardPersonal();

private slots:
    void on_backButton_released();

    void on_loginbutton_released();

    void on_serverbackButton_released();

    void on_serverloginbutton_released();


private:
    Ui::loginWindow *ui;
    PersonMainWindow PerWin;
    CheckUserWindow CheckUwin;
};

#endif // LOGINWINDOW_H
