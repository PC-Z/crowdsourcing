/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2019-08-01
Description:main函数，程序运行，MainWindow展示。
**************************************************/

#include "mainwindow.h"
#include <QApplication>
#include "userinfo.h"
#include "platform.h"
#include <QVector>
#include <QDebug>

int main(int argc, char *argv[])
{
    //QVector<UserInfo*> ALL_USERS = QVector<UserInfo *>();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
