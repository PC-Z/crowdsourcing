#include "tranwindow.h"
#include "ui_tranwindow.h"
#include "platform.h"
#include <QMessageBox>

TranWindow::TranWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TranWindow)
{
    ui->setupUi(this);
}

TranWindow::~TranWindow()
{
    delete ui;
}

//刷新
void TranWindow::tranwinRefresh()
{
    ui->a_bookbrowser->clear();
    ui->p_bookread->clear();
    ui->p_booktranbrowser->clear();
    ui->ld_taskbookbrowser->clear();

    ui->ld_translaterlabel->clear();
    ui->ld_allsublabel->clear();
    ui->l_bookread->clear();
    ui->l_commentedit->clear();
    ui->l_commentwidget->hide();
    ui->l_subtranbrowser->clear();
    ui->l_translateridentity->clear();
    ui->t_bookread->clear();
    ui->t_booktranslate->clear();
    ui->t_commentbrowser->clear();
    ui->t_commentwidget->hide();
    //ui->to_booktranslate->setEnabled(false);
    ui->ls_finaltranslateedit->clear();

}

//获取图书文本界面
void TranWindow::setgetbookview()
{
    ui->stackedWidget->setCurrentIndex(0);
    tranwinRefresh();
    ui->a_bookbrowser->setText(NOW_TASK->taskBook->bookContent);
}

//发布者查看已经完成的任务
void TranWindow::setposterview()
{
    tranwinRefresh();
    ui->stackedWidget->setCurrentIndex(1);
    ui->p_bookread->setText(NOW_TASK->taskBook->bookContent);
    ui->p_booktranbrowser->setText(NOW_TASK->taskTran);
}

//发布者关闭
void TranWindow::on_p_closebutton_released()
{
    this->close();
}

//负责人分割任务
void TranWindow::setleaderDivideview(int num)
{
    tranwinRefresh();
    SubEdit.clear();
    ui->stackedWidget->setCurrentIndex(2);
    ui->ld_taskbookbrowser->setText(NOW_TASK->taskBook->bookContent);
    ui->ld_toolbox->setItemText(0,"TaskDivide");
    ui->ld_toolbox->setItemEnabled(0,false);
    ui->ld_allsublabel->setText(QString::number(num));
    QString identity;
    for(int i=0;i<num;i++)
    {
        identity = NOW_TASK->taskTranslater[i]->userIdentity;
        QString title = "Task For "+identity;
        QTextEdit *newedit = new QTextEdit();
        SubEdit.push_back(newedit);
        newedit->setProperty("identity",identity);
        //qDebug()<<identity<<isok<<newedit->property("identity").toString();
        ui->ld_toolbox->addItem(newedit,title);
    }
}

//负责人提交任务
void TranWindow::setLeaderSubmitview(int num)
{
    tranwinRefresh();
    ui->stackedWidget->setCurrentIndex(6);
    ui->ls_toolbox->setItemText(0,"TaskSubmit");
    ui->ls_toolbox->setItemEnabled(0,false);
    QString identity;
    for(int i=0;i<num;i++)
    {
        identity = NOW_TASK->taskTranslater[i]->userIdentity;
        QString title = "Task Of "+identity;
        QTextBrowser *newbrowser = new QTextBrowser();
        //找到子任务
        QString subtaskname = NOW_TASK->taskName + identity;
        NOW_SUB = SubTaskInfo::getnowTask(ALL_SUBTASK,subtaskname);
        newbrowser->setText(NOW_SUB->subTran);
        SubRead.push_back(newbrowser);

        ui->ls_toolbox->addItem(newbrowser,title);
    }
}


void TranWindow::on_vd_confirmsubtaskbutton_released()
{
    u_LEADER *nowleader = new u_LEADER(*NOW_USER);
    bool isfull = true;
    for(int i=0;i<SubEdit.size();i++)
    {
        QString tempcon = SubEdit[i]->toPlainText();
        qDebug()<<"tran lead divide:"<<tempcon<<"??";
        if(tempcon.isEmpty())
        {
            isfull = false;
            QMessageBox::information(this,"prompt","Please Divide The Task!");
            this->close();
        }

    }
    if(isfull)
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this,
                 "question","Divide The Task As Above Now?");
        if(result == QMessageBox::Yes)
        {
            for(int i=0;i<SubEdit.size();i++)
            {
                QString tempcon = SubEdit[i]->toPlainText();
                QString identity = SubEdit[i]->property("identity").toString();
                //qDebug()<<"SubTask for "<<identity;
                UserInfo *mesrec = UserInfo::userFind(ALL_USERS,identity);

                nowleader->l_userDivideTask(NOW_TASK,mesrec,tempcon);
            }
            this->close();
        }

    }

    for(int i=0;i<SubEdit.size();i++)
    {
        delete SubEdit[i];
    }
    nowleader->userUPDATE(&CHANGE_USERS);
    nowleader->userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,
                                  nowleader->userIdentity);
    nowleader->userUPDATE(NOW_TASK->taskLeader);

}
//负责人暂存任务
void TranWindow::on_ls_savebutton_released()
{
    u_LEADER *nowleader = new u_LEADER(*NOW_USER);
    bool isfull = true;
    QString temptran = ui->ls_finaltranslateedit->toPlainText();
    if(temptran.isEmpty())
    {
        isfull = false;
        QMessageBox::information(this,"prompt","Please Edit The Task!");
    }
    if(isfull)
    {
        nowleader->l_userTrantemp(NOW_TASK,temptran);
    }
}

//负责人查看之前保存内容
void TranWindow::on_ls_lastsavebutton_released()
{
    QString lastsave = NOW_TASK->taskTrantemp;
    ui->ls_finaltranslateedit->setText(lastsave);
}


//刷新
void TranWindow::on_refreshbutton_released()
{

    setLeaderSubmitview(NOW_TASK->taskTranslater.size());
}

//发布者确认提交
void TranWindow::on_ls_submitconfirmbutton_released()
{
    u_LEADER *nowleader = new u_LEADER(*NOW_USER);
    bool isfull = true;
    QString finaltran = ui->ls_finaltranslateedit->toPlainText();
    if(finaltran.isEmpty())
    {
        isfull = false;
        QMessageBox::information(this,"prompt","Please Edit The Task!");
    }
    if(isfull)
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this,
                 "question","Submit The Task As Above Now?");
        if(result == QMessageBox::Yes)
        {
            nowleader->l_userSubmittask(NOW_TASK,finaltran);
            emit SubmitbackTOtask();
        }

    }


    for(int i=0;i<SubRead.size();i++)
    {
        delete SubRead[i];
    }
    nowleader->userUPDATE(&CHANGE_USERS);
    nowleader->userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,
                                  nowleader->userIdentity);
    nowleader->userUPDATE(NOW_TASK->taskLeader);

}


void TranWindow::setLeaderCheckview()
{
    tranwinRefresh();
    ui->stackedWidget->setCurrentIndex(3);
    ui->l_bookread->setText(NOW_SUB->subtaskContent);
    ui->l_translateridentity->setText(NOW_SUB->subTranslater->userIdentity);
    ui->l_commentedit->clear();
    ui->l_subtranbrowser->setText(NOW_SUB->subTran);
}

//负责人进行评论
void TranWindow::on_l_commentbutton_released()
{
    ui->l_commentwidget->show();
    ui->l_commentedit->clear();
}

//负责人确认评论
void TranWindow::on_l_commentconfirm_released()
{
    u_LEADER *newuser = new u_LEADER(*NOW_USER);
    QString comment = ui->l_commentedit->toPlainText();
    if(comment.isEmpty())
    {
        QMessageBox::information(this,"prompt","Please Edit Your Comment!");
    }
    else
    {
        QMessageBox::information(this,"prompt","Successfully Send Comment!");
        newuser->l_userFeedbacktask(NOW_SUB, comment);
        ui->l_commentedit->clear();
    }
    newuser->userUPDATE(&CHANGE_USERS);
    newuser->userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,
                                  newuser->userIdentity);
    newuser->userUPDATE(NOW_TASK->taskLeader);
}

//负责人查看往期评论
void TranWindow::on_l_commentbefore_released()
{
    ui->l_commentedit->clear();
    QString comment;
    for(int i=0;i<NOW_SUB->taskFeedback.size();i++)
    {
        comment =
                NOW_SUB->taskFeedback[i]->commentTime.toString("yyyy-MM-dd")
                + "\nFrom: " +NOW_SUB->taskFeedback[i]->commentPerson->userIdentity
                + "\n" + NOW_SUB->taskFeedback[i]->commentContent + "\n"
                + "---------------------\n"  + comment;
    }
    ui->l_commentedit->setText(comment);
}

//负责人查看译者
void TranWindow::on_l_translaterbutton_released()
{
    CheckUWin.show();
    QString identity = NOW_SUB->subTranslater->userIdentity;
    CheckUWin.checkuserfunction(identity);
}

void TranWindow::on_l_commentclosetool_released()
{
    ui->l_commentwidget->hide();
}


//译者本人任务
void TranWindow::settranview()
{
    tranwinRefresh();
    ui->stackedWidget->setCurrentIndex(4);
    ui->t_bookread->setText(NOW_SUB->subtaskContent);
}

//译者互译
void TranWindow::setotherTranview()
{
    tranwinRefresh();
    ui->stackedWidget->setCurrentIndex(5);
    ui->to_bookread->setText(NOW_SUB->subtaskContent);
    //其他译者只能查看已经提交的
    ui->to_booktranslate->setText(NOW_SUB->subTran);
    //ui->to_booktranslate->setEnabled(false);
    //其他译者发表评论

}

//译者查看上一次提交
void TranWindow::on_t_lastsubmitbutton_released()
{
    ui->t_booktranslate->setText(NOW_SUB->subTran);
}

//译者查看评论
void TranWindow::on_t_commentbutton_released()
{
    ui->t_commentwidget->show();
    u_TRAN *newuser = new u_TRAN(*NOW_USER);
    bool iscomment = newuser->t_usercheckComment(NOW_SUB);
    if(!iscomment)
    {
        //还没有评论
        QMessageBox::information(this,"prompt","No Comments Now!");
    }
    else
    {
        QString comment;
        for(int i=0;i<NOW_SUB->taskFeedback.size();i++)
        {
            comment =
                    NOW_SUB->taskFeedback[i]->commentTime.toString("yyyy-MM-dd")
                    + "\nFrom: " +NOW_SUB->taskFeedback[i]->commentPerson->userIdentity
                    + "\n" + NOW_SUB->taskFeedback[i]->commentContent + "\n"
                    + "---------------------\n"  + comment;
        }
        ui->t_commentbrowser->setText(comment);
    }

}

//译者暂存任务
void TranWindow::on_t_savebutton_released()
{
    u_TRAN *newuser = new u_TRAN(*NOW_USER);
    QString temptran = ui->t_booktranslate->toPlainText();
    if(temptran.isEmpty())
    {
        QMessageBox::information(this,"prompt","Please Check Your Task Again!");
    }
    else
    {
    newuser->t_userTrantemp(NOW_SUB,temptran);
    }
}

//译者提交任务
void TranWindow::on_t_submitbutton_released()
{
    u_TRAN *nowuser = new u_TRAN(*NOW_USER);
    QString finaltran = ui->t_booktranslate->toPlainText();
    if(finaltran.isEmpty())
    {
        //翻译部分为空
        QMessageBox::information(this,"prompt","Please Check Your Task Again!");
    }
    else
    {
        nowuser->t_userSubmittaskpart(NOW_SUB,finaltran);
    }
}

//译者查看评论
void TranWindow::on_t_commentclosetool_released()
{
    ui->t_commentwidget->close();
}

//译者查看上一次暂存
void TranWindow::on_t_lastsavebutton_released()
{
    ui->t_booktranslate->setText(NOW_SUB->subTrantemp);
}

//译者查看其他译者
void TranWindow::on_pushButton_2_released()
{
    CheckUWin.show();
    QString identity = NOW_SUB->subTranslater->userIdentity;
    CheckUWin.checkuserfunction(identity);
}

//译者评论其他译者
void TranWindow::on_pushButton_4_released()
{
    ui->to_commentwidget->show();
    ui->to_commentedit->clear();
}

void TranWindow::on_to_commentclosetool_released()
{
    ui->to_commentwidget->hide();
}

//译者查看往期评论（只能看自己的）
void TranWindow::on_to_commentbefore_released()
{
    ui->to_commentedit->clear();
    QString comment;
    for(int i=0;i<NOW_SUB->taskFeedback.size();i++)
    {
        if(NOW_SUB->taskFeedback[i]->commentPerson == NOW_USER)
        {
            comment =
                    NOW_SUB->taskFeedback[i]->commentTime.toString("yyyy-MM-dd")
                    + "\nFrom: " +NOW_SUB->taskFeedback[i]->commentPerson->userIdentity
                    + "\n" + NOW_SUB->taskFeedback[i]->commentContent + "\n"
                    + "---------------------\n"  + comment;
        }
    }
    ui->to_commentedit->setText(comment);
}

//译者确认评论
void TranWindow::on_to_commentconfirm_released()
{
    u_TRAN *newuser = new u_TRAN(*NOW_USER);
    QString comment = ui->to_commentedit->toPlainText();
    if(comment.isEmpty())
    {
        QMessageBox::information(this,"prompt","Please Edit Your Comment!");
    }
    else
    {
        QMessageBox::information(this,"prompt","Successfully Send Comment!");
        newuser->t_userCommentother(NOW_SUB, comment);
        ui->to_commentedit->clear();
    }
}


