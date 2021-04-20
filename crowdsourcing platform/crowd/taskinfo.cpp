/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2010-08-03
Description:任务类源文件
**************************************************/
#include "taskinfo.h"
#include "userinfo.h"


/*************************************************
Function:       SubTaskInfo
Description:    含参的子任务构造函数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          TaskInfo 原始任务，用于复制，UserInfo *，作为子任务译者指针
                QString 子任务文本
Output:         //
Return:         //
Others:         //
*************************************************/
SubTaskInfo::SubTaskInfo(const TaskInfo &a, UserInfo *tran, QString &content):
    TaskInfo(a),subtaskContent(content)
{
    subtaskName = a.taskName + tran->userIdentity;
    subTranslater = tran;
}

SubTaskInfo::SubTaskInfo(const TaskInfo &a):
    TaskInfo(a)
{

}

/*************************************************
Function:       getnowTask
Description:    定位当前子任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QVector<SubTaskInfo *> tasklist, 待查找的子任务数组
                QString &nowname，子任务唯一标识
Output:         //
Return:         //
Others:         //
*************************************************/
SubTaskInfo *SubTaskInfo::getnowTask(QVector<SubTaskInfo *> tasklist, QString &nowname)
{
    for(int i=0;i<tasklist.size();i++)
    {
        if(tasklist[i]->subtaskName == nowname)
        {
            return tasklist[i];
        }
    }
    return 0x0;

}


/*************************************************
Function:       taskUPDATE
Description:    更新子任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QVector<SubTaskInfo *> 待更新的子任务数组
Output:         //
Return:         //
Others:         //
*************************************************/
void SubTaskInfo::taskUPDATE(QVector<SubTaskInfo *> *tasklist)
{
    bool istask = false;
    for(int i=0;i<tasklist->size();i++)
    {
        if((*tasklist)[i]->subtaskName == this->subtaskName)
        {
            istask = true;
            (*tasklist)[i] = this;
            break;
        }
    }
    if(!istask)
    {
        tasklist->push_back(this);
    }
}


/*************************************************
Function:       Book
Description:    书籍类的含参构造函数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QVector<SubTaskInfo *> tasklist, 待查找的子任务数组
                QString &nowname，子任务唯一标识
Output:         //
Return:         //
Others:         //
*************************************************/
Book::Book(QString &borilan, QString &bname,
           QString &bcontent,QString &bpreread):
    bookName(bname),bookContent(bcontent),
    bookoriLan(borilan),bookPreread(bpreread)
{}



/*************************************************
Function:       taskUPDATE
Description:    更新任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QVector<SubTaskInfo *> tasklist, 待查找的子任务数组
Output:         //
Return:         //
Others:         //
*************************************************/
void TaskInfo::taskUPDATE(QVector<TaskInfo *> *tasklist)
{
    bool istask = false;
    for(int i=0;i<tasklist->size();i++)
    {
        if((*tasklist)[i]->taskName == this->taskName)
        {
            istask = true;
            (*tasklist)[i] = this;
            break;
        }
    }
    if(!istask)
    {
        tasklist->push_back(this);
    }
}


/*************************************************
Function:       getnowTask
Description:    定位当前任务
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QVector<SubTaskInfo *> tasklist, 待查找的任务数组
                QString &nowname，任务唯一标识
Output:         //
Return:         //
Others:         //
*************************************************/
TaskInfo *TaskInfo::getnowTask(QVector<TaskInfo *> tasklist, QString &nowname)
{
    for(int i=0;i<tasklist.size();i++)
    {
        if(tasklist[i]->taskName == nowname)
        {
            return tasklist[i];
        }
    }
    return 0x0;
}

/*************************************************
Function:       gettransignup
Description:    获取译者报名人数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         int 类型，译者报名人数
Others:         //
*************************************************/
int TaskInfo::gettransignup()const
{
    return taskTranslater.size();
}

/*************************************************
Function:       getleadsignup
Description:    获取负责人报名人数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         int 类型，负责人报名人数
Others:         //
*************************************************/
int TaskInfo::getleadsignup()const
{
    return leadSignup.size();
}

/*************************************************
Function:       getsubTasknum
Description:    获取子任务个数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         int 类型，子任务个数
Others:         //
*************************************************/
int TaskInfo::getsubTasknum()const
{
    return subTasknum;
}

/*************************************************
Function:       setsubTasknum
Description:    设定子任务个数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          int 类型，子任务个数
Output:         //
Return:         //
Others:         //
*************************************************/
void TaskInfo::setsubTasknum(int a)
{
    subTasknum = a;
}


/*************************************************
Function:       setTaskpaystate
Description:    设定任务支付状态
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          （枚举类型），任务支付状态
Output:         //
Return:         //
Others:         //
*************************************************/
void TaskInfo::setTaskpaystate(int a)
{
    taskPayState = a;
}

/*************************************************
Function:       setTaskcircle
Description:    设定任务周期
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          int 类型，任务周期
Output:         //
Return:         //
Others:         //
*************************************************/
void TaskInfo::setTaskcircle(int a)
{
    taskCircleDay = a;
}


/*************************************************
Function:       setTaskpay
Description:    设定任务酬金
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          float 类型，任务酬金
Output:         //
Return:         //
Others:         //
*************************************************/
void TaskInfo:: setTaskpay(float a)
{
    taskPay = a;
}

/*************************************************
Function:       setTaskpostday
Description:    设定任务发布时间
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QDate 类型，任务发布时间
Output:         //
Return:         //
Others:         //
*************************************************/
void TaskInfo::setTaskpostday(QDate &a)
{
    taskPostDay = a;
}

/*************************************************
Function:       setTaskleadday
Description:    设定子任务个数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QDate 类型，任务负责人报名截止时间
Output:         //
Return:         //
Others:         //
*************************************************/
void TaskInfo::setTaskleadday(QDate &a)
{
    taskLeadDay = a;
}

/*************************************************
Function:       setTaskstate
Description:    设定任务状态
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          int 类型，任务状态
Output:         //
Return:         //
Others:         //
*************************************************/
void TaskInfo::setTaskstate(int a)
{
    taskState = a;
}

/*************************************************
Function:       getTaskstate
Description:    获取任务状态
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         int 类型 任务状态
Others:         //
*************************************************/
int TaskInfo::getTaskstate()const
{
    return taskState;
}

/*************************************************
Function:       getTaskstatestr
Description:    获取QString任务状态
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         QString 类型 任务状态
Others:         //
*************************************************/
QString TaskInfo::getTaskstatestr()const
{
    QString qtstate;
    switch (taskState) {
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
        qtstate = "UNKNOWN";
        break;
    }
    return qtstate;

}

/*************************************************
Function:       getTaskpaystate
Description:    获取任务支付状态
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         int 类型 任务支付状态
Others:         //
*************************************************/
int TaskInfo::getTaskpaystate()const
{
    return taskPayState;
}

/*************************************************
Function:       getTaskpaystatestr
Description:    获取任务支付状态
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         QString 类型 任务支付状态
Others:         //
*************************************************/
QString TaskInfo::getTaskpaystatestr()const
{
    QString qtstate;
    switch (taskPayState) {
    case 0:
        qtstate = "NOTPAY";
        break;
    case 1:
        qtstate = "PAY";
        break;
    default:
        qtstate = "UNKNOWN";
        break;
    }
    return qtstate;
}


/*************************************************
Function:       getTaskcirxleday
Description:    获取任务周期
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         int 类型 任务周期
Others:         //
*************************************************/
int TaskInfo::getTaskcircleday()const
{
    return taskCircleDay;
}


/*************************************************
Function:       getTaskpay
Description:    获取任务酬金
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         float 类型 任务酬金
Others:         //
*************************************************/
float TaskInfo::getTaskpay()const
{
    return taskPay;
}

/*************************************************
Function:       getTaskpostday
Description:    获取任务发布时间
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         QDate 类型 任务发布时间
Others:         //
*************************************************/
QDate TaskInfo::getTaskpostday()const
{
    return taskPostDay;
}

/*************************************************
Function:       getTaskleadday
Description:    获取任务负责人报名截止时间
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         QDate 类型 任务负责人报名截止时间
Others:         //
*************************************************/
QDate TaskInfo::getTaskleadday()const
{
    return taskLeadDay;
}

/*************************************************
Function:       TaskInfo
Description:    任务构造函数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         //
Others:         初始化部分参数
*************************************************/
TaskInfo::TaskInfo()
{
    taskPoster = NULL;
    taskLeader = NULL;
    isDivide = NOTDIVIDE;
    isTranSignday = NOTSET;
}

/*************************************************
Function:       Comment
Description:    反馈构造函数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
Comment::Comment(QString &content, QString &taskname, QString &taskcontent,int count,UserInfo *person):
    commentContent(content),commentTask(taskcontent),
    count(count),commentPerson(person),commentTaskname(taskname)
{
    commentTime = QDateTime::currentDateTime();
}


/*************************************************
Function:       commentUPDATE
Description:    更新任务反馈
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
void Comment::commentUPDATE(QVector<Comment *> *commentlist)
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
    if(!iscomment)
    {
        commentlist->push_back(this);
    }
}
