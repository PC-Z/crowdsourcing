/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2010-08-03
Description:用户类源文件
**************************************************/
#include "userinfo.h"
/*全局变量，存放所有的用户*/
extern QVector<UserInfo *> ALL_USERS;
/*全局变量，存放所有信息发生改变的用户*/
extern QVector<UserInfo *> CHANGE_USERS;
/*全局变量，存放所有的任务*/
extern QVector<TaskInfo *> ALL_TASKS;
/*全局变量，存放所有信息发生改变的任务*/
extern QVector<TaskInfo *> CHANGE_TASKS;
/*全局变量，存放所有的子任务*/
extern QVector<SubTaskInfo *> ALL_SUBTASK;
/*全局变量，存放所有信息发生改变的子任务*/
extern QVector<SubTaskInfo *> CHANGE_SUB;
/*全局变量，存放所有的用户评论*/
extern QVector<userComment *> ALL_USERCOMMENTS;
/*全局变量，存放所有信息发生改变的用户评论*/
extern QVector<userComment *> CHANGE_USERCOMMENTS;

//*******************************************************************//
//************************   译者类 u_TRAN   *************************//
//*******************************************************************//

u_TRAN::u_TRAN(UserInfo &a):
    UserInfo(a)
{}


/*************************************************
Function:       t_userTrantemp
Description:    译者暂存任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          SubTaskInfo * 译者操作的任务, QString 暂存的文本
Output:         //
Return:         //
Others:         //
*************************************************/
void u_TRAN::t_userTrantemp(SubTaskInfo *task, QString &trancon)
{
    task->subTrantemp = trancon;
    task->taskUPDATE(&CHANGE_SUB);
    task->taskUPDATE(&ALL_SUBTASK);
}

/*************************************************
Function:       t_userSubmitaskpart
Description:    译者提交任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          SubTaskInfo * 译者提交的任务, QString 提交的文本
Output:         //
Return:         //
Others:         //
*************************************************/
void u_TRAN::t_userSubmittaskpart(SubTaskInfo *task, QString &finaltran)
{
    task->subTran = finaltran;
    QString submitmes = "The Task "+task->taskName +"Is Submit!";
    mesSend(submitmes,task->taskLeader,this);
    task->taskUPDATE(&CHANGE_SUB);
    task->taskUPDATE(&ALL_SUBTASK);
}


/*************************************************
Function:       t_usercheckComment
Description:    译者暂存任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          SubTaskInfo * 译者查看的任务
Output:         //
Return:         bool 是否存在反馈
Others:         //
*************************************************/
bool u_TRAN::t_usercheckComment(SubTaskInfo *task)
{
    if(task->taskFeedback.size()==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}


/*************************************************
Function:       t_userCommenttoother
Description:    译者互校
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          SubTaskInfo * 译者操作的任务, QString 译者的反馈
Output:         //
Return:         //
Others:         //
*************************************************/
void u_TRAN::t_userCommentother(SubTaskInfo *task,QString &comment)
{
    int count = ALL_COMMENT.size();
    Comment *newcomment = new Comment(comment,task->subtaskName,
                                      task->subtaskContent,count+1,this);
    newcomment->commentUPDATE(&ALL_COMMENT);
    newcomment->commentUPDATE(&CHANGE_COMMENT);
    task->taskFeedback.push_back(newcomment);
    QString feedbackmes = "Feedback From Translater Of Task "+task->taskName;
    mesSend(feedbackmes,task->subTranslater);
    task->taskUPDATE(&CHANGE_SUB);
    task->taskUPDATE(&ALL_SUBTASK);
}

/*************************************************
Function:       t_userGetPaid
Description:    译者领取酬金
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          SubTaskInfo * 译者操作的任务
Output:         //
Return:         bool 是否领取成功
Others:         //
*************************************************/
bool u_TRAN::t_userGetPaid(TaskInfo *task)
{
    if(task->getTaskpaystate())
    {
        userMoney += (task->getTaskpay())/(task->taskTranslater.size())/2;
        QString getpaidmes = "The Task "+ task->taskName +
                " Translater"+NOW_USER->userIdentity+ " Gets Paid.";
        mesSend(getpaidmes,task->taskPoster);
        userUPDATE(&CHANGE_USERS);
        task->taskUPDATE(&CHANGE_TASKS);
        task->taskUPDATE(&ALL_TASKS);
        return true;
    }
    else
    {
        return false;
    }
}


//*******************************************************************//
//*********************   负责人类 u_LEADER   ************************//
//*******************************************************************//

/*************************************************
Function:       l_userSubmittask
Description:    负责人提交任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, QString 提交的文本
Output:         //
Return:         //
Others:         //
*************************************************/
void u_LEADER::l_userSubmittask(TaskInfo *task, QString &finaltran)
{
    task->taskTran = finaltran;
    QString submitmes = "The Task "+task->taskName +" Is Submit!";
    mesSend(submitmes,task->taskPoster,this);
    task->taskUPDATE(&CHANGE_TASKS);
    task->taskUPDATE(&ALL_TASKS);
}

/*************************************************
Function:       l_userTrantemp
Description:    负责人暂存任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, QString 暂存的文本
Output:         //
Return:         //
Others:         //
*************************************************/
void u_LEADER::l_userTrantemp(TaskInfo *task, QString &trancon)
{
    task->taskTrantemp = trancon;
    task->taskUPDATE(&CHANGE_TASKS);
    task->taskUPDATE(&ALL_TASKS);
}

/*************************************************
Function:       l_userFeedbacktask
Description:    负责人反馈任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, QString 反馈内容
Output:         //
Return:         //
Others:         //
*************************************************/
void u_LEADER::l_userFeedbacktask(SubTaskInfo *subtask, QString &feedback)
{
    int count = ALL_COMMENT.size();
    Comment *newcomment = new Comment(feedback,subtask->subtaskName,
                                      subtask->subtaskContent,count+1,this);
    newcomment->commentUPDATE(&ALL_COMMENT);
    newcomment->commentUPDATE(&CHANGE_COMMENT);
    subtask->taskFeedback.push_back(newcomment);
    QString feedbackmes = "Feedback From Leader Of Task "+subtask->taskName;
    mesSend(feedbackmes,subtask->subTranslater);
    subtask->taskUPDATE(&CHANGE_SUB);
    subtask->taskUPDATE(&ALL_SUBTASK);
}

/*************************************************
Function:       l_userChecksubtask
Description:    负责人检查任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          SubTaskInfo * 负责人操作的任务
Output:         //
Return:         //
Others:         //
*************************************************/
bool u_LEADER::l_userChecksubtask(SubTaskInfo *subtask)
{
    if(subtask->subTran.isEmpty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*************************************************
Function:       l_userDivideTask
Description:    负责人划分任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, UserInfo *tran 指定的译者
                QString 指定的子任务文本
Output:         //
Return:         //
Others:         //
*************************************************/
void u_LEADER::l_userDivideTask(TaskInfo *task, UserInfo *tran, QString &taskcon)
{
    SubTaskInfo *newsubtask = new SubTaskInfo(*task, tran, taskcon);
    newsubtask->subtaskName = task->taskName + tran->userIdentity;

    l_userChangeState(task,Translating);
    //task->taskSub.push_back(newsubtask);
    newsubtask->taskUPDATE(&(task->taskSub));
    newsubtask->taskUPDATE(&(tran->userTran));
    task->taskUPDATE(&(tran->userTasks));
    //tran->userTran.push_back(newsubtask);
    //tran->userTasks.push_back(newsubtask);
    task->isDivide = DIVIDE;
    QString submes = "You Have A New Translating Task!";
    mesSend(submes,tran);

    //系统更新
    newsubtask->taskUPDATE(&ALL_SUBTASK);
    newsubtask->taskUPDATE(&CHANGE_SUB);

}

/*************************************************
Function:       l_userChangeState
Description:    负责人修改任务状态
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, TASKSTATE 任务状态
Output:         //
Return:         //
Others:         //
*************************************************/
bool u_LEADER::l_userChangeState(TaskInfo *targettask, TASKSTATE newstate)
{
    if((newstate != targettask->getTaskstate())&&(newstate == Translating))
    {
        targettask->setTaskstate(Translating);
        QString traningmes = "The Task "+targettask->taskName+"Is Translating!";
        mesSend(traningmes, this);
        mesSend(traningmes,targettask->taskPoster);
        for(int i=0;i<targettask->taskTranslater.size();i++)
        {
            mesSend(traningmes,targettask->taskTranslater[i]);
        }
        targettask->taskUPDATE(&CHANGE_TASKS);
        targettask->taskUPDATE(&ALL_TASKS);
        return true;
    }
    return false;
        //NOW_TASK->taskState = Translating;
}


/*************************************************
Function:       l_userDecideTran
Description:    负责人确认译者
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, UserInfo *指定的译者
                int 申请次数检查
Output:         //
Return:         //
Others:         当负责人想要在截止日期之前确认译者，则需要int类型数据的改变
*************************************************/
bool u_LEADER::l_userDecideTran(TaskInfo *task, UserInfo *tran, int times)
{
    bool isddl = false;
    //确认是否在截止日期
    QDate nowdate = QDate::currentDate();
    if(nowdate > task->taskTranDay)
    {
        isddl = true;
    }

    if((times>0)||(isddl)==true)
    {
        tran->userUPDATE(&(task->taskTranslater));
        //task->taskTranslater.push_back(tran);
        //向负责人发送聘请信息
        QString tranmes = "Now You Are Translater of Task "+task->taskName;
        mesSend(tranmes,tran,this);
        //告知发布者
        QString tranmestopost = tran->userIdentity +"Is One Of The Translaters Of Task "+task->taskName;
        mesSend(tranmestopost,task->taskPoster,this);

        //译者任务增加，系统修改
        tran->usertrannum += 1;
        tran->userTasks.push_back(task);
        //tran->userTran.push_back(dynamic_cast<SubTaskInfo *>(task));
        tran->userUPDATE(&CHANGE_USERS);
        task->taskUPDATE(&CHANGE_TASKS);
        task->taskUPDATE(&ALL_TASKS);
        return true;
    }
    else
    {
        return false;
    }
}

/*************************************************
Function:       l_userCheckSignup
Description:    负责人查看报名译者
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, int num 译者所处位置
Output:         //
Return:         UserInfo *
Others:         //
*************************************************/
UserInfo *u_LEADER::l_userCheckSignup(TaskInfo *task, int num)
{
    userUPDATE(&CHANGE_USERS);
    userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,this->userIdentity);
    if((num < task->tranSignup.size())&&(num>=0))
    {
        return task->tranSignup[num];
    }
    else
    {
        return 0;
    }
}

/*************************************************
Function:       l_usersetTransignday
Description:    负责人确定译者报名截止时间
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, QDate 截止时间
Output:         //
Return:         //
Others:         //
*************************************************/
bool u_LEADER::l_usersetTransignday(TaskInfo *mytask, QDate &day)
{
    if(day < QDate::currentDate())
    {
        //ddl时间不对
        return false;
    }
    else
    {
        mytask->taskTranDay = day;
        mytask->isTranSignday = SET;
        mytask->taskUPDATE(&CHANGE_TASKS);
        return true;
    }
}

/*************************************************
Function:       l_userGetPaid
Description:    负责人领取酬金
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务
Output:         //
Return:         bool 负责人是否领取成功
Others:         //
*************************************************/
bool u_LEADER::l_userGetPaid(TaskInfo *a)
{
    if(a->getTaskpaystate())
    {
        userMoney += (a->getTaskpay())/2;
        QString getpaidmes = "The Task "+ a->taskName +"Leader Gets Paid.";
        mesSend(getpaidmes,a->taskPoster);
        userUPDATE(&CHANGE_USERS);
        userUPDATE(&ALL_USERS);
        NOW_USER = UserInfo::userFind(ALL_USERS,this->userIdentity);
        a->taskUPDATE(&CHANGE_TASKS);
        return true;
    }
    else
    {
        return false;
    }
}

u_LEADER::u_LEADER(UserInfo &a):
    UserInfo(a)
{}

//*******************************************************************//
//**********************   发布者类 u_POST   *************************//
//*******************************************************************//

u_POST::u_POST(UserInfo &a):
    UserInfo(a)
{}

/*************************************************
Function:       p_userCheckSignup
Description:    负责人提交任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, int 负责人所处位置
Output:         //
Return:         UserInfo 负责人报名者用户指针
Others:         //
*************************************************/
UserInfo *u_POST::p_userCheckSignup(TaskInfo *a, int num)
{
    userUPDATE(&CHANGE_USERS);
    userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,this->userIdentity);
    if((num < a->leadSignup.size())&&(num>=0))
    {
        return a->leadSignup[num];
    }
    else
    {
        return 0;
    }
}


/*************************************************
Function:       p_userDecideLeader
Description:    发布者确认负责人
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 负责人操作的任务, UserInfo *确认的用户指针
                int 申请次数
Output:         //
Return:         bool 是否确认成功
Others:         若发布者想要在截止时间之前确认，则需要修改申请次数
*************************************************/
bool u_POST::p_userDecideLeader(TaskInfo *a, UserInfo *leader,int times)
{
    bool isddl = false;
    //确认是否在截止日期
    QDate nowdate = QDate::currentDate();
    if(nowdate > a->taskLeadDay)
    {
        isddl = true;
    }

    if((times>0)||(isddl)==true)
    {
        a->taskLeader = leader;
        //向负责人发送聘请信息
        QString leadermes = "Now You Are Leader of Task"+a->taskName;
        mesSend(leadermes,leader,this);
        //修改任务状态
        p_userChangeState(a,WaitingTranslater);
        //负责人负责任务增加，系统修改
        leader->userleadnum += 1;
        a->taskUPDATE(&(leader->userTasks));
        a->taskUPDATE(&(leader->userLead));
//        leader->userTasks.push_back(a);
//        leader->userLead.push_back(a);
        userUPDATE(&CHANGE_USERS);
        userUPDATE(&ALL_USERS);
        NOW_USER = UserInfo::userFind(ALL_USERS,this->userIdentity);
        leader->userUPDATE(&CHANGE_USERS);
        a->taskUPDATE(&CHANGE_TASKS);
        //qDebug()<<"decide"<<(CHANGE_USERS).size();

        return true;
    }
    else
    {
        return false;
    }
}

/*************************************************
Function:       p_userChecktask
Description:    发布者查看任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 发布者操作的任务
Output:         //
Return:         bool 是否有内容
Others:         //
*************************************************/
bool u_POST::p_userChecktask(TaskInfo *task)
{
    userUPDATE(&CHANGE_USERS);
    userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,this->userIdentity);
    if(task->taskTran.isEmpty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*************************************************
Function:       p_userChangeState
Description:    发布者修改任务状态
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 发布者操作的任务, TASKSTATE 任务新状态
                int 申请次数
Output:         //
Return:         //
Others:         //
*************************************************/
void u_POST::p_userChangeState(TaskInfo *targettask, TASKSTATE newstate)
{
    int before = targettask->getTaskstate();
    if(newstate != before)
    {
        QString qtstate;
        switch (newstate) {
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
        targettask->setTaskstate(newstate);
        if(newstate == Finished)
        {
            if(targettask->taskLeader)
            {
                targettask->taskLeader->useraddCredit(2);
                targettask->taskLeader->userUPDATE(&ALL_USERS);
                targettask->taskLeader->userUPDATE(&CHANGE_USERS);
                for(int i=0;i<targettask->taskTranslater.size();i++)
                {
                    targettask->taskTranslater[i]->useraddCredit(1);
                    targettask->taskTranslater[i]->userUPDATE(&ALL_USERS);
                    targettask->taskTranslater[i]->userUPDATE(&CHANGE_USERS);
                }
            }
        }
        QString changemes = "The Task "+targettask->taskName+" Is "+qtstate;
        this->mesSend(changemes,this);
        if(newstate!=WaitingLeader)
        {
            this->mesSend(changemes,targettask->taskLeader);
            if((newstate == WaitingTranslater)||(newstate==Finished))
            {
                for(int i=0;i<targettask->taskTranslater.size();i++)
                {
                    this->mesSend(changemes,targettask->taskTranslater[i]);
                }

            }
        }

        //NOW_TASK->taskState = Finished;
    }
    userUPDATE(&CHANGE_USERS);
    userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,this->userIdentity);
    targettask->taskUPDATE(&CHANGE_TASKS);
    targettask->taskUPDATE(&ALL_TASKS);
}

/*************************************************
Function:       p_userPayLeader
Description:    发布者确认支付酬金
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 发布者操作的任务
Output:         //
Return:         //
Others:         //
*************************************************/
void u_POST::p_userPayLeader(TaskInfo *task)
{
    p_userChangeState(task,Finished);
    task->setTaskpaystate(PAY);

    QString changemes = "The Task "+task->taskName+"Is Paid";
    this->mesSend(changemes,this);
    this->mesSend(changemes,task->taskLeader);
    for(int i=0;i<task->taskTranslater.size();i++)
    {
        this->mesSend(changemes,task->taskTranslater[i]);
    }
    userUPDATE(&CHANGE_USERS);
    userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,this->userIdentity);
    task->taskUPDATE(&CHANGE_TASKS);
}







/*************************************************
Function:       userFindInterest
Description:    用户推荐任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         QVector<TaskInfo *>
Others:         //
*************************************************/
QVector<TaskInfo *> UserInfo::userFindInterest()
{
    QVector <TaskInfo *> tasklist;

    for(int i=0;i<ALL_TASKS.size();i++)
    {
        bool notmy = true;
        for(int j=0;j<userTasks.size();j++)
        {
            if(ALL_TASKS[i] == userTasks[j])
            {
                notmy = false;
                break;
            }
        }
        if(notmy)
        {
            for(int j=0;j<userTasks.size();j++)
            {
                if((ALL_TASKS[i]->tasktargetLan == userTasks[j]->tasktargetLan)
                   || (ALL_TASKS[i]->taskBook->bookoriLan == userTasks[j]->taskBook->bookoriLan)
                   || (ALL_TASKS[i]->tasktargetLan == userTasks[j]->taskBook->bookoriLan)
                   || (ALL_TASKS[i]->taskBook->bookoriLan == userTasks[j]->tasktargetLan))
                {
                    tasklist.push_back(ALL_TASKS[i]);
                    break;
                }
            }
        }
    }
    return tasklist;
}


/*************************************************
Function:       userSignupLead
Description:    用户报名成为负责人
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 用户报名的任务
Output:         //
Return:         int 用户报名情况
Others:         //
*************************************************/
int UserInfo::userSignupLead(TaskInfo *task)
{
    if(QDate::currentDate() > task->taskLeadDay)
    {
        return l_Overdue;
    }
    else if(userCredit < LEADERCREDIT)
    {
        return l_LackCredit;
    }
    else if(task->taskPoster->userIdentity == userIdentity)
    {
        return l_Poster;
    }
    else
    {
        bool flag = true;
        for(int i=0;i<task->leadSignup.size();i++)
        {
            if(task->leadSignup[i]->userIdentity == userIdentity)
            {
                flag = false;
                return l_AlreadySignup;
            }
        }
        for(int i=0;i<task->tranSignup.size();i++)
        {
            if(task->tranSignup[i]->userIdentity == userIdentity)
            {
                flag = false;
                return l_AlreadySignupTran;
            }
        }
        if(flag)
        {
            //报名成功
            task->leadSignup.push_back(this);
            userSignLead.push_back(task);
            QString signupmes = "A New Leader Signs up For The Task "+task->taskName;
            mesSend(signupmes,task->taskPoster);
            userUPDATE(&CHANGE_USERS);
            task->taskUPDATE(&CHANGE_TASKS);
            return l_Success;
        }
        return 0x0;
    }

}

/*************************************************
Function:       userSignupTran
Description:    用户报名成为译者
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo * 用户报名的任务
Output:         //
Return:         int 用户报名情况
Others:         //
*************************************************/
int UserInfo::userSignupTran(TaskInfo *task)
{
    if(QDate::currentDate() > task->taskTranDay)
    {
        return t_Overdue;
    }
    else if((task->taskLeader)&&(task->taskLeader->userIdentity == userIdentity))
    {
        return t_Leader;
    }
    else if(task->taskPoster->userIdentity == userIdentity)
    {
        return t_Poster;
    }
    else
    {
        bool flag = true;
        for(int i=0;i<task->tranSignup.size();i++)
        {
            if(task->tranSignup[i]->userIdentity == userIdentity)
            {
                flag = false;
                return t_AlreadySignup;
            }
        }
        for(int i=0;i<task->leadSignup.size();i++)
        {
            if(task->leadSignup[i]->userIdentity == userIdentity)
            {
                flag = false;
                return t_AlreadySignupLeader;
            }
        }
        if(flag)
        {
            //报名成功
            task->tranSignup.push_back(this);
            userSignTran.push_back(task);
            QString signupmes = "A New Translater Signs up For The Task "+task->taskName;
            mesSend(signupmes ,task->taskPoster);
            if(task->getTaskstate() == WaitingTranslater)
            {
                mesSend(signupmes,task->taskLeader);
            }
            userUPDATE(&CHANGE_USERS);
            task->taskUPDATE(&CHANGE_TASKS);
            return t_Success;
        }
    }

    return 0x0;
}



/*************************************************
Function:       mesSend
Description:    用户发送消息
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QString ,用户发送的内容
                UserInfo *用户消息发送的对象,
                UserInfo *发送消息的用户
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::mesSend(QString &content,
        UserInfo *receiver, UserInfo *sender)
{
    Message *newmes = new Message(content,receiver,sender);
    receiver->userRecmes.push_back(newmes);
    sender->userSendmes.push_back(newmes);
    newmes->mesUPDATE(&ALL_MES);
    newmes->mesUPDATE(&CHANGE_MES);
    //qDebug()<<"this message is from "<<sender<<sender->userIdentity;

}


/*************************************************
Function:       mesSend
Description:    系统发送消息
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QString ,用户发送的内容
                UserInfo *用户消息发送的对象,
Output:         //
Return:         //
Others:         重载了用户发送消息函数
*************************************************/
void UserInfo::mesSend(QString &content, UserInfo *receiver)
{
    Message *newmes = new Message(content,receiver,0x0);
    receiver->userRecmes.push_back(newmes);
    newmes->mesUPDATE(&ALL_MES);
    newmes->mesUPDATE(&CHANGE_MES);
    //qDebug()<<content<<"hello system";
}

/*************************************************
Function:       mesunRead
Description:    用户检查未读消息
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         int 未读消息总数
Others:         //
*************************************************/
int UserInfo::mesunRead()
{
    int newmessum = 0;
    for(int i=0;i<userRecmes.size();i++)
    {
        if((userRecmes[i]->mesisRead) == false)
        {
            //未读消息
            newmessum +=1;
        }
    }
    return newmessum;
}

UserInfo::UserInfo()
{
    userMoney = 0;
    userCredit = 0;
    userpixadd = "://photouserini.jpg";
    userleadnum = 0;
    usertrannum = 0;
}

/*************************************************
Function:       userUPDATE
Description:    用户更新
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          UserInfo *待更新的用户指针
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::userUPDATE(UserInfo *targetuser)
{
    if(userIdentity == targetuser->userIdentity)
    {
        targetuser = this;
    }
}

/*************************************************
Function:       userUPDATE
Description:    用户更新
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QVector<UserInfo *>待更新的用户数组
Output:         //
Return:         //
Others:         重载了上一函数
*************************************************/
void UserInfo::userUPDATE(QVector<UserInfo *> *userlist)
{
    bool isuser = false;
    for(int i=0;i<userlist->size();i++)
    {
        if((*userlist)[i]->userIdentity == this->userIdentity)
        {

            isuser = true;
            (*userlist)[i] = dynamic_cast<UserInfo *>(this);
            break;
        }
    }
    if(isuser == false)
    {
        userlist->push_back(dynamic_cast<UserInfo *>(this));
    }

}

/*************************************************
Function:       userFind
Description:    用户定位
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QVector<UserInfo *>待查找的用户数组
                QString 用户标识
Output:         //
Return:         //
Others:         //
*************************************************/
UserInfo *UserInfo::userFind(QVector<UserInfo *> a, QString identity)
{
    bool isfind = false;
    for(int i=0;i<a.size();i++)
    {
        if(a[i]->userIdentity == identity)
        {
            isfind = true;
            return a[i];
        }
    }
    if(!isfind)
    {
        return NULL;
    }
    return NULL;
}



/*************************************************
Function:       useraddCredit
Description:    用户积分增加
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          int 增加的积分
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::useraddCredit(int credit)
{
    userCredit += credit;
}

/*************************************************
Function:       usersetCredit
Description:    用户初始化积分
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          int 用户初始化的积分值
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::usersetCredit(int credit)
{
    userCredit = credit;
}

/*************************************************
Function:       applyuserFriend
Description:    用户发送好友申请
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          UserInfo *申请好友的用户对象
Output:         //
Return:         bool 申请是否成功
Others:         //
*************************************************/
bool UserInfo::applyuserFriend(UserInfo *targetuser)
{
    bool notfriend = true;
    for(int i=0;i<userFriends.size();i++)
    {
        if(userFriends[i] == targetuser)
        {
            notfriend = false;
            return false;
        }
    }
    if(notfriend)
    {
        targetuser->userRecfriend.push_back(this);
        userSendfriend.push_back(targetuser);

        userUPDATE(&ALL_USERS);
        userUPDATE(&CHANGE_USERS);
        targetuser->userUPDATE(&ALL_USERS);
        targetuser->userUPDATE(&CHANGE_USERS);
        return true;
    }
}

/*************************************************
Function:       adduserFriend
Description:    用户添加好友
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          UserInfo *待添加的用户好友
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::adduserFriend(UserInfo *targetuser)
{
    targetuser->userFriends.push_back(this);
    userFriends.push_back(targetuser);


    int recindex = targetuser->userRecfriend.indexOf(this);
    qDebug()<<"recnum"<<recindex;
    for(int i=recindex;i<targetuser->userRecfriend.size();i++)
    {
        if((i+1)<targetuser->userRecfriend.size())
        {
            targetuser->userRecfriend[i] = targetuser->userRecfriend[i+1];
        }
    }
    targetuser->userRecfriend.pop_back();
    int sendindex = userSendfriend.indexOf(targetuser);
    for(int i=sendindex;i<targetuser->userSendfriend.size();i++)
    {
        if((i+1)<targetuser->userSendfriend.size())
        {
            targetuser->userSendfriend[i] = targetuser->userSendfriend[i+1];
        }
    }
    targetuser->userSendfriend.pop_back();


    userUPDATE(&ALL_USERS);
    userUPDATE(&CHANGE_USERS);
    targetuser->userUPDATE(&ALL_USERS);
    targetuser->userUPDATE(&CHANGE_USERS);
}


/*************************************************
Function:       refuseuserFriend
Description:    拒绝用户好友申请
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          UserInfo *待拒绝的用户好友
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::refuseuserFriend(UserInfo *targetuser)
{
    int recindex = targetuser->userRecfriend.indexOf(this);
    for(int i=recindex;i<targetuser->userRecfriend.size();i++)
    {
        if((i+1)<targetuser->userRecfriend.size())
        {
            targetuser->userRecfriend[i] = targetuser->userRecfriend[i+1];
        }
    }
    targetuser->userRecfriend.pop_back();
    int sendindex = userSendfriend.indexOf(targetuser);
    for(int i=sendindex;i<targetuser->userSendfriend.size();i++)
    {
        if((i+1)<targetuser->userSendfriend.size())
        {
            targetuser->userSendfriend[i] = targetuser->userSendfriend[i+1];
        }
    }
    targetuser->userSendfriend.pop_back();


    userUPDATE(&ALL_USERS);
    userUPDATE(&CHANGE_USERS);
    targetuser->userUPDATE(&ALL_USERS);
    targetuser->userUPDATE(&CHANGE_USERS);
}


//评价构造函数
userComment::userComment(UserInfo *postuser, UserInfo *targetuser, QString content):
    postUser(postuser),targetUser(targetuser),commentContent(content)
{
    count += 1;
}

/*************************************************
Function:       userCommentUPDATE
Description:    用户评价更新
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QVector<userComment *> *待更新的数组指针
Output:         //
Return:         //
Others:         //
*************************************************/
void userComment::userCommentUPDATE(QVector<userComment *> *commentlist)
{
    bool iscomment = false;
    for(int i=0;i<commentlist->size();i++)
    {
        if((*commentlist)[i]->count == this->count)
        {
            iscomment = true;
            (*commentlist)[i] = this;
            break;
        }
    }
    if(iscomment == false)
    {
        commentlist->push_front(this);
    }
}


/*************************************************
Function:       adduserComments
Description:    用户评价其他用户
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          UserInfo *待评价的用户
                QString 评价内容
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::adduserComments(UserInfo *target,QString &mycomment)
{    
    userComment *newcomment = new userComment(this,target,mycomment);
    target->userComments.push_front(newcomment);
    newcomment->userCommentUPDATE(&ALL_USERCOMMENTS);
    newcomment->userCommentUPDATE(&CHANGE_USERCOMMENTS);
}

/*************************************************
Function:       getuserMoney
Description:    获取用户余额
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         float 用户余额
Others:         //
*************************************************/
float UserInfo::getuserMoney()const
{
    return userMoney;
}

/*************************************************
Function:       getuserPsd
Description:    获取用户密码
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         QString 用户密码
Others:         //
*************************************************/
QString UserInfo::getuserPsd()const
{
    return userPassword;
}

/*************************************************
Function:       setuserCreditIni
Description:    根据用户填写的语言资质证明给予积分
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QString 用户语言资质证明
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::setuserCreditIni(QString &qualification)
{
    //根据qualification进行判断
    int qualifysum = 0;
    QStringList qualifylist;
    qualifylist<<"tofel"<<"托福"<<"ielts"<<"雅思"
              <<"gre"<<"英语"<<"法语";
    QStringListIterator itqualifylist(qualifylist);
    while(itqualifylist.hasNext())
    {
        if(qualification.contains(itqualifylist.next(),Qt::CaseInsensitive))
        {
            qualifysum += 1;
        }
    }
    userCredit = 4*qualifysum;
}

/*************************************************
Function:       setuserMoney
Description:    设置用户余额
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          float 用户余额
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::setuserMoney(float a)
{
    userMoney += a;
    userUPDATE(&(ALL_USERS));
    userUPDATE(&(CHANGE_USERS));
}

/*************************************************
Function:       setuserPassword
Description:    设置用户密码
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QString 用户密码
Output:         //
Return:         //
Others:         //
*************************************************/
void UserInfo::setuserPassword(QString a)
{
    userPassword = a;
}

/*************************************************
Function:       userLogin
Description:    用户登录
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QString 用户账号
                QString 用户密码
Output:         //
Return:         //
Others:         //
*************************************************/
int UserInfo::userLogin(QString &accountstr, QString &psd)
{
    bool isfind = false;
    int find = 1;
    int account = accountstr.toInt();
    if(accountstr.isEmpty())
    {
        //输入账号为空
        find = NullAccount;
    }
    else if(psd.isEmpty())
    {
        //输入密码为空
        find = NullPassword;
    }
    else
    {
        for(int i=0;i<ALL_USERS.size();i++)
        {
            if(ALL_USERS[i]->userAccount == account)
            {
                isfind = true;
                if(ALL_USERS[i]->getuserPsd() == psd)
                {
                    qDebug()<<"成功登录！"<<"账号："<<ALL_USERS[i]->userAccount
                           <<"密码："<<ALL_USERS[i]->getuserPsd();
                    //账号密码均正确
                    find = Success;
                    NOW_USER = ALL_USERS[i];
                    qDebug()<<"post"<<NOW_USER->userPost.size()
                           <<"lead"<<NOW_USER->userLead.size()
                          <<"tran"<<NOW_USER->userTran.size()
                         <<"all task"<<NOW_USER->userTasks.size();
                }
                else if(ALL_USERS[i]->getuserPsd() != psd)
                {
                    qDebug()<<"密码错误！";
                    find = WrongPassword;
                }
            }
        }
        if(!isfind)
        {
            qDebug()<<"无效用户名！"<<ALL_USERS[0]->userAccount;
            find = WrongAccount;
        }
    }
    return find;
}

