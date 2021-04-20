/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2019-08-01
Description:loginWindow是用户登录的窗口，将实现
            普通用户登录、平台Server登录等功能。
**************************************************/

#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "platform.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>

loginWindow::loginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);
    connect(ui->backButton,&QPushButton::clicked,this,&loginWindow::sendLogbackToMain);
}

loginWindow::~loginWindow()
{
    delete ui;
}

/*************************************************
Function:       sendLogbackToMain
Description:    发射信号的函数，用于从LoginWindow切换到MainWindow
Calls:          信号LogbackToMain()
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void loginWindow::sendLogbackToMain()
{
    emit LogbackToMain();
}

/*************************************************
Function:       loginWinRefresh
Description:    界面刷新函数包括清空输入框、设置输入格式
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void loginWindow::loginWinRefresh()
{
    ui->accountedit->clear();
    ui->psdedit->clear();
    ui->psdedit->setEchoMode(QLineEdit::Password);
    ui->pixlabel->setScaledContents(true);

    ui->serveraccountedit->clear();
    ui->serverpsdedit->clear();

}


/*************************************************
Function:       loginUser
Description:    切换到登录界面
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void loginWindow::loginUser()
{
    ui->loginstacked->setCurrentIndex(0);
}


/*************************************************
Function:       on_backButton_released
Description:    按下back按钮的槽函数，返回MainWindow
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void loginWindow::on_backButton_released()
{
    sendLogbackToMain();
    this->close();
}

/*************************************************
Function:       on_loginbutton_released
Description:    用户界面按下login按钮的槽函数
Calls:          UserInfo::userLogin(QString &,QString &)
                //进行账号密码检验
Input:          //
Output:         //
Return:         //
Others:         若检验正确则成功登陆，切换到PersonMainWindow
*************************************************/
void loginWindow::on_loginbutton_released()
{
    QString accountstr = ui->accountedit->text();
    QString psd = ui->psdedit->text();
    int find = UserInfo::userLogin(accountstr,psd);
    switch(find)
    {
    case NullAccount:
        QMessageBox::information(this,"prompt","Please Input UserAccount!");
        break;
    case NullPassword:
        QMessageBox::information(this,"prompt","Please Input UserPassword!");
        break;
    case Success:
        ui->pixlabel->setPixmap(QPixmap(NOW_USER->userpixadd));
        QMessageBox::information(this,"prompt","Successfully Loginned!");

        //切换到personal page
        PerWin.myPersonalshow();
        loginWinRefresh();
        this->close();
        break;
    case WrongPassword:
        QMessageBox::information(this,"prompt","Wrong Password!");
        ui->psdedit->clear();
        ui->psdedit->setFocus();
        break;
    case WrongAccount:
        QMessageBox::information(this,"prompt","Wrong UserAccount!");
        ui->accountedit->clear();
        ui->accountedit->setFocus();
        break;
    default:
        break;
    }
}

/***********************
 *
 *
 *   SERVER
 *
 *
**********************/
/*************************************************
Function:       loginServer
Description:    Server登录界面的刷新函数
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void loginWindow::loginServer()
{
    ui->loginstacked->setCurrentIndex(1);
    ui->serveraccountedit->clear();
    ui->serverpsdedit->clear();
    ui->serverpsdedit->setEchoMode(QLineEdit::Password);
}

/*************************************************
Function:       on_serverbackButton_released
Description:    Server登录界面返回MainWindow
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void loginWindow::on_serverbackButton_released()
{
    sendLogbackToMain();
    this->close();
}


/*************************************************
Function:       on_serverloginbutton_released
Description:    Server登录界面login按钮的槽函数，进行Server
                账号密码检验
Calls:          Server::serverLogin(QString &，QString &)
Input:          //
Output:         //
Return:         //
Others:         成功登录切换到CheckUserWindow查看所有用户列表
*************************************************/
void loginWindow::on_serverloginbutton_released()
{
    QString account = ui->serveraccountedit->text();
    QString psd = ui->serverpsdedit->text();
    bool islogin = Server::serverLogin(account,psd);
    if(islogin)
    {
        QMessageBox::information(this,"prompt","Successfully Login!");
        CheckUwin.show();
        CheckUwin.servercheckuserview(ALL_USERS);
        this->close();
    }
    else
    {
        QMessageBox::information(this,"prompt","Wrong Account Or Wrong Password!");
        ui->serveraccountedit->clear();
        ui->serverpsdedit->clear();
    }
}

