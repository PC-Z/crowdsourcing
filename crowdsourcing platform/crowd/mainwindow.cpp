/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2019-08-01
Description:MainWindow是打开程序的主窗口，
            用户可以选择注册新账户、已有账号登录
            平台Server登录
**************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "platform.h"
#include "userinfo.h"
#include <Qstring>
#include <QDebug>
#include "registerwindow.h"
#include "sqlconnection.h"

QVector<Server *> ALL_SERVERS;
Server A_SERVER;
Server *NOW_SERVER = &A_SERVER;
QVector<UserInfo *> ALL_USERS;
QVector<TaskInfo *> ALL_TASKS;
QVector<SubTaskInfo *> ALL_SUBTASK;
QVector<Message *> ALL_MES;
QVector<UserInfo *> CHANGE_USERS;
QVector<TaskInfo *> CHANGE_TASKS;
QVector<SubTaskInfo *> CHANGE_SUB;
QVector<Message *> CHANGE_MES;
UserInfo A_USER ;
TaskInfo A_TASK ;
SubTaskInfo A_SUB;
UserInfo *NOW_USER = &A_USER;
TaskInfo *NOW_TASK = &A_TASK;
SubTaskInfo *NOW_SUB = &A_SUB;
QVector<Comment *> ALL_COMMENT;
QVector<Comment *> CHANGE_COMMENT;
QVector<userComment *> ALL_USERCOMMENTS;
QVector<userComment *> CHANGE_USERCOMMENTS;

int userComment:: count = 0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    createSQLConnection();
    putInQt_User();
    putInQt_Task();
    putInQt_Mes();
    putInQt_Tran();
    putInQt_Transign();
    putInQt_Leadsign();
    putInQt_SubTask();
    putInQt_Comment();
    putInQt_userComment();
    putInQt_Server();
    putInQt_Friend();
    putInQt_AddFriend();

    putMessageInVector();
    putSubtaskInVector();
    putCommentInSubtaskVector();
    putPostInVector();
    putLeadInVector();
    putTaskInVector();
    putuserCommentInUserVector();

    ui->setupUi(this);
    ui->loginlabel->setPixmap(QPixmap("://logo/loginlogo.png"));
    ui->loginlabel->setScaledContents(true);
    ui->registerlabel->setPixmap(QPixmap("://logo/regislogo.png"));
    ui->registerlabel->setScaledContents(true);
    ui->serverlabel->setPixmap(QPixmap("://logo/serverlogo.png"));
    ui->serverlabel->setScaledContents(true);
    connect(&LogWin,&loginWindow::LogbackToMain,this,&MainWindow::show);
    connect(&RegWin,&RegisterWindow::RegbackToMain,this,&MainWindow::show);
    connect(&PerMainWin,&PersonMainWindow::PerToMain,this,&MainWindow::PerBacktoMain);
}

MainWindow::~MainWindow()
{
    //数据存入数据库
    //createSQLConnection();
    putInsql_User();
    putInsql_Task();

    putInsql_Mes();
    putInsql_TaskandTran();
    putInsql_TaskandLeadsign();
    putInsql_TaskandTransign();
    putInsql_TaskandTran();
    putInsql_SubTask();
    putInsql_Comment();
    putInsql_userComment();
    putInsql_Friend();
    putInsql_AddFriend();
    qDebug()<<"bye~";

    for(int i=0;i<ALL_USERS.size();i++)
    {
        delete ALL_USERS[i];
    }
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        delete ALL_TASKS[i];
    }
    for(int i=0;i<ALL_MES.size();i++)
    {
        delete ALL_MES[i];
    }
    for(int i=0;i<ALL_SUBTASK.size();i++)
    {
        delete ALL_SUBTASK[i];
    }
    for(int i=0;i<ALL_COMMENT.size();i++)
    {
        delete ALL_COMMENT[i];
    }
    for(int i=0;i<ALL_USERCOMMENTS.size();i++)
    {
        delete ALL_USERCOMMENTS[i];
    }
    for(int i=0;i<ALL_SERVERS.size();i++)
    {
        delete ALL_SERVERS[i];
    }

    delete ui;

}

/*************************************************
Function:       on_registerButton_released
Description:    点击注册按钮的槽函数
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         切换到注册窗口RegisterWindow
*************************************************/
void MainWindow::on_registerButton_released()
{
    RegWin.show();
    RegWin.regisWinRefresh();
}

/*************************************************
Function:       on_loginButton_clicked
Description:    点击登录按钮的槽函数
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         切换到登录窗口LoginWindow
*************************************************/
void MainWindow::on_loginButton_clicked()
{
    //this->hide();
    LogWin.show();
    LogWin.loginWinRefresh();
    LogWin.loginUser();
}

/*************************************************
Function:       PerBacktoMain()
Description:    对PersonMainWindow所发射信号的槽函数
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         打开MainWindow
*************************************************/
void MainWindow::PerBacktoMain()
{
    this->show();
}

/*************************************************
Function:       on_serverbutton_released
Description:    点击Server按钮的槽函数
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         切换到Server登录窗口LoginWindow
*************************************************/
void MainWindow::on_serverbutton_released()
{
    LogWin.show();
    LogWin.loginWinRefresh();
    LogWin.loginServer();
}
