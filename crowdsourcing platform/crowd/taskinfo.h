/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2010-08-03
Description:任务类头文件
**************************************************/
#ifndef TASKINFO_H
#define TASKINFO_H
#include <QString>
#include <QVector>
#include <Qdate>

/*枚举，任务状态*/
enum TASKSTATE
{
    WaitingLeader=0,//任务正在招募负责人
    WaitingTranslater,//任务正在招募译者
    Translating,//任务正在翻译
    Finished//任务结束
};
/*枚举，支付状态*/
enum PAYSTATE
{
    NOTPAY=0,//任务未支付
    PAY=1};//任务已支付
/*枚举，任务分割状态*/
enum DIVIDESTATE
{
    NOTDIVIDE=0,//任务未划分
    DIVIDE//任务已划分
};
/*枚举，任务译者报名截止时间*/
enum TRANDDLSTATE
{
    NOTSET=0,//任务未划分
    SET//任务已划分
};
class UserInfo;
class PersonMainWindow;
class SubTaskInfo;


/*Book类*/
class Book
{
    friend class TaskInfo;//友元类TaskInfo
public:
    QString bookName;//书籍名称
    QString bookContent;//书籍内容
    QString bookoriLan;//文本原始语言
    QString bookPreread;//文本试读

    Book(){}
    ~Book(){}
    Book(QString &borilan, QString &bname,
         QString &bcontent,QString &bpreread);

};

/*Comment类，子任务反馈*/
class Comment
{
public:
    int count;//反馈序列标记
    UserInfo *commentPerson;//反馈提出者
    QString commentContent;//反馈内容
    QDateTime commentTime;//反馈提出时间
    QString commentTask;//反馈针对的子任务译文
    QString commentTaskname;//反馈的子任务名称

    Comment(){count+=1;}
    //反馈的含参构造函数
    Comment(QString &content, QString &taskname,
            QString &taskcontent,int count,UserInfo *person);
    //反馈的更新函数
    void commentUPDATE(QVector<Comment *> *commentlist);
    ~Comment(){}
};

/*TaskInfo类，任务*/
class TaskInfo
{
    friend class u_POST;//友元类，发布者
    friend class u_Leader;//友元类，负责人
private:

    float taskPay;                        //任务酬金
    int taskCircleDay;                    //任务周期
    int taskPayState;                //任务支付状态
    int taskState;                   //任务状态
    int subTasknum;                  //子任务个数
public:
    UserInfo *taskPoster;                 //任务发布者
    UserInfo *taskLeader;                 //任务负责人
    QVector<UserInfo *> taskTranslater;   //任务译者

    QString taskName;                //任务名字，唯一标识
    Book *taskBook;                  //任务书籍
    QString tasktargetLan;           //任务目标语言
    QString taskIntro;               //任务描述
    QString taskTran;                //翻译后的任务内容
    QString taskTrantemp;            //翻译内容暂存
    QDate taskPostDay;                    //任务发起时间
    QDate taskLeadDay;                    //负责人报名截止时间
    QDate taskTranDay;                    //译者报名截止时间
    QString taskCover;                    //任务书籍封面
    QVector<UserInfo *> leadSignup;       //负责人报名人
    QVector<UserInfo *> tranSignup;       //译者报名人
    QVector<SubTaskInfo *> taskSub;       //任务子任务
    int isTranSignday;                    //任务译者报名截止时间是否确定？
    int isDivide;                         //任务是否划分？


    TaskInfo();
    virtual ~TaskInfo(){}
    //更新队列中的信息
    virtual void taskUPDATE(QVector <TaskInfo *> *tasklist);
    //定位任务
    static TaskInfo *getnowTask(QVector<TaskInfo *> tasklist, QString &nowname);
    //返回任务状态
    int getTaskstate()const;
    //返回任务支付状态
    int getTaskpaystate()const;
    //返回QString类型的任务状态
    QString getTaskstatestr()const;
    //返回QString类型的任务支付状态
    QString getTaskpaystatestr() const;
    //返回任务周期
    int getTaskcircleday()const;
    //返回任务酬金
    float getTaskpay()const;
    //返回任务发布时间
    QDate getTaskpostday()const;
    //返回任务负责人报名截止时间
    QDate getTaskleadday()const;
    //返回任务子任务个数
    int getsubTasknum()const;
    //设定任务状态
    void setTaskstate(int a);
    //设定任务支付状态
    void setTaskpaystate(int a);
    //设定任务周期
    void setTaskcircle(int a);
    //设定任务酬金
    void setTaskpay(float a);
    //设定任务发布时间
    void setTaskpostday(QDate &a);
    //设定负责人报名截止时间
    void setTaskleadday(QDate &a);
    //设定子任务个数
    void setsubTasknum(int a);
    //获得译者报名人数
    int gettransignup()const;
    //获得负责人报名人数
    int getleadsignup()const;

};

/*SubTaskInfo类，子任务*/
class SubTaskInfo : public TaskInfo
{
public:
    QString subtaskName;                //子任务唯一标识
    UserInfo *subTranslater;            //子任务译者
    QString subtaskContent;             //子任务内容
    QString subTrantemp;                //子任务暂存
    QString subTran;                    //子任务提交
    QVector<Comment *> taskFeedback;      //任务反馈


    SubTaskInfo(){}
    virtual ~SubTaskInfo(){}
    SubTaskInfo(const TaskInfo &a);
    SubTaskInfo(const TaskInfo &a,UserInfo *tran,QString &content);
    //定位子任务
    static SubTaskInfo *getnowTask(QVector<SubTaskInfo *> tasklist, QString &nowname);
    //更新子任务
    virtual void taskUPDATE(QVector<SubTaskInfo *> *subtasklist);
};



#endif // TASKINFO_H

