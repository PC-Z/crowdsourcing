/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2019-08-01
Description:CheckUserWindow是查看用户的窗口，将实现
            普通用户查询其他用户列表、
            平台Server查询所有用户、
            单独查看某用户信息等功能。
**************************************************/

#include "checkuserwindow.h"
#include "ui_checkuserwindow.h"
#include "platform.h"

/*************************************************
Function:       CheckUserWindow
Description:    CheckUserWindow构造函数
Calls:          setCheckview//设置用户列表表头数据
                ui->setupUi(this)//呈现ui界面
Others:         构造函数中的setCheckview主要是设置表头
                为后续打印数据做准备
*************************************************/
CheckUserWindow::CheckUserWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CheckUserWindow)
{
    ui->setupUi(this);
    setCheckview();
}

CheckUserWindow::~CheckUserWindow()
{
    delete ui;
}


/***********************
 *
 *
 *   SERVER
 *
 *
**********************/

/*************************************************
Function:       setCheckview
Description:    设置普通用户查询用户列表表头函数
                表头依次为头像（pix），昵称（identity）
                账号（account），检查按钮（check），
                选择按钮（choose）
Calls:          //
Others:         平台Server表头设置函数另有设计，
                选择按钮仅对可以确认负责人的任务发布者、
                以及可以确认译者的任务负责人有效
*************************************************/
void CheckUserWindow::setCheckview()
{
    checkuserModel = new QStandardItemModel;
    checkuserModel->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("Pix")));
    checkuserModel->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("Identity")));
    checkuserModel->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("Account")));
    checkuserModel->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("Check")));
    checkuserModel->setHorizontalHeaderItem(4,new QStandardItem(QObject::tr("Choose")));
    ui->check_tableview->setModel(checkuserModel);
    ui->check_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->check_tableview->setColumnWidth(0,100);
    ui->check_tableview->setColumnWidth(1,100);
    ui->check_tableview->setColumnWidth(2,100);
    ui->check_tableview->setColumnWidth(3,100);
    ui->check_tableview->setColumnWidth(4,100);
}


/*************************************************
Function:       setcheckuserModel
Description:    根据输入的参数建立表格，此函数旨在将重复
                的操作（即表格插入数据）集合成一个函数，
                从而避免代码冗杂
Calls:          UserInfo::userFind(QVector<UserInfo *> , QString)
                TaskInfo::getTaskstate()
Input:          参数row，为int类型，表示插入的行数
                参数pix，为QString类型，表示用户头像文件地址
                在函数体中会转化成QPixmap和QIcon插入表格
                参数identity，为QString类型，表示用户昵称（唯一标识）
                参数account，为int类型，表示系统为用户匹配的账号
Output:         //
Return:         //
Others:         本函数配合相应的用户列表打印函数进行
*************************************************/
void CheckUserWindow::setcheckuserModel(int row, QString &pix, QString &identity, int account)
{
    QPixmap ppix = QPixmap(pix);
    QIcon ipix = QIcon(ppix);
    checkuserModel->setItem(row,0,new QStandardItem(ipix,"pix"));
    checkuserModel->setItem(row,1,new QStandardItem(identity));
    checkuserModel->setItem(row,2,new QStandardItem(QString("%1").arg(account, 6, 10, QChar('0'))));
    //设置查看按钮
    QPushButton *button = new QPushButton("Check");
    button->setProperty("pix",pix);
    button->setProperty("identity",identity);
    button->setProperty("account",account);
    button->setProperty("row",row);
    connect(button, &QPushButton::released, this, &CheckUserWindow::Checkuser);
    ui->check_tableview->setIndexWidget
            (checkuserModel->index(checkuserModel->rowCount()-1,3), button);
    //设置查看按钮
    //设置选择按钮
    QPushButton *buttons = new QPushButton("Choose");
    buttons->setProperty("pix",pix);
    buttons->setProperty("identity",identity);
    buttons->setProperty("account",account);
    buttons->setProperty("row",row);
    if((NOW_TASK->taskLeader))
    {
        if(identity == NOW_TASK->taskLeader->userIdentity)
        {
            buttons->setEnabled(false);
        }
        else
        {
            bool istran = false;
            for(int i=0;i<NOW_TASK->taskTranslater.size();i++)
            {
                if(NOW_TASK->taskTranslater[i]->userIdentity == identity)
                {
                    istran = true;
                    buttons->setEnabled(false);
                    break;
                }
            }
            if(!istran)
            {
                        UserInfo *chosen = UserInfo::userFind(NOW_TASK->leadSignup,identity);

                        if(chosen)
                        {
                            if(NOW_TASK->getTaskstate() == WaitingLeader)
                            {
                                connect(buttons, &QPushButton::released, this, &CheckUserWindow::Chooseleaduser);
                            }
                            else
                            {
                                buttons->setEnabled(false);
                            }
                        }
                        //选择译者
                        chosen = UserInfo::userFind(NOW_TASK->tranSignup,identity);
                        if(chosen)
                        {
                            if(NOW_TASK->getTaskstate() == WaitingTranslater)
                            {
                                connect(buttons, &QPushButton::released, this, &CheckUserWindow::Choosetranuser);
                            }
                            else
                            {
                                buttons->setEnabled(false);
                            }
                        }
            }
        }

    }
    else
    {
        //还没有负责人
        UserInfo *chosen = UserInfo::userFind(NOW_TASK->leadSignup,identity);
        if(chosen)
        {
            if(NOW_TASK->getTaskstate() == WaitingLeader)
            {
                connect(buttons, &QPushButton::released, this, &CheckUserWindow::Chooseleaduser);

            }
            else
            {
                buttons->setEnabled(false);
            }
        }
        //选择译者
        chosen = UserInfo::userFind(NOW_TASK->tranSignup,identity);
        if(chosen)
        {
            if(NOW_TASK->getTaskstate() == WaitingTranslater)
            {
                connect(buttons, &QPushButton::released, this, &CheckUserWindow::Choosetranuser);
            }
            else
            {
                buttons->setEnabled(false);
            }
        }
    }




    ui->check_tableview->setIndexWidget
            (checkuserModel->index(checkuserModel->rowCount()-1,4), buttons);
    //设置选择按钮
}


/*************************************************
Function:       checkuserview
Description:    根据输入的userinfo数据，打印出其中所有用户的
                头像、昵称、账号信息，并且所有用户可以通过check按钮
                查看用户更多基本信息，若所查看的用户列表为负责人报名人列表
                且点击choose的用户为可以确认负责人的任务发布者，
                则可以选择该用户为负责人。若所查看的用户列表为译者报名人列表
                且点击choose的用户为可以确认译者的任务负责人，
                则可以选择该用户为译者。
Calls:          CheckUserWindow::setCheckview()
                CheckUserWindow::setcheckuserModel(int row, QString &pix,
                                            QString &identity, int account)
Input:          参数QVector<UserInfo *> userlist，为QVector类型，
                表示需要查看的用户数组
Output:         //
Return:         //
Others:         本函数配合相应的辅助插入数据函数进行
*************************************************/
void CheckUserWindow::checkuserview(QVector<UserInfo *> userlist)
{
    ui->checkstacked->setCurrentIndex(0);
    setCheckview();
    checkuserModel->removeRows(0,checkuserModel->rowCount());
    for(int i=0;i<userlist.size();i++)
    {
        QString pix = userlist[i]->userpixadd;
        QString identity = userlist[i]->userIdentity;
        int account = userlist[i]->userAccount;
        setcheckuserModel(i,pix,identity,account);
    }
}

/*************************************************
Function:       Checkuser
Description:    槽函数，查看所点击按钮的用户基本信息
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void CheckUserWindow::Checkuser()
{
    ui->checkstacked->setCurrentIndex(1);
    QPushButton *button = (QPushButton *)sender();
    QString identity = button->property("identity").toString();
    UserInfo *tempcheck = UserInfo::userFind(ALL_USERS,identity);
    if(tempcheck)
    {
        ui->pixlabel->setPixmap(QPixmap( tempcheck->userpixadd));
        ui->pixlabel->setScaledContents(true);
        ui->accountlabel->setText(QString("%1").arg(tempcheck->userAccount, 6, 10, QChar('0')));
        ui->identitylabell->setText(tempcheck->userIdentity);
        ui->birthdayread->setText(tempcheck->userBirthday.toString("yyyy-MM-dd"));
        ui->sexread->setText(tempcheck->userSex);
        ui->creditread->setText(QString::number( tempcheck->userCredit));
        ui->userleadnum->setText(QString::number(tempcheck->userleadnum));
        ui->usertrannum->setText(QString::number(tempcheck->usertrannum));
        ui->qualificationbrowser->setText(tempcheck->userQualification);
        ui->addcommentedit->clear();

        if(tempcheck == NOW_USER)
        {
            ui->addcommentbutton->hide();
            ui->addcommentedit->hide();
        }
        else
        {
            ui->addcommentbutton->show();
            ui->addcommentedit->show();
        }

        ui->addcommentbutton->setProperty("checkidentity",tempcheck->userIdentity);
        ui->addfriendbutton->setProperty("checkidentity",tempcheck->userIdentity);
        QString comments;
        for(int i=0;i<tempcheck->userComments.size();i++)
        {
            comments += tempcheck->userComments[i]->commentContent
                    +"\n**********\n";
        }
            ui->commentbrowser->setText(comments);
    }
}


/*************************************************
Function:       checkuserfunction
Description:    单独用于查看用户函数，以弥补上述check按钮槽函数
                所不能覆盖的情况
Calls:          UserInfo::userFind(QVector<UserInfo *>,QString &)
Input:          参数identity，为QString类型，表示想要查看用户的昵称（唯一标识）
Output:         //
Return:         //
Others:         //
*************************************************/
void CheckUserWindow::checkuserfunction(QString &identity)
{
    ui->checkstacked->setCurrentIndex(1);
    UserInfo *tempcheck = UserInfo::userFind(ALL_USERS,identity);
    if(tempcheck)
    {
        ui->pixlabel->setPixmap(QPixmap(tempcheck->userpixadd));
        ui->pixlabel->setScaledContents(true);
        ui->accountlabel->setText(QString("%1").arg(tempcheck->userAccount, 6, 10, QChar('0')));

        ui->identitylabell->setText(tempcheck->userIdentity);
        ui->identitylabell->adjustSize();
        ui->birthdayread->setText(tempcheck->userBirthday.toString("yyyy-MM-dd"));
        ui->birthdayread->adjustSize();
        ui->sexread->setText(tempcheck->userSex);
        ui->creditread->setText(QString::number( tempcheck->userCredit));

        if(tempcheck == NOW_USER)
        {
            ui->addcommentbutton->hide();
            ui->addcommentedit->hide();
        }
        else
        {
            ui->addcommentbutton->show();
            ui->addcommentedit->show();
        }


        ui->userleadnum->setText(QString::number(tempcheck->userleadnum));
        ui->userleadnum->setReadOnly(true);
        ui->usertrannum->setText(QString::number(tempcheck->usertrannum));
        ui->usertrannum->setReadOnly(true);
        ui->qualificationbrowser->setText(tempcheck->userQualification);

        ui->addcommentbutton->setProperty("checkidentity",tempcheck->userIdentity);
        ui->addfriendbutton->setProperty("checkidentity",tempcheck->userIdentity);
        QString comments;
        for(int i=0;i<tempcheck->userComments.size();i++)
        {
            comments += tempcheck->userComments[i]->commentContent
                    +"\n**********\n";
        }
            ui->commentbrowser->setText(comments);
    }
}


/*************************************************
Function:       Chooseleaduser
Description:    点击choose按钮的槽函数，对应选择该用户为负责人
Calls:          UserInfo::userFind(QVector<UserInfo *>,QString &)
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void CheckUserWindow::Chooseleaduser()
{
    QPushButton *button = (QPushButton *)sender();
    QString identity = button->property("identity").toString();
    UserInfo *chosen = UserInfo::userFind(ALL_USERS,identity);
    u_POST *nowposter = new u_POST(*NOW_USER);
    nowposter->userUPDATE(&CHANGE_USERS);
    nowposter->userUPDATE(&ALL_USERS);
    nowposter->userUPDATE(NOW_TASK->taskPoster);
    NOW_USER = UserInfo::userFind(ALL_USERS,nowposter->userIdentity);
    bool isdecide;
    if(chosen)
    {
        isdecide = nowposter->p_userDecideLeader(NOW_TASK,chosen,0);

        if(isdecide)
        {
            QMessageBox::information(this,"prompt","Successfully Choose The Leader!");
        }
        else
        {
            QMessageBox::StandardButton result;
            result = QMessageBox::question(this,"question","Want To Decide The Leader Before Deadline?",
                                  QMessageBox::Yes|QMessageBox::No);
            if(result == QMessageBox::Yes)
            {
                isdecide = nowposter->p_userDecideLeader(NOW_TASK,chosen,1);
            }

        }
        if(isdecide)
        {
            button->setEnabled(false);
        }
        NOW_TASK->taskUPDATE(&CHANGE_TASKS);

    }
    else
    {
        qDebug()<<"checkuserwin:"<<"no leadsignup";
    }
    checkuserview(NOW_TASK->leadSignup);
}

/*************************************************
Function:       Chooseleaduser
Description:    点击choose按钮的槽函数，对应选择该用户为译者
Calls:          UserInfo::userFind(QVector<UserInfo *>,QString &)
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void CheckUserWindow::Choosetranuser()
{
    QPushButton *button = (QPushButton *)sender();
    QString identity = button->property("identity").toString();
    UserInfo *chosen = UserInfo::userFind(ALL_USERS,identity);
    u_LEADER *nowleader = new u_LEADER(*NOW_USER);
    NOW_USER = nowleader;
    bool isdecide = false;
    if(chosen)
    {
        //在招聘译者阶段点击选择
        isdecide = nowleader->l_userDecideTran(NOW_TASK,chosen,0);
        if(isdecide)
        {
            QMessageBox::information(this,"prompt","Successfully Choose The Transalter!");
        }
        else
        {
            QMessageBox::StandardButton result;
            result = QMessageBox::question(this,"question","Want To Decide The Translater Before Deadline?",
                                  QMessageBox::Yes|QMessageBox::No);
            if(result == QMessageBox::Yes)
            {
                isdecide = nowleader->l_userDecideTran(NOW_TASK,chosen,1);
                chosen->userUPDATE(&CHANGE_USERS);

            }

        }
        if(isdecide)
        {
            button->setEnabled(false);
        }
        NOW_TASK->taskUPDATE(&CHANGE_TASKS);
    }
    else
    {
        qDebug()<<"checkuserwin:"<<"no transignup";
    }
}


/*************************************************
Function:       on_sendmessagebutton_released
Description:    点击sendmessage按钮的槽函数，向当前页面的用户发送消息
Calls:          MessageWindow::messendfromCheckWin(QString &)
Input:          //
Output:         //
Return:         //
Others:         函数跳转到消息窗口发送消息
*************************************************/
void CheckUserWindow::on_sendmessagebutton_released()
{
    MesWin.show();
    QString receiver = ui->identitylabell->text();
    MesWin.messendfromCheckWin(receiver);

}

/*************************************************
Function:       on_sendmessagebutton_released
Description:    点击addcomment按钮的槽函数，评价当前页面的用户
Calls:          UserInfo::userFind(QVector<UserInfo *>,QString &)
                UserInfo::adduserComments(UserInfo *target, QString &comment)
Input:          //
Output:         //
Return:         //
Others:         //评价后的消息后直接打印在评价框屏幕上
*************************************************/
void CheckUserWindow::on_addcommentbutton_released()
{
    QString comment = ui->addcommentedit->toPlainText();
    if(comment.isEmpty())
    {
        QMessageBox::information(this,"prompt","Please Input Your Comment!");
    }
    else
    {
        QPushButton *button = (QPushButton *)sender();
        QString identity = button->property("checkidentity").toString();
        UserInfo *tempcheck = UserInfo::userFind(ALL_USERS,identity);

        NOW_USER->adduserComments(tempcheck,comment);
        if(tempcheck)
        {
            QString comments;
            for(int i=0;i<tempcheck->userComments.size();i++)
            {
                comments += tempcheck->userComments[i]->commentContent
                        +"\n**********\n";
            }
                ui->commentbrowser->setText(comments);
        }
    }
}

/***********************
 *
 *
 *   FRIEND
 *
 *
**********************/

/*************************************************
Function:       setServerCheckview
Description:    设置平台Server查询用户列表表头函数
                表头依次为昵称（identity），
                账号（account），检查按钮（check），
Calls:          //
Others:         通过检查按钮，Server可以更改用户的积分值
*************************************************/
void CheckUserWindow::setFriendCheckview()
{
    friendcheckModel = new QStandardItemModel;
    friendcheckModel->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("Identity")));
    friendcheckModel->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("Account")));
    friendcheckModel->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("Check")));
    ui->check_tableview->setModel(friendcheckModel);
    ui->check_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->check_tableview->setColumnWidth(0,100);
    ui->check_tableview->setColumnWidth(1,100);
    ui->check_tableview->setColumnWidth(2,100);
}

void CheckUserWindow::checkfriendview(QVector<UserInfo *> userlist)
{
    ui->checkstacked->setCurrentIndex(0);
    setFriendCheckview();
    friendcheckModel->removeRows(0,friendcheckModel->rowCount());
    for(int i=0;i<userlist.size();i++)
    {

        QString identity = userlist[i]->userIdentity;
        int account = userlist[i]->userAccount;
        qDebug()<<"friend1"<<identity<<account;
        setFriendcheckModel(i,identity,account);
        qDebug()<<"friend2"<<identity<<account;
    }
}

void CheckUserWindow::setFriendcheckModel(int row, QString &identity ,int account)
{
    friendcheckModel->setItem(row,0,new QStandardItem(identity));
    QString caccount = QString("%1").arg(account, 6, 10, QChar('0'));
    friendcheckModel->setItem(row,1,new QStandardItem(caccount));
    //设置查看按钮
    QPushButton *button = new QPushButton("Check");
    button->setProperty("identity",identity);
    button->setProperty("account",account);
    button->setProperty("row",row);
    connect(button, &QPushButton::released, this, &CheckUserWindow::Checkuser);
    ui->check_tableview->setIndexWidget
            (friendcheckModel->index(friendcheckModel->rowCount()-1,2), button);
}

/***********************
 *
 *
 *   SERVER
 *
 *
**********************/

/*************************************************
Function:       setServerCheckview
Description:    设置平台Server查询用户列表表头函数
                表头依次为昵称（identity），
                账号（account），检查按钮（check），
Calls:          //
Others:         通过检查按钮，Server可以更改用户的积分值
*************************************************/
void CheckUserWindow::setServerCheckview()
{
    servercheckuserModel = new QStandardItemModel;
    servercheckuserModel->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("Identity")));
    servercheckuserModel->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("Account")));
    servercheckuserModel->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("Check")));
    ui->check_tableview->setModel(servercheckuserModel);
    ui->check_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->check_tableview->setColumnWidth(0,100);
    ui->check_tableview->setColumnWidth(1,100);
    ui->check_tableview->setColumnWidth(2,100);
}


/*************************************************
Function:       setServercheckuserModel
Description:    根据输入的参数建立表格，此函数旨在将重复
                的操作（即表格插入数据）集合成一个函数，
                从而避免代码冗杂
Calls:          //
Input:          参数row，为int类型，表示插入的行数
                参数identity，为QString类型，表示用户昵称（唯一标识）
                参数account，为int类型，表示系统为用户匹配的账号
Output:         //
Return:         //
Others:         本函数配合相应的用户列表打印函数进行
*************************************************/
void CheckUserWindow::setServercheckuserModel(int row, QString &identity, int account)
{
    servercheckuserModel->setItem(row,0,new QStandardItem(identity));
    QString caccount = QString("%1").arg(account, 6, 10, QChar('0'));
    servercheckuserModel->setItem(row,1,new QStandardItem(caccount));
    //设置查看按钮
    QPushButton *button = new QPushButton("Check");
    button->setProperty("identity",identity);
    button->setProperty("account",account);
    button->setProperty("row",row);
    connect(button, &QPushButton::released, this, &CheckUserWindow::serverCheckuser);
    ui->check_tableview->setIndexWidget
            (servercheckuserModel->index(servercheckuserModel->rowCount()-1,2), button);

}



/*************************************************
Function:       servercheckuserview
Description:    根据输入的userinfo数据，打印出其中所有用户的
                昵称、账号信息，并且Server可以通过check按钮
                查看用户更多基本信息并修改其积分。
Calls:          CheckUserWindow::setServerCheckview()
                CheckUserWindow::setServercheckuserModel(int row,
                                            QString &identity, int account)
Input:          参数QVector<UserInfo *> userlist，为QVector类型，
                表示需要查看的用户数组
Output:         //
Return:         //
Others:         本函数配合相应的辅助插入数据函数进行
*************************************************/
void CheckUserWindow::servercheckuserview(QVector<UserInfo *> userlist)
{
    ui->checkstacked->setCurrentIndex(0);
    setServerCheckview();
    servercheckuserModel->removeRows(0,servercheckuserModel->rowCount());
    for(int i=0;i<userlist.size();i++)
    {
        QString identity = userlist[i]->userIdentity;
        int account = userlist[i]->userAccount;
        setServercheckuserModel(i,identity,account);
    }
}


/*************************************************
Function:       serverCheckuser
Description:    Server查看用户函数
Calls:          UserInfo::userFind(QVector<UserInfo *>,QString &)
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void CheckUserWindow::serverCheckuser()
{
    ui->checkstacked->setCurrentIndex(2);
    QPushButton *button = (QPushButton *)sender();
    QString identity = button->property("identity").toString();
    UserInfo *tempcheck = UserInfo::userFind(ALL_USERS,identity);
    if(tempcheck)
    {
        ui->serversetcreditbutton->setProperty("identity",identity);

        ui->serverpixlabel->clear();
        ui->serveraccountread->clear();
        ui->serverIdentityread->clear();
        ui->servercreditread->clear();
        ui->servercreditread->setValidator(new QIntValidator(1,100));
        ui->serverqualificationbrowser->clear();

        ui->serverpixlabel->setPixmap(QPixmap( tempcheck->userpixadd));
        ui->serverpixlabel->setScaledContents(true);
        ui->serveraccountread->setEnabled(false);
        ui->serveraccountread->setText(QString("%1").arg(tempcheck->userAccount, 6, 10, QChar('0')));
        ui->serverIdentityread->setEnabled(false);
        ui->serverIdentityread->setText(tempcheck->userIdentity);
        ui->serverqualificationbrowser->setText(tempcheck->userQualification);
    }
}

/*************************************************
Function:       on_serverbackbutton_released
Description:    Server从用户个人界面返回列表界面
Calls:          //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void CheckUserWindow::on_serverbackbutton_released()
{
    ui->checkstacked->setCurrentIndex(0);
}


/*************************************************
Function:       on_serversetcreditbutton_released
Description:    Server改变用户积分数值
Calls:          Server::serverCredituser(UserInfo *user,int credit)
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void CheckUserWindow::on_serversetcreditbutton_released()
{
    int credit = ui->servercreditread->text().toInt();
    QString identity = ui->serversetcreditbutton->property("identity").toString();
    UserInfo *tempuser = UserInfo::userFind(ALL_USERS,identity);
    NOW_SERVER->serverCredituser(tempuser,credit);
}

/*************************************************
Function:       on_addfriendbutton_released
Description:    addfriend按钮的槽函数
Calls:          UserInfo::userFind(QVector<UserInfo *>,QString)
                UserInfo::applyuserFriend(UserInfo *targetuser)
Input:          //
Output:         //
Return:         //
Others:         发送好友申请，进入对方用户好友等待队列
*************************************************/
void CheckUserWindow::on_addfriendbutton_released()
{
//ui->addfriendbutton->setProperty("checkidentity",tempcheck->userIdentity);
    QString identity = ui->addfriendbutton->property("checkidentity").toString();
    UserInfo *targetuser = UserInfo::userFind(ALL_USERS,identity);
    bool notfriend = NOW_USER->applyuserFriend(targetuser);
    if(notfriend)
    {
        QMessageBox::information(this,"prompt","Successfully Send The Application!");
    }
    else
    {
        QMessageBox::information(this,"prompt","You Are Already Friends!");
    }
}
