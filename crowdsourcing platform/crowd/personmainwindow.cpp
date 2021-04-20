#include "personmainwindow.h"
#include "ui_personmainwindow.h"
#include "platform.h"
#include <QTableView>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

PersonMainWindow::PersonMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PersonMainWindow)
{
    ui->setupUi(this);

    //界面设置
    personWinSet();

    //系统时间
    ui->statusBar->addWidget(ui->datelabel);
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,&QTimer::timeout,this,&PersonMainWindow::timeUpDate);
}

PersonMainWindow::~PersonMainWindow()
{
    delete ui;
}

//homepage设置
//NEW勾选框
void PersonMainWindow::on_new_firstradiobutton_released()
{
    ui->new_stackedWidget->setCurrentIndex(0);
}

void PersonMainWindow::on_new_secondradiobutton_released()
{
    ui->new_stackedWidget->setCurrentIndex(1);
}
//RECOMMEND勾选框
void PersonMainWindow::on_REC_firstradiobutton_released()
{
    ui->REC_stackedWidget->setCurrentIndex(0);
}

void PersonMainWindow::on_REC_secondradiobutton_released()
{
    ui->REC_stackedWidget->setCurrentIndex(1);
}

//最新任务
void PersonMainWindow::homepageNewSet()
{
    ui->new_stackedWidget->setCurrentIndex(0);
    if(ALL_TASKS.size() == 0)
    {
        ui->new_firstbookcoverlabel->clear();
        ui->new_firstbookname->clear();
        ui->new_firstposter->clear();

        ui->new_firstradiobutton->hide();
        ui->new_secondradiobutton->hide();
    }
    else if(ALL_TASKS.size() == 1)
    {
        TaskInfo *thistask = ALL_TASKS[0];
        ui->new_firstbookcoverlabel->setPixmap(QPixmap(thistask->taskCover));
        ui->new_firstbookcoverlabel->setScaledContents(true);
        ui->new_firstbookname->setText(thistask->taskBook->bookName);
        ui->new_firstbookname->adjustSize();
        ui->new_firstposter->setText(thistask->taskPoster->userIdentity);
        ui->new_firstradiobutton->show();
        ui->new_firstradiobutton->setProperty("taskname",thistask->taskName);
        ui->new_firstradiobutton->setProperty("taskstate",thistask->getTaskstate());

        ui->new_secondradiobutton->hide();
    }
    else
    {
        ui->new_firstradiobutton->show();
        ui->new_secondradiobutton->show();
        int num = ALL_TASKS.size();
        ui->new_firstbookcoverlabel->setPixmap(QPixmap(ALL_TASKS[num-1]->taskCover));
        ui->new_firstbookcoverlabel->setScaledContents(true);
        ui->new_firstbookname->setText(ALL_TASKS[num-1]->taskBook->bookName);
        ui->new_firstbookname->adjustSize();
        ui->new_firstposter->setText(ALL_TASKS[num-1]->taskPoster->userIdentity);
        ui->new_secondbookcoverlabel->setPixmap(QPixmap(ALL_TASKS[num-2]->taskCover));
        ui->new_secondbookcoverlabel->setScaledContents(true);
        ui->new_secondbookname->setText(ALL_TASKS[num-2]->taskBook->bookName);
        ui->new_secondbookname->adjustSize();
        ui->new_secondposter->setText(ALL_TASKS[num-2]->taskPoster->userIdentity);
    }
}

//推荐任务
void PersonMainWindow::homepageRecSet()
{
    ui->REC_stackedWidget->setCurrentIndex(0);
    //寻找目标语言或者原始语言与我的任务相同的
    QVector<TaskInfo *> tasklist =  NOW_USER->userFindInterest();
    qDebug()<<tasklist.size();
    if(tasklist.size() == 0)
    {
        ui->REC_firstbookcoverlabel->clear();
        ui->REC_firstbookname->clear();
        ui->REC_firstposter->clear();
        ui->REC_secondbookcoverlabel->clear();
        ui->REC_secondbookname->clear();
        ui->REC_secondposter->clear();

        ui->REC_firstradiobutton->hide();
        ui->REC_secondradiobutton->hide();
    }
    else if(tasklist.size() == 1)
    {
        TaskInfo *thistask = tasklist[0];
        ui->REC_firstbookcoverlabel->setPixmap(QPixmap(thistask->taskCover));
        ui->REC_firstbookcoverlabel->setScaledContents(true);
        ui->REC_firstbookname->setText(thistask->taskBook->bookName);
        ui->REC_firstbookname->adjustSize();
        ui->REC_firstposter->setText(thistask->taskPoster->userIdentity);
        ui->REC_firstradiobutton->show();
        ui->REC_firstradiobutton->setProperty("taskname",thistask->taskName);
        ui->REC_firstradiobutton->setProperty("taskstate",thistask->getTaskstate());

        ui->REC_secondradiobutton->hide();
    }
    else
    {

        ui->REC_firstradiobutton->show();
        ui->REC_secondradiobutton->show();
        ui->REC_firstbookcoverlabel->setPixmap(QPixmap(tasklist[0]->taskCover));
        ui->REC_firstbookcoverlabel->setScaledContents(true);
        ui->REC_firstbookname->setText(tasklist[0]->taskBook->bookName);
        ui->REC_firstbookname->adjustSize();
        ui->REC_firstposter->setText(tasklist[0]->taskPoster->userIdentity);
        ui->REC_secondbookcoverlabel->setPixmap(QPixmap(tasklist[1]->taskCover));
        ui->REC_secondbookcoverlabel->setScaledContents(true);
        ui->REC_secondbookname->setText(tasklist[1]->taskBook->bookName);
        ui->REC_secondbookname->adjustSize();
        ui->REC_secondposter->setText(tasklist[1]->taskPoster->userIdentity);
    }
}


//界面设置
void PersonMainWindow::personWinSet()
{
    ui->mainstacked->setCurrentIndex(0);
    ui->pertab->setCurrentIndex(0);
    //用户浏览个人信息界面
    ui->pixlabel->setScaledContents(true);
    ui->accountread->setEnabled(false);
    ui->Identityread->setEnabled(false);
    ui->Sexread->setEditable(false);
    ui->emailread->setEnabled(false);
    ui->birthdayread->setEnabled(false);
    ui->moneyread->setEnabled(false);
    ui->creditread->setEnabled(false);
    ui->pixroad->setEnabled(false);
    ui->moneyedit->setEnabled(false);
    ui->psdedit->setEchoMode(QLineEdit::Password);
    ui->psdconedit->setEchoMode(QLineEdit::Password);
    //用户修改个人信息界面
    ui->birthdayedit->setCalendarPopup(true);

    ui->pixchangelabel->setScaledContents(true);
}

//用户修改个人信息数据更新
//由于设置函数时作为友元函数的失误，故此函数出现在此处
void UserInfo::userchangeinfo(PersonMainWindow *a)
{
    QString newpsd = a->ui->psdedit->text();
    if(!newpsd.isEmpty())
    {
        userPassword = a->ui->psdedit->text();
    }
    QString newemail = a->ui->emailedit->text();
    if(!newemail.isEmpty())
    {
        userEmail = newemail;
    }
    userSex = a->ui->Sexedit->currentText();
    userBirthday = a->ui->birthdayedit->date();
    QString newpix = a->ui->pixroad->text();
    if(!newpix.isEmpty())
    {
        userpixadd = newpix;
    }
}

//系统时间显示
void PersonMainWindow::timeUpDate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->datelabel->setText(str);
}

void PersonMainWindow::usernewmes()
{
    int mesunread = NOW_USER->mesunRead();
    if(mesunread != 0)
    {
        QString unreadstr = "You Have "+QString::number(mesunread)
                            + "New Messages!";
        QMessageBox::information(this,"prompt",unreadstr);
    }
}

//有新的好友申请
void PersonMainWindow::usernewfriends()
{
    qDebug()<<NOW_USER->userRecfriend.size();
    for(int i=0;i<NOW_USER->userRecfriend.size();i++)
    {
        QString identity = NOW_USER->userRecfriend[i]->userIdentity;
        UserInfo *senduser = UserInfo::userFind(ALL_USERS,identity);
        QString mes = identity + " Sends A Application For Friends. Agree?";
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this,"question",
                      mes,QMessageBox::Yes|QMessageBox::No);
        if(result == QMessageBox::Yes)
        {
            NOW_USER->adduserFriend(senduser);
        }
        else
        {
            NOW_USER->refuseuserFriend(senduser);
        }


    }
}

void PersonMainWindow::myPersonalshow()
{
    this->show();
    //定位至homepage
    ui->mainstacked->setCurrentIndex(0);
    //最新任务
    homepageNewSet();
    //个性化推荐
    homepageRecSet();
    //新消息提醒
    usernewmes();
    //新的好友申请
    usernewfriends();
    //用户基本信息栏
    ui->pixlabel->setPixmap(QPixmap(NOW_USER->userpixadd));
    ui->accountlabel->setText(QString("%1").arg(NOW_USER->userAccount, 6, 10, QChar('0')));
    ui->identitylabell->setText(NOW_USER->userIdentity);
    ui->pixchangelabel->setPixmap(QPixmap(NOW_USER->userpixadd));
    ui->accountread->setText(QString("%1").arg(NOW_USER->userAccount, 6, 10, QChar('0')));
    ui->Identityread->setText(NOW_USER->userIdentity);
    int index = ui->Sexread->findText(NOW_USER->userSex);
    ui->Sexread->setCurrentIndex(index);
    ui->Sexedit->setCurrentIndex(index);
    ui->Sexread->setEnabled(false);
    ui->emailread->setText(NOW_USER->userEmail);
    ui->birthdayread->setDate(NOW_USER->userBirthday);
    QString strmoney = QString::number(NOW_USER->getuserMoney(),'f',2);
    ui->moneyread->setText(strmoney);
    ui->creditread->setText(QString::number(NOW_USER->userCredit,10));
    ui->moneyedit->setText(strmoney);
    ui->emailedit->setText(NOW_USER->userEmail);
    ui->birthdayedit->setDate(NOW_USER->userBirthday);
    ui->moneyedit->setText(QString::number(NOW_USER->getuserMoney()));


}


void PersonMainWindow::on_aboutuserButton_released()
{
    ui->mainstacked->setCurrentIndex(0);
    myPersonalshow();
}

void PersonMainWindow::on_pushButton_released()
{
    ui->mainstacked->setCurrentIndex(1);
    homepageNewSet();
    homepageRecSet();
}

void PersonMainWindow::on_abouttool_released()
{
    ui->mainstacked->setCurrentIndex(2);
}

void PersonMainWindow::on_QAtool_released()
{
    ui->mainstacked->setCurrentIndex(3);
}

void PersonMainWindow::on_psdconedit_editingFinished()
{
    if(ui->psdedit->text() == NULL)
    {
        qDebug()<<"没有输入密码！";

        ui->psdedit->clear();
        ui->psdconedit->clear();
        ui->psdedit->setFocus();
        QMessageBox::critical(this,"prompt","Password Cannot Be NULL!");

    }
    else
    {
        QString psdold = ui->psdedit->text();
        QString psdre = ui->psdconedit->text();
        if(psdold != psdre)
        {
            qDebug()<<"输入密码不一致！";
            QMessageBox::critical(this,"prompt","The Password isnot the Same!");
            ui->psdconedit->clear();
            ui->psdconedit->setFocus();
        }
    }
    myPersonalshow();
}

void PersonMainWindow::on_confirmbuttom_released()
{
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this,"question","Sure for Your Change?",
                          QMessageBox::Yes|QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        NOW_USER->userchangeinfo(this);
        NOW_USER->userUPDATE(&CHANGE_USERS);
        NOW_USER->userUPDATE(&ALL_USERS);
    }
    myPersonalshow();

}

void PersonMainWindow::on_pixchangebutton_released()
{
    //extern QVector<UserInfo *> ALL_USERS;
    QFileDialog *pixfile = new QFileDialog(this);
    pixfile->setWindowTitle(tr("Open Files"));
    //设置文件默认路径
    pixfile->setDirectory(".");
    //设置文件过滤器
    pixfile->setNameFilter("Images(*.png *.jpg *.jpeg *.bmp)");

    //设置视图
    pixfile->setViewMode(QFileDialog::Detail);
    //设置可以选择多个文件，默认只能选择一个文件QFileDialog::ExistingFiles
    //pixfile->setFileMode(QFileDialog::ExistingFiles);
    //打印所有选择的文件路径
    QStringList filenames;
    if(pixfile->exec())
    {
        filenames = pixfile->selectedFiles();
    }
    for(auto tmp : filenames)
    {
       ui->pixchangelabel->setPixmap(tmp);
       ui->pixchangelabel->setScaledContents(true);
       ui->pixroad->setText(tmp);
   }
    myPersonalshow();
}

void PersonMainWindow::on_mymessage_released()
{
    MesWin.show();
    MesWin.mesWinset();
}

void PersonMainWindow::on_postbutton_released()
{
    PostWin.show();
    PostWin.postWinRefresh();
}

void PersonMainWindow::on_mytaskpushButton_released()
{
    MytaskWin.show();
    MytaskWin.setTASKview();
    //界面打印与我相关的所有任务
    qDebug()<<NOW_USER->userIdentity<<NOW_USER->userTasks.size();
    MytaskWin.showMYTaskmodel(NOW_USER->userTasks);
}

void PersonMainWindow::on_mypostButton_released()
{
    MytaskWin.show();
    //界面打印我发布的所有任务
    //MytaskWin.setPOSTview();
    MytaskWin.setTASKview();
    MytaskWin.showMYTaskmodel(NOW_USER->userPost);
}

void PersonMainWindow::on_myleadButton_released()
{
    MytaskWin.show();
    //界面打印我负责的所有任务
    MytaskWin.setTASKview();
    MytaskWin.showMYTaskmodel(NOW_USER->userLead);
}

void PersonMainWindow::on_mytranButton_released()
{
    MytaskWin.show();
    //界面打印我参与翻译的所有任务
    MytaskWin.setTASKview();
    MytaskWin.showMYTaskmodel(NOW_USER->userTran);
}


void PersonMainWindow::on_logoutbutton_released()
{
    emit PerToMain("logout");
    this->close();
}

void PersonMainWindow::on_tranferinbutton_released()
{
    QString dlgTitle="Transfer";
    QString txtLabel="Please Input Money";
    float defaultValue=20.00;

    float minValue=0, maxValue=10000;  //范围
    int decimals=2;//小数点位数

    bool ok=false;
    float inputValue = QInputDialog::getDouble(this, dlgTitle,txtLabel,
                            defaultValue, minValue,maxValue,decimals,&ok);
    if (ok) //确认选择
    {
        NOW_USER->setuserMoney(inputValue);
    }
    myPersonalshow();

}

//所有任务
void PersonMainWindow::on_allbutton_released()
{
    MytaskWin.show();
    MytaskWin.setTASKview();
    MytaskWin.showMYTaskmodel(ALL_TASKS);
}

//申请成为任务负责人
void PersonMainWindow::on_leadbutton_released()
{
    MytaskWin.show();
    //陈列招募负责人的tableview，左侧状态栏置空
    MytaskWin.setTASKview();
    QVector<TaskInfo *> taskrecruiting;
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        if(ALL_TASKS[i]->getTaskstate() == WaitingLeader)
        {
            taskrecruiting.push_back(ALL_TASKS[i]);
        }
    }
    MytaskWin.showMYTaskmodel(taskrecruiting);
}

//申请成为任务译者
void PersonMainWindow::on_tranbutton_released()
{
    MytaskWin.show();
    MytaskWin.setTASKview();
    QVector<TaskInfo *> taskrecruiting;
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        if(ALL_TASKS[i]->getTaskstate() == WaitingTranslater)
        {
            taskrecruiting.push_back(ALL_TASKS[i]);
        }
    }
    MytaskWin.showMYTaskmodel(taskrecruiting);
}

//搜索
void PersonMainWindow::on_searchbutton_released()
{
    QString myinput = ui->search->text();
    QVector<TaskInfo *> tasksearch;
    if(myinput.isEmpty())
    {
        QMessageBox::information(this,"prompt","Please Input Something!");
    }
    else
    {
        for(int i=0;i<ALL_TASKS.size();i++)
        {
            if(ALL_TASKS[i]->taskName.contains(myinput,Qt::CaseInsensitive))
            {
                tasksearch.push_back(ALL_TASKS[i]);
            }
        }
    }
    MytaskWin.show();
    MytaskWin.setTASKview();
    MytaskWin.showMYTaskmodel(tasksearch);
}

//查看我的朋友
void PersonMainWindow::on_myfriendbutton_released()
{
    CheckUWin.show();
    CheckUWin.checkfriendview(NOW_USER->userFriends);
}

