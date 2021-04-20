#ifndef PERSONMAINWINDOW_H
#define PERSONMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QInputDialog>
#include "userinfo.h"
#include "messagewindow.h"
#include "mytaskwindow.h"
#include "posttaskwindow.h"
#include "checkuserwindow.h"


namespace Ui {
class PersonMainWindow;
}
//class UserInfo;
class PersonMainWindow : public QMainWindow
{
    Q_OBJECT

    friend void UserInfo::userchangeinfo(PersonMainWindow *a);
    //friend void UserInfo::userPostTask(PersonMainWindow *a);
public:
    explicit PersonMainWindow(QWidget *parent = 0);
    ~PersonMainWindow();
    //个人信息页面的一些设置
    void personWinSet();
    //新消息提醒
    void usernewmes();
    //好友申请
    void usernewfriends();
    //主页的设置
    //最新任务
    void homepageNewSet();
    //推荐任务
    void homepageRecSet();

signals:
    void PerToMain(QString a);


private slots:
    /*
    void on_backButton01_released();

    void on_backButton02_released();
    */
    void timeUpDate();

    void on_aboutuserButton_released();

    void on_pushButton_released();

    void on_abouttool_released();

    void on_QAtool_released();

    void on_psdconedit_editingFinished();

    void on_confirmbuttom_released();

    void on_pixchangebutton_released();

    void on_mymessage_released();

    void on_postbutton_released();

    void on_mytaskpushButton_released();

    void on_mypostButton_released();

    void on_myleadButton_released();

    void on_mytranButton_released();

    void on_logoutbutton_released();

    void on_tranferinbutton_released();

    void on_leadbutton_released();

    void on_tranbutton_released();

    void on_new_firstradiobutton_released();

    void on_new_secondradiobutton_released();

    void on_allbutton_released();

    void on_searchbutton_released();

    void on_myfriendbutton_released();

    void on_REC_firstradiobutton_released();

    void on_REC_secondradiobutton_released();

public slots:
    void myPersonalshow();

private:
    Ui::PersonMainWindow *ui;
    MessageWindow MesWin;
    postTaskWindow PostWin;
    mytaskWindow MytaskWin;
    CheckUserWindow CheckUWin;

};

#endif // PERSONMAINWINDOW_H
