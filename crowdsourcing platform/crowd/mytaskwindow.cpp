#include "mytaskwindow.h"
#include "ui_mytaskwindow.h"


mytaskWindow::mytaskWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mytaskWindow)
{
    ui->setupUi(this);
    ui->taskstacked->setCurrentIndex(0);
    setTASKview();
    checkTaskset();
    connect(&TranWin, &TranWindow::SubmitbackTOtask,this,&mytaskWindow::setSubmitFalse);
}

//已经提交任务，故submit不可以点选
void mytaskWindow::setSubmitFalse()
{
    ui->l_submitbutton->setEnabled(false);
}

mytaskWindow::~mytaskWindow()
{
    delete ui;
}
//设置表头
void mytaskWindow::checkTaskset()
{
    taskModel = new QStandardItemModel;
    taskModel->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("TaskName")));
    taskModel->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("Poster")));
    taskModel->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("State")));
    taskModel->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("Myrole")));
    taskModel->setHorizontalHeaderItem(4,new QStandardItem(QObject::tr("Check")));
    ui->task_tableview->setModel(taskModel);
    ui->task_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->task_tableview->setColumnWidth(0,100);
    ui->task_tableview->setColumnWidth(1,80);
    ui->task_tableview->setColumnWidth(2,150);
    ui->task_tableview->setColumnWidth(3,80);
    ui->task_tableview->setColumnWidth(4,80);
}

//辅助函数
void mytaskWindow::setTaskModel(int row, QString &tname, QString &tposter, int tstate,int myrole)
{
    QString qtstate;
    QString qmyrole;
    switch (tstate) {
    case 0:
        qtstate = "WaitingLeader";
        break;
    case 1:
        qtstate = "WaitingTranslaters";
        break;
    case 2:
        qtstate = "Translating";
        break;
    case 3:
        qtstate = "Finished";
        break;
    default:
        break;
    }
    switch (myrole) {
    case 0:
        qmyrole = "VISITOR";
        break;
    case 1:
        qmyrole = "POSTER";
        break;
    case 2:
        qmyrole = "LEADER";
        break;
    case 3:
        qmyrole = "TRANSLATER";
        break;
    default:
        break;
    }
    taskModel->setItem(row,0,new QStandardItem(tname));
    taskModel->setItem(row,1,new QStandardItem(tposter));
    taskModel->setItem(row,2,new QStandardItem(qtstate));
    taskModel->setItem(row,3,new QStandardItem(qmyrole));
}

//查看所有任务
void mytaskWindow::showMYTaskmodel(QVector<TaskInfo *> userchecktask)
{
    taskModel->removeRows(0,taskModel->rowCount());
    ui->taskstacked->setCurrentIndex(0);
    for(int i=0;i<userchecktask.size();i++)
    {
        QString tname = userchecktask[i]->taskName;

        QString tposter = userchecktask[i]->taskPoster->userIdentity;
        int tstate = userchecktask[i]->getTaskstate();
        int myrole;

        if(userchecktask[i]->taskPoster->userIdentity  == NOW_USER->userIdentity)
        {
            myrole = POSTER;
        }
        else if(userchecktask[i]->taskLeader != 0x0)
        {
            if(userchecktask[i]->taskLeader->userIdentity == NOW_USER->userIdentity)
            {
                myrole = LEADER;
            }
            else
            {
                bool isTran = false;

                for(int ttran = 0;ttran<userchecktask[i]->taskTranslater.size();ttran++)
                {
                    if(userchecktask[i]->taskTranslater[ttran]->userIdentity == NOW_USER->userIdentity)
                    {
                        isTran = true;
                    }
                }
                if(isTran)
                {
                    myrole = TRANSLATER;
                }
                else
                {
                    myrole = VISITOR;
                }
            }
        }
        else
        {
            bool isTran = false;

            for(int ttran = 0;ttran<userchecktask[i]->taskTranslater.size();ttran++)
            {
                if(userchecktask[i]->taskTranslater[ttran]->userIdentity == NOW_USER->userIdentity)
                {
                    isTran = true;
                }
            }
            if(isTran)
            {
                myrole = TRANSLATER;
            }
            else
            {
                myrole = VISITOR;
            }
        }

        setTaskModel(i,tname, tposter, tstate, myrole);

        //设置查看按钮
        QPushButton *button = new QPushButton("Check");
        button->setProperty("taskname",tname);
        button->setProperty("poster",tposter);
        button->setProperty("state",tstate);
        button->setProperty("myrole",myrole);
        qDebug()<<"myrole"<<myrole;
        //针对不同的角色可以产生不同的查看界面
        if(myrole == POSTER)
        {
            //NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
            connect(button, &QPushButton::released, this, &mytaskWindow::checkMYPOSTTask);
        }

        else if(myrole == LEADER)
        {
            //NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
            connect(button, &QPushButton::released, this, &mytaskWindow::checkMYLEADTask);

        }
        else if(myrole == TRANSLATER)
        {
            //NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
            connect(button, &QPushButton::released, this, &mytaskWindow::checkMYTRANTask);

        }
        else if(myrole == VISITOR)
        {
            //NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
            connect(button, &QPushButton::released, this, &mytaskWindow::checkVISITORTask);
        }
        ui->task_tableview->setIndexWidget
                            (taskModel->index(taskModel->rowCount()-1,4), button);
        //设置查看按钮
    }
}

//针对子任务
void mytaskWindow::showMYTaskmodel(QVector<SubTaskInfo *> userchecktask)
{
    ui->taskstacked->setCurrentIndex(0);
    taskModel->removeRows(0,taskModel->rowCount());
    for(int i=0;i<userchecktask.size();i++)
    {
        qDebug()<<userchecktask.size();
        QString tname = userchecktask[i]->taskName;
        qDebug()<<tname;
        QString tposter = userchecktask[i]->taskPoster->userIdentity;
        qDebug()<<tposter;
        qDebug()<<userchecktask[i]->subTranslater;
        QString ttranslater = userchecktask[i]->subTranslater->userIdentity;
        qDebug()<<ttranslater;
        QString subtname = userchecktask[i]->subtaskName;
        int tstate = userchecktask[i]->getTaskstate();
        int myrole;
        qDebug()<<tname<<"leader:"<<userchecktask[i]->taskLeader->userIdentity;
        if(userchecktask[i]->taskPoster->userIdentity == NOW_USER->userIdentity)
        {
            myrole = POSTER;
        }
        else if(userchecktask[i]->taskLeader)
        {
            if(userchecktask[i]->taskLeader->userIdentity == NOW_USER->userIdentity)
            {

                myrole = LEADER;
            }
            else
            {
                bool isTran = false;
                for(int ttran = 0;ttran<userchecktask[i]->taskTranslater.size();ttran++)
                {
                    if(userchecktask[i]->taskTranslater[ttran]->userIdentity == NOW_USER->userIdentity)
                    {
                        isTran = true;
                    }
                }
                if(isTran)
                {
                    myrole = TRANSLATER;
                }
                else
                {
                    myrole = VISITOR;
                }
            }
        }
        else
        {
            bool isTran = false;
            for(int ttran = 0;ttran<userchecktask[i]->taskTranslater.size();ttran++)
            {
                if(userchecktask[i]->taskTranslater[ttran]->userIdentity == NOW_USER->userIdentity)
                {
                    isTran = true;                    
                }
            }
            if(isTran)
            {
                myrole = TRANSLATER;
            }
            else
            {
                myrole = VISITOR;
            }
        }

        setTaskModel(i,tname, ttranslater, tstate, myrole);
        qDebug()<<"myrole"<<myrole;
        //设置查看按钮
        QPushButton *button = new QPushButton("Check");
        button->setProperty("taskname",tname);
        button->setProperty("translater",ttranslater);
        button->setProperty("poster",tposter);
        button->setProperty("state",tstate);
        button->setProperty("myrole",myrole);
        button->setProperty("subtaskname",subtname);
        //针对不同的角色可以产生不同的查看界面
        if(myrole == POSTER)
        {
            //子任务的对应修改
            NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
            connect(button, &QPushButton::released, this, &mytaskWindow::checkMYPOSTTask);
            //NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
        }

        else if(myrole == LEADER)
        {
            NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
            connect(button, &QPushButton::released, this, &mytaskWindow::LeadcheckMYSubTask);
            //NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
        }
        else if(myrole == TRANSLATER)
        {
            NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
            NOW_SUB = SubTaskInfo::getnowTask(ALL_SUBTASK,subtname);
            connect(button, &QPushButton::released, this, &mytaskWindow::checkMYTRANTask);
            //NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
        }
        else if(myrole == VISITOR)
        {
            NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
            connect(button, &QPushButton::released, this, &mytaskWindow::checkVISITORTask);
            //NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
        }

        ui->task_tableview->setIndexWidget
                            (taskModel->index(taskModel->rowCount()-1,4), button);
        //设置查看按钮
    }
}

//译者查看其他译者
void mytaskWindow::showMYTaskmodel(QVector<SubTaskInfo *> userchecktask,int)
{
    ui->taskstacked->setCurrentIndex(0);
    taskModel->removeRows(0,taskModel->rowCount());
    taskModel->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("Translater")));
    int row = 0;
    for(int i=0;i<userchecktask.size();i++)
    {
        QString tname = userchecktask[i]->taskName;
        QString tposter = userchecktask[i]->taskPoster->userIdentity;
        QString ttranslater = userchecktask[i]->subTranslater->userIdentity;
        QString subtname = userchecktask[i]->subtaskName;
        int myrole = TRANSLATER;
        int tstate = NOW_TASK->getTaskstate();

        if(userchecktask[i]->subTranslater == NOW_USER)
        {
            row-=1;
        }
        else
        {
            setTaskModel(row,tname, ttranslater, tstate, myrole);
            //设置查看按钮
            QPushButton *button = new QPushButton("Check");
            button->setProperty("taskname",tname);
            button->setProperty("translater",ttranslater);
            button->setProperty("poster",tposter);
            button->setProperty("state",tstate);
            button->setProperty("myrole",myrole);
            button->setProperty("subtaskname",subtname);
            NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,tname);
            NOW_SUB = SubTaskInfo::getnowTask(ALL_SUBTASK,subtname);
            connect(button, &QPushButton::released, this, &mytaskWindow::checkOTHERTRANTask);
            ui->task_tableview->setIndexWidget
                            (taskModel->index(taskModel->rowCount()-1,4), button);
            //设置查看按钮
        }
        row += 1;
    }
}

void mytaskWindow::setTASKview()
{
    ui->tasknamelabel->hide();
    ui->booknamelabel->hide();
    ui->posteridentity->hide();
    ui->tasknamelabel->clear();
    ui->booknamelabel->clear();
    ui->bookcoverlabel->clear();
    ui->booknamelabel->clear();
    ui->posteridentity->clear();
    ui->translatetype->clear();
    ui->getbookbutton->hide();
    ui->translatetype->hide();
}

void mytaskWindow::on_getbookbutton_released()
{
    TranWin.show();
    TranWin.setgetbookview();
}

//*******************************************************************//
//*************************   发布者相关    ***************************//
//*******************************************************************//
void mytaskWindow::setPOSTview()
{
    if(NOW_TASK)
    {
        ui->getbookbutton->show();
        ui->tasknamelabel->show();
        ui->booknamelabel->show();
        ui->posteridentity->show();
        ui->translatetype->show();
        ui->tasknamelabel->setText(NOW_TASK->taskName);
        ui->bookcoverlabel->setPixmap(QPixmap(NOW_TASK->taskCover));
        ui->bookcoverlabel->setScaledContents(true);
        ui->booknamelabel->setText(NOW_TASK->taskBook->bookName);
        ui->posteridentity->setText(NOW_TASK->taskPoster->userIdentity);
        ui->translatetype->setText(NOW_TASK->taskBook->bookoriLan+"To\n"+NOW_TASK->tasktargetLan);
        //ui->translatetype->resize(20,60);
        ui->translatelaber->setText(QString::number(NOW_TASK->getsubTasknum()));
        ui->translatelaber->adjustSize();
        ui->taskstateread->setText(NOW_TASK->getTaskstatestr());
        ui->taskstateread->adjustSize();
        ui->taskpaymentedit->setEnabled(false);
        ui->taskpaymentedit->setText(QString::number(NOW_TASK->getTaskpay(),'f',2));
        ui->circletimeedit->setEnabled(false);
        ui->circletimeedit->setText(QString::number( NOW_TASK->getTaskcircleday()));
        ui->signupdeadlineedit->setEnabled(false);
        ui->signupdeadlineedit->setDate(NOW_TASK->taskLeadDay);

        ui->p_transignnum->setText(QString::number(NOW_TASK->tranSignup.size()));
        ui->p_leadersignnum->setText(QString::number(NOW_TASK->leadSignup.size()));

        if(NOW_TASK->getTaskstate()!=WaitingLeader)
        {
            //已经确定负责人了
            ui->p_leaderidentity->setText(NOW_TASK->taskLeader->userIdentity);
        }
        if((NOW_TASK->getTaskstate()==Translating)||(NOW_TASK->getTaskstate()==Finished))
        {
            //已经确定译者了
            ui->p_translaters->setText(QString::number(NOW_TASK->taskTranslater.size()));
        }
    }
}

//发布者的查看界面
void mytaskWindow::checkMYPOSTTask()
{
    ui->taskstacked->setCurrentIndex(1);
    QPushButton *button = (QPushButton *)sender();
    QString nowtaskname = button->property("taskname").toString();
    NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,nowtaskname);
    ui->tasknamelabel->setText(NOW_TASK->taskName);
    setPOSTview();

}
//发布者查看负责人报名人

void mytaskWindow::on_leadersignupbutton_released()
{
    CheckUWin.show();
    CheckUWin.checkuserview(NOW_TASK->leadSignup);
}

//发布者查看负责人
void mytaskWindow::on_p_checkleaderbutton_released()
{
    if(NOW_TASK->getTaskstate() == WaitingLeader)
    {
        QMessageBox::information(this,"prompt","The Task Has No Leader Now!");
    }
    else
    {
        CheckUWin.show();
        QString identity = NOW_TASK->taskLeader->userIdentity;
        CheckUWin.checkuserfunction(identity);
    }
}

//发布者确定负责人

//发布者查看译者报名人？
void mytaskWindow::on_transignupbutton_released()
{
    CheckUWin.show();
    CheckUWin.checkuserview(NOW_TASK->tranSignup);

}


//发布者查看译者
void mytaskWindow::on_checktranbutton_released()
{
    //已经确定译者，查看译者相关信息
    CheckUWin.show();
    CheckUWin.checkuserview(NOW_TASK->taskTranslater);
}

//发布者查看任务进行状态
void mytaskWindow::on_checktaskstatebutton_released()
{
    QString tstate = ui->taskstateread->text();
    if(tstate == "WaitingLeader")
    {
        QMessageBox::information(this,"prompt","Your Task IS Recruiting Leader Now!");
    }
    else if(tstate == "WaitingTranslater")
    {
        QMessageBox::information(this,"prompt","Your Task IS Recruiting Translaters Now!");
    }
    else if(tstate == "Finished")
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this,"question","Your Task IS Finished Now!Check It?",
                              QMessageBox::Yes|QMessageBox::No);
        if(result == QMessageBox::Yes)
        {

            TranWin.show();
            TranWin.setposterview();
        }
    }
    else if(tstate == "Translating")
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this,"question","Your Task IS Translating Now!Check It?",
                              QMessageBox::Yes|QMessageBox::No);
        if(result == QMessageBox::Yes)
        {
            TranWin.show();
            TranWin.setposterview();
        }
    }
}

//发布者结束任务
void mytaskWindow::on_finishtaskbutton_released()
{
    u_POST *userNOWposter = new u_POST(*NOW_USER);
    userNOWposter->userUPDATE(&CHANGE_USERS);
    userNOWposter->userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,userNOWposter->userIdentity);
    userNOWposter->userUPDATE(NOW_TASK->taskPoster);
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this,"question","Sure To Finish Your Task?",
                          QMessageBox::Yes|QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        userNOWposter->p_userChangeState(NOW_TASK,Finished);

        ui->taskstateread->setText("Finished");
    }

}

//发布者确认支付
void mytaskWindow::on_paytaskbutton_released()
{
    //支付界面
    u_POST *nowuser = new u_POST(*NOW_USER);
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this,"question","Pay Now?",
                          QMessageBox::Yes|QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        nowuser->p_userPayLeader(NOW_TASK);
    }
    /*
    bool ispay = nowuser->p_userPayLeader(NOW_TASK);
    if(ispay)
    {
        QMessageBox::information(this,"prompt","Successfully Paid!");
    }
    else
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this,"question","Not Enough Money.Transfer In Now?",
                              QMessageBox::Yes|QMessageBox::No);
        if(result == QMessageBox::Yes)
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
                ispay = nowuser->p_userPayLeader(NOW_TASK);
                if(ispay)
                {
                    QMessageBox::information(this,"prompt","Successfully Paid!");
                }
                else
                {
                    QMessageBox::information(this,"prompt","Still Not Enough.Try Again.");
                }
            }
        }
    }
    */

}

//发布者返回查询页面
void mytaskWindow::on_backbutton_released()
{
    ui->taskstacked->setCurrentIndex(0);
    setTASKview();
}

//*******************************************************************//
//*************************   负责人相关    ***************************//
//*******************************************************************//
void mytaskWindow::setLEADview()
{
    if(NOW_TASK)
    {
        ui->tasknamelabel->show();
        ui->booknamelabel->show();
        ui->posteridentity->show();
        ui->translatetype->show();
        ui->getbookbutton->show();
        ui->tasknamelabel->setText(NOW_TASK->taskName);
        ui->bookcoverlabel->setPixmap(QPixmap(NOW_TASK->taskCover));
        ui->bookcoverlabel->setScaledContents(true);
        ui->booknamelabel->setText(NOW_TASK->taskBook->bookName);
        ui->posteridentity->setText(NOW_TASK->taskPoster->userIdentity);
        ui->translatetype->setText(NOW_TASK->taskBook->bookoriLan+"To\n"+NOW_TASK->tasktargetLan);
        //ui->translatetype->resize(20,60);

        ui->l_taskpaymentedit->setEnabled(false);
        ui->l_taskpaymentedit->setText(QString::number(NOW_TASK->getTaskpay(),'f',2));

        ui->l_circletimeedit->setEnabled(false);
        ui->l_circletimeedit->setText(QString::number( NOW_TASK->getTaskcircleday()));

        ui->l_signupdeadlineedit->setDate(NOW_TASK->taskTranDay);
        ui->l_signupdeadlineedit->calendarPopup();

        ui->l_trannum->setText(QString::number(NOW_TASK->taskTranslater.size()));
        ui->l_subtasknum->setText(QString::number( NOW_TASK->taskSub.size()));
        ui->l_transignnum->setText(QString::number( NOW_TASK->tranSignup.size()));
        ui->l_posterread->setText(NOW_TASK->taskPoster->userIdentity);
        ui->l_posterread->adjustSize();

        if(NOW_TASK->isTranSignday)
        {
            ui->l_setddlbutton->setEnabled(false);
            if(NOW_TASK->isDivide)
            {
                ui->l_postsubtaskbutton->setEnabled(false);
                ui->l_checksubbutton->setEnabled(true);
            }
            else
            {
                ui->l_postsubtaskbutton->setEnabled(true);
                ui->l_checksubbutton->setEnabled(true);
            }
        }
        else
        {
            ui->l_setddlbutton->setEnabled(true);
            ui->l_postsubtaskbutton->setEnabled(false);
            ui->l_checksubbutton->setEnabled(false);
        }

    }

}

//按下查看我负责的任务按钮
void mytaskWindow::checkMYLEADTask()
{
    ui->taskstacked->setCurrentIndex(2);
    QPushButton *button = (QPushButton *)sender();
    QString nowtaskname = button->property("taskname").toString();
    NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,nowtaskname);
    ui->tasknamelabel->setText(NOW_TASK->taskName);
    setLEADview();
}

//负责人查看单独的子任务
void mytaskWindow::LeadcheckMYSubTask()
{
    //切换到tranwin中查看子任务页面
    TranWin.show();

    QPushButton *button = (QPushButton *)sender();
    QString taskname = button->property("taskname").toString();
    NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,taskname);
    QString nowtaskname = button->property("subtaskname").toString();
    NOW_SUB = SubTaskInfo::getnowTask(ALL_SUBTASK,nowtaskname);
    TranWin.setLeaderCheckview();
}

//负责者返回查询页面
void mytaskWindow::on_l_backbutton_released()
{
    ui->taskstacked->setCurrentIndex(0);
    setTASKview();
}


//负责人查看子任务
void mytaskWindow::on_l_checksubbutton_released()
{
    setTASKview();
    showMYTaskmodel(NOW_TASK->taskSub);
}

//负责人分发子任务
void mytaskWindow::on_l_postsubtaskbutton_released()
{
    int trannum = NOW_TASK->taskTranslater.size();
    if(trannum==0)
    {
        //还没有确定译者
        QMessageBox::information(this,"prompt","You Haaven't Chosen The Translaters!");
    }
    else
    {
        QString informes = "You Have To Divide The Task Into "+QString::number(trannum)+"Parts!";
        QMessageBox::information(this,"prompt",informes);
        TranWin.show();
        TranWin.setleaderDivideview(trannum);
    }
}

//负责人提交任务
void mytaskWindow::on_l_submitbutton_released()
{
    int trannum = NOW_TASK->taskTranslater.size();
    TranWin.show();
    TranWin.setLeaderSubmitview(trannum);
}

//负责人设置译者报名截止时间
void mytaskWindow::on_l_setddlbutton_released()
{
    u_LEADER *newleader = new u_LEADER(*NOW_USER);
    newleader->userUPDATE(&CHANGE_USERS);
    newleader->userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,
                                  newleader->userIdentity);
    newleader->userUPDATE(NOW_TASK->taskLeader);
    bool isddl = false;
    QDate setddl = ui->l_signupdeadlineedit->date();
    isddl = newleader->l_usersetTransignday(NOW_TASK, setddl);
    if(isddl)
    {
        QMessageBox::information(this,"prompt","Successfully Set Translaters Signup Deadline!");
        ui->l_signupdeadlineedit->setReadOnly(false);
        ui->l_postsubtaskbutton->setEnabled(true);
        ui->l_checksubbutton->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this,"prompt","Please Set Correct Deadline Date!");
    }
    NOW_TASK->taskUPDATE(&CHANGE_TASKS);
}

//负责人查看译者报名信息
void mytaskWindow::on_l_transignupbutton_released()
{
    CheckUWin.show();
    CheckUWin.checkuserview(NOW_TASK->tranSignup);
}

//负责人查看发布者信息并可以发送消息交流
void mytaskWindow::on_l_checkposterbutton_released()
{
    CheckUWin.show();
    QString identity = NOW_TASK->taskPoster->userIdentity;
    CheckUWin.checkuserfunction(identity);
}

//负责人领取酬金
void mytaskWindow::on_l_gettaskbutton_released()
{
    u_LEADER *nowuser =new u_LEADER(*NOW_USER);
    bool isgetpaid = nowuser->l_userGetPaid(NOW_TASK);
    if(isgetpaid)
    {
        QMessageBox::information(this,"prompt","Successfully Receive Payment!");
        ui->l_gettaskbutton->setEnabled(false);
    }
    else
    {
        QMessageBox::information(this,"prompt","Wait For The Poster To Pay!");
    }
    nowuser->userUPDATE(&CHANGE_USERS);
    nowuser->userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,
                                  nowuser->userIdentity);
    nowuser->userUPDATE(NOW_TASK->taskLeader);
}

//负责人查看译者
void mytaskWindow::on_l_checktranbutton_released()
{
    CheckUWin.show();
    CheckUWin.checkuserview(NOW_TASK->taskTranslater);
}
//*******************************************************************//
//*************************   翻译者相关    ***************************//
//*******************************************************************//
void mytaskWindow::setTRANview()
{
    if(NOW_SUB)
    {
        ui->t_leaderread->setText(NOW_SUB->taskLeader->userIdentity);
        ui->t_leaderread->adjustSize();
        ui->t_posterread->setText(NOW_SUB->taskPoster->userIdentity);
        ui->t_posterread->adjustSize();
        ui->t_trannum->setText(QString::number( NOW_SUB->taskTranslater.size()));
        ui->t_circletimeedit->setEnabled(false);
        ui->t_circletimeedit->setText(QString::number(NOW_SUB->getTaskcircleday()));
        ui->t_taskpaymentedit->setEnabled(false);
        ui->t_taskpaymentedit->setText(QString::number(NOW_SUB->getTaskpay(),'f',2));
    }

}

//翻译界面
void mytaskWindow::checkMYTRANTask()
{
    ui->taskstacked->setCurrentIndex(3);
    QPushButton *button = (QPushButton *)sender();
    QString taskname = button->property("taskname").toString();
    QString nowtaskname = taskname + NOW_USER->userIdentity;
    NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,taskname);
    NOW_SUB = SubTaskInfo::getnowTask(ALL_SUBTASK,nowtaskname);

    if(NOW_SUB)
    {
        ui->tasknamelabel->show();
        ui->booknamelabel->show();
        ui->posteridentity->show();
        ui->translatetype->show();
        ui->tasknamelabel->setText(NOW_SUB->taskName);
        ui->tasknamelabel->setText(NOW_SUB->taskName);
        ui->bookcoverlabel->setPixmap(QPixmap(NOW_SUB->taskCover));
        ui->bookcoverlabel->setScaledContents(true);
        ui->booknamelabel->setText(NOW_SUB->taskBook->bookName);
        ui->posteridentity->setText(NOW_SUB->taskPoster->userIdentity);
        ui->translatetype->setText(NOW_SUB->taskBook->bookoriLan+"To\n"+NOW_SUB->tasktargetLan);
        //ui->translatetype->resize(20,60);
        setTRANview();
    }
}

void mytaskWindow::checkOTHERTRANTask()
{
    QPushButton *button = (QPushButton *)sender();
    QString taskname = button->property("taskname").toString();
    QString nowtaskname = button->property("subtaskname").toString();
    NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,taskname);
    NOW_SUB = SubTaskInfo::getnowTask(ALL_SUBTASK,nowtaskname);
    TranWin.show();
    TranWin.setotherTranview();
}


//译者查看发布者
void mytaskWindow::on_t_checkposterbutton_released()
{
    CheckUWin.show();
    QString identity = NOW_SUB->taskPoster->userIdentity;
    CheckUWin.checkuserfunction(identity);
}

//译者查看负责人
void mytaskWindow::on_t_checkleaderbutton_released()
{
    CheckUWin.show();
    QString identity = NOW_SUB->taskLeader->userIdentity;
    CheckUWin.checkuserfunction(identity);
}

//译者查看译者
void mytaskWindow::on_t_checktranbutton_released()
{
    CheckUWin.show();
    CheckUWin.checkuserview(NOW_SUB->taskTranslater);
}

//译者返回总的任务页面
void mytaskWindow::on_t_backbutton_released()
{
    ui->taskstacked->setCurrentIndex(0);
    setTASKview();
}

//译者查看别人的任务
void mytaskWindow::on_t_othertaskbutton_released()
{
    setTASKview();
    QString taskname = NOW_SUB->taskName;
    NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,taskname);
    showMYTaskmodel(NOW_TASK->taskSub,0);
}


//译者领取酬金
void mytaskWindow::on_t_gettaskbutton_released()
{
    u_TRAN *nowuser =new u_TRAN(*NOW_USER);
    bool isgetpaid = nowuser->t_userGetPaid(NOW_SUB);
    if(isgetpaid)
    {
        QMessageBox::information(this,"prompt","Successfully Receive Payment!");
        ui->t_gettaskbutton->setEnabled(false);
    }
    else
    {
        QMessageBox::information(this,"prompt","Wait For The Poster To Pay!");
    }


    //int nowindex = NOW_TASK->taskTranslater.indexOf(NOW_USER);
    nowuser->userUPDATE(&CHANGE_USERS);
    nowuser->userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,
                                  nowuser->userIdentity);
    NOW_SUB->subTranslater = nowuser;
    //NOW_TASK->taskTranslater.replace(nowindex,nowuser);
    nowuser->userUPDATE(&(NOW_TASK->taskTranslater));

}


//译者进入任务编辑页面
void mytaskWindow::on_t_entermytaskbutton_released()
{
    //切换到tranwin中查看子任务页面
    TranWin.show();
    TranWin.settranview();
}

//*******************************************************************//
//***********************   普通用户可见    **************************//
//*******************************************************************//
void mytaskWindow::setVISITORview()
{
    qDebug()<<NOW_TASK;
    qDebug()<<NOW_TASK->taskName;
    if(NOW_TASK)
    {
        ui->tasknamelabel->show();
        ui->booknamelabel->show();
        ui->posteridentity->show();
        ui->translatetype->show();
        ui->tasknamelabel->setText(NOW_TASK->taskName);
        ui->bookcoverlabel->setPixmap(QPixmap(NOW_TASK->taskCover));
        ui->bookcoverlabel->setScaledContents(true);
        ui->booknamelabel->setText(NOW_TASK->taskBook->bookName);
        ui->posteridentity->setText(NOW_TASK->taskPoster->userIdentity);
        ui->translatetype->setText(NOW_TASK->taskBook->bookoriLan+"To\n"+NOW_TASK->tasktargetLan);
        ui->getbookbutton->hide();
        int tstate = NOW_TASK->getTaskstate();
        switch (tstate) {
        case WaitingLeader:
            setL_VISITORview();
            break;
        case WaitingTranslater:
            setT_VISITORview();
            break;
        case Translating:
            setA_VISITORview();
            break;
        case Finished:
            setA_VISITORview();
            break;
        default:
            break;
        }

    }
}

//针对招募负责人的浏览界面
void mytaskWindow::setL_VISITORview()
{
    ui->taskstacked->setCurrentIndex(4);
    ui->vl_bookintrobrowser->setText(NOW_TASK->taskIntro);
    ui->vl_circleread->setText(QString::number(NOW_TASK->getTaskcircleday()));
    ui->vl_circleread->setEnabled(false);
    ui->vl_signupdeadlineread->setDate(NOW_TASK->taskLeadDay);
    //qDebug()<<"when check"<<NOW_TASK->taskLeadDay.toString("yyyy-MM-dd");
    ui->vl_signupdeadlineread->setReadOnly(true);
    ui->vl_taskpaymentread->setText(QString::number(NOW_TASK->getTaskpay(),'f',2));
    //qDebug()<<"VL"<<ui->vl_taskpaymentread->text();
    ui->vl_taskpaymentread->setReadOnly(true);

    ui->vl_posterread->setText(NOW_TASK->taskPoster->userIdentity);
    ui->vl_signupleadernum->setText(QString::number( NOW_TASK->leadSignup.size()));
    ui->vl_signuptrannum->setText(QString::number(NOW_TASK->tranSignup.size()));
    ui->vl_checkposterbutton->setProperty("identity",NOW_TASK->taskPoster->userIdentity);

}

//针对招募译者的浏览界面
void mytaskWindow::setT_VISITORview()
{
    ui->taskstacked->setCurrentIndex(5);
    ui->vt_bookintrobrowser->setText(NOW_TASK->taskIntro);
    ui->vt_circleread->setText(QString::number(NOW_TASK->getTaskcircleday()));
    ui->vt_circleread->setEnabled(false);
    ui->vt_signupdeadlineread->setDate(NOW_TASK->taskTranDay);
    ui->vt_signupdeadlineread->setReadOnly(true);
    ui->vt_taskpaymentread->setText(QString::number(NOW_TASK->getTaskpay(),'f',2));
    ui->vt_taskpaymentread->setReadOnly(true);

    ui->vt_posterread->setText(NOW_TASK->taskPoster->userIdentity);
    ui->vt_leaderread->setText(NOW_TASK->taskLeader->userIdentity);
    ui->vt_signuptrannum->setText(QString::number(NOW_TASK->tranSignup.size()));
    ui->vt_checkposterbutton->setProperty("identity",NOW_TASK->taskPoster->userIdentity);
    ui->vt_checkleader->setProperty("identity",NOW_TASK->taskLeader->userIdentity);
}

//针对无参与用户（翻译进行中或者已经完成）的浏览界面
void mytaskWindow::setA_VISITORview()
{
    ui->taskstacked->setCurrentIndex(6);
    ui->v_posterread->setText(NOW_TASK->taskPoster->userIdentity);
    ui->v_leaderread->setText(NOW_TASK->taskLeader->userIdentity);
    ui->v_bookintrobrowser->setText(NOW_TASK->taskIntro);
    ui->v_trannum->setText(QString::number(NOW_TASK->taskTranslater.size()));
    ui->v_checkposterbutton->setProperty("identity",NOW_TASK->taskPoster->userIdentity);
    ui->v_checkleaderbutton->setProperty("identity",NOW_TASK->taskLeader->userIdentity);
}

void mytaskWindow::checkVISITORTask()
{
    QPushButton *button = (QPushButton *)sender();
    QString nowtaskname = button->property("taskname").toString();
    NOW_TASK = TaskInfo::getnowTask(ALL_TASKS,nowtaskname);
    qDebug()<<NOW_TASK;
    qDebug()<<NOW_TASK->taskName;
    setVISITORview();
}

void mytaskWindow::on_v_checkposterbutton_released()
{
    CheckUWin.show();
    QString identity = NOW_TASK->taskPoster->userIdentity;
    CheckUWin.checkuserfunction(identity);
}

void mytaskWindow::on_v_checkleaderbutton_released()
{
    CheckUWin.show();
    QString identity = NOW_TASK->taskLeader->userIdentity;
    CheckUWin.checkuserfunction(identity);
}

void mytaskWindow::on_v_checktranbutton_released()
{
    CheckUWin.show();
    CheckUWin.checkuserview(NOW_TASK->taskTranslater);
}

void mytaskWindow::on_vl_checkposterbutton_released()
{
    CheckUWin.show();
    QString identity = NOW_TASK->taskPoster->userIdentity;
    CheckUWin.checkuserfunction(identity);
}

void mytaskWindow::on_vt_checkposterbutton_released()
{
    CheckUWin.show();
    QString identity = NOW_TASK->taskPoster->userIdentity;
    CheckUWin.checkuserfunction(identity);
}

void mytaskWindow::on_vt_checkleader_released()
{
    CheckUWin.show();
    QString identity = NOW_TASK->taskLeader->userIdentity;
    CheckUWin.checkuserfunction(identity);
}


//用户点击报名负责人
void mytaskWindow::on_vl_signupleader_released()
{
    int signupstate = NOW_USER->userSignupLead(NOW_TASK);
    switch (signupstate) {
    case l_Success:
        QMessageBox::information(this,"prompt","Successfully Sign up!");
        break;
    case l_Overdue:
        QMessageBox::information(this,"prompt","Sorry,Your Signing Up Is Overdue!");
        break;
    case l_LackCredit:
        QMessageBox::information(this,"prompt","Sorry,You Are Lack Of Credit To Be A Leader!");
        break;
    case l_AlreadySignup:
        QMessageBox::information(this,"prompt","You've Signed Up This Task Before!");
        break;
    case l_Poster:
        QMessageBox::information(this,"prompt","You Are The Poster Of This Task!");
        break;
    case l_AlreadySignupTran:
        QMessageBox::information(this,"prompt","You've Signed Up This Task As A Translater!");
        break;
    default:
        break;
    }
}

//用户点击报名译者
void mytaskWindow::on_vl_signuptranbutton_released()
{
    int signupstate = NOW_USER->userSignupTran(NOW_TASK);
    switch (signupstate) {
    case t_Success:
        QMessageBox::information(this,"prompt","Successfully Sign Up!");
        break;
    case t_Overdue:
        QMessageBox::information(this,"prompt","Sorry,Your Signing Up Is Overdue!");
        break;
    case t_AlreadySignup:
        QMessageBox::information(this,"prompt","You've Signed Up This Task Before!");
        break;
    case t_Poster:
        QMessageBox::information(this,"prompt","You Are The Poster Of This Task!");
        break;
    case t_Leader:
        QMessageBox::information(this,"prompt","You Are The Leader Of This Task!");
        break;
    case t_AlreadySignupLeader:
        QMessageBox::information(this,"prompt","You've Signed Up This Task As A Leader!");
        break;
    default:
        break;
    }
}


void mytaskWindow::on_vt_signuptranbutton_released()
{
    int signupstate = NOW_USER->userSignupTran(NOW_TASK);
    switch (signupstate) {
    case t_Success:
        QMessageBox::information(this,"prompt","Successfully Sign Up!");
        break;
    case t_Overdue:
        QMessageBox::information(this,"prompt","Sorry,Your Signing Up Is Overdue!");
        break;
    case t_AlreadySignup:
        QMessageBox::information(this,"prompt","You've Signed Up This Task Before!");
        break;
    case t_Poster:
        QMessageBox::information(this,"prompt","You Are The Poster Of This Task!");        break;
    case t_Leader:
        QMessageBox::information(this,"prompt","You Are The Leader Of This Task!");
        break;
    case t_AlreadySignupLeader:
        QMessageBox::information(this,"prompt","You've Signed Up This Task As A Leader!");
        break;
    default:
        break;
    }
}


