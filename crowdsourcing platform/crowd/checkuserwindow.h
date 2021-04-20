#ifndef CHECKUSERWINDOW_H
#define CHECKUSERWINDOW_H

#include <QMainWindow>
#include "messagewindow.h"
//#include <QStandardItemModel>
#include <QMessageBox>

namespace Ui {
class CheckUserWindow;
}

class CheckUserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CheckUserWindow(QWidget *parent = 0);
    ~CheckUserWindow();
    //设置user表头
    void setCheckview();
    //设置server表头
    void setServerCheckview();
    //设置friend表头
    void setFriendCheckview();
    //辅助函数(server)
    void setServercheckuserModel(int row,QString &identity, int account);
    //辅助函数(friend)
    void setFriendcheckModel(int row, QString &identity ,int account);
    //辅助函数(user)
    void setcheckuserModel(int row,QString &pix,QString &identity, int account);
    //查看用户队列
    void checkuserview(QVector<UserInfo *> userlist);
    //查看好友队列
    void checkfriendview(QVector<UserInfo *> userlist);
    //Server查看用户
    void servercheckuserview(QVector<UserInfo *> userlist);
    //查看用户
    void checkuserfunction(QString &identity);

signals:

public slots:
    void Checkuser();

private slots:

    void Chooseleaduser();

    void Choosetranuser();

    void serverCheckuser();

    void on_sendmessagebutton_released();

    void on_addcommentbutton_released();

    void on_serverbackbutton_released();

    void on_serversetcreditbutton_released();

    void on_addfriendbutton_released();


private:
    Ui::CheckUserWindow *ui;
    QStandardItemModel *checkuserModel;
    QStandardItemModel *servercheckuserModel;
    QStandardItemModel *friendcheckModel;
    MessageWindow MesWin;

};

#endif // CHECKUSERWINDOW_H
