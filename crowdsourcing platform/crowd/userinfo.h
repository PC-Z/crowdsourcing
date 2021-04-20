/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2010-08-03
Description:用户类头文件
**************************************************/
#ifndef USERINFO_H
#define USERINFO_H
#define LEADERCREDIT 4
#define CHECKERCREDIT 8

#include <QString>
#include <Qmap>
#include <QDate>
#include <QDebug>
#include "taskinfo.h"
#include "message.h"

class postTaskWindow;
class MessageBOX;
class Message;
class TaskInfo;

/*枚举，报名负责人可能出现的结果*/
enum SignLead
{
    l_Success=0,//报名负责人成功
    l_LackCredit,//缺乏积分
    l_AlreadySignup,//已经报名负责人
    l_Overdue,//过期
    l_Poster,//已经是任务发布者
    l_AlreadySignupTran//已经报名任务译者
};
/*枚举，报名译者可能出现的结果*/
enum SignTran
{
    t_Success,//报名译者成功
    t_AlreadySignup,//已经报名
    t_Overdue,//过期
    t_Poster,//已经是任务发布者
    t_Leader,//已经是任务负责人
    t_AlreadySignupLeader//已经报名负责人
};
/*枚举，用户登录可能出现的结果*/
enum LoginState
{
    Success=0,//登陆成功
    NullAccount,//账号为空
    NullPassword,//密码为空
    WrongAccount,//无效用户名
    WrongPassword//错误密码
};

/*用户评论类*/
class userComment
{
public:
    static int count;
    UserInfo *targetUser;       //被评论的用户
    UserInfo *postUser;         //发表评价的用户
    QString commentContent;     //评论内容

    userComment(){count +=1;}
    ~userComment(){}
    userComment(UserInfo *postuser, UserInfo *targetuser, QString content);
    void userCommentUPDATE(QVector<userComment *> *commentlist);
};

/*用户类*/
class UserInfo
{
protected:
    QString userPassword;       //用户密码
    float userMoney;            //用户余额
public:
    int userAccount;            //用户账号
    QString userIdentity;       //用户昵称(uni)
    QString userQualification;  //用户语言资质
    QString userSex;            //用户性别
    QDate userBirthday;         //用户生日
    QString userEmail;          //用户邮箱
    QString userpixadd;         //用户头像路径
    int userCredit;             //用户积分
    int userleadnum;            //用户负责的任务总数
    int usertrannum;            //用户翻译的任务总数
    //**********待补充
    QVector<UserInfo *> userFriends;        //用户好友
    QVector<UserInfo *> userSendfriend;     //用户发出好友请求
    QVector<UserInfo *> userRecfriend;      //用户收到好友请求
    QVector<userComment *> userComments;    //用户评价标签
    QVector<Message *> userRecmes;          //用户收到的消息
    QVector<Message *> userSendmes;         //用户发送的消息
    QVector<TaskInfo *> userTasks;          //用户任务列表
    QVector<TaskInfo *> userPost;           //用户发布的任务
    QVector<TaskInfo *> userLead;           //用户负责的任务
    QVector<SubTaskInfo *> userTran;        //用户翻译的任务
    QVector<TaskInfo *> userSignLead;       //用户报名成为负责人的任务
    QVector<TaskInfo *> userSignTran;       //用户报名成为译者的任务

    //用户登录
    static int userLogin(QString &account , QString &psd);
    //确定用户初始积分值
    void setuserCreditIni(QString &a);

public:
    UserInfo();
    ~UserInfo(){}
    //更新队列中的信息
    void userUPDATE(QVector <UserInfo *> *userlist);
    //更新某个用户信息
    void userUPDATE(UserInfo *);
    //设置用户密码
    void setuserPassword(QString a);
    //用户充值
    void setuserMoney(float a);
    //获取用户密码
    QString getuserPsd()const;
    //获取用户余额
    float getuserMoney()const;
    //用户改变个人信息（在personmainwindow实现）
    void userchangeinfo(PersonMainWindow *a);
    //定位用户
    static UserInfo *userFind(QVector<UserInfo *> a,QString identity);


//**********************************************************//
//******************* 用户交互与消息处理  *********************//
//**********************************************************//
    //评论其他用户
    void adduserComments(UserInfo *target,QString &comment);
    //拒绝好友申请
    void refuseuserFriend(UserInfo *targetuser);
    //添加用户为好友
    void adduserFriend(UserInfo *targetuser);
    //发送交友消息
    bool applyuserFriend(UserInfo *targetuser);
    //向其他用户发送消息
    void mesSend(QString &content,UserInfo *receiver, UserInfo *sender);
    //系统对用户发送信息
    void mesSend(QString &content,UserInfo *receiver);
    //用户检查未读消息
    int mesunRead();


//**********************************************************//
//********************** 任务处理 ***************************//
//**********************************************************//
    //用户发布任务
    bool userPostTask(postTaskWindow *a);
    //用户报名负责人
    int userSignupLead(TaskInfo *task);
    //用户报名译者
    int userSignupTran(TaskInfo *task);
    //用户积分增加
    void useraddCredit(int credit);
    //用户初始化积分
    void usersetCredit(int credit);
    //用户推荐任务表
    QVector<TaskInfo *> userFindInterest();
};

/*发布者类*/
class u_POST : public UserInfo
{
public:
    u_POST(){}
    ~u_POST(){}
    u_POST(UserInfo &a);
    //发布者查看报名信息
    UserInfo *p_userCheckSignup(TaskInfo *a,int num);
    //发布者确认负责人
    bool p_userDecideLeader(TaskInfo *a,UserInfo *leader,int times);
    //发布者查看任务
    bool p_userChecktask(TaskInfo *task);
    //发布者修改任务状态
    void p_userChangeState(TaskInfo *targettask, TASKSTATE newstate);
    //发布者支付酬金
    void p_userPayLeader(TaskInfo *task);
};

/*负责人类*/
class u_LEADER : public UserInfo
{
public:
    u_LEADER(){}
    ~u_LEADER(){}
    u_LEADER(UserInfo &a);

    //负责人确认译者ddl
    bool l_usersetTransignday(TaskInfo *mytask,QDate &day);
    //负责人查看译者报名信息
    UserInfo *l_userCheckSignup(TaskInfo *task, int num);
    //负责人确认译者
    bool l_userDecideTran(TaskInfo *task, UserInfo *tran, int times);
    //负责人分割任务
    void l_userDivideTask(TaskInfo *task, UserInfo *tran, QString &taskcon);
    //负责人修改任务状态
    bool l_userChangeState(TaskInfo *targettask, TASKSTATE newstate);
    //负责人检察任务
    bool l_userChecksubtask(SubTaskInfo *subtask);
    //负责人给予反馈
    void l_userFeedbacktask(SubTaskInfo *subtask, QString &feedback);
    //负责人暂存任务
    void l_userTrantemp(TaskInfo *task, QString &trancon);
    //负责人提交任务
    void l_userSubmittask(TaskInfo *task, QString &finaltran);
    //负责人获得酬金
    virtual bool l_userGetPaid(TaskInfo *a);

};


/*译者类*/
class u_TRAN : public UserInfo
{
public:
    u_TRAN(){}
    ~u_TRAN(){}
    u_TRAN(UserInfo &a);
    //译者暂存任务
    void t_userTrantemp(SubTaskInfo *task, QString &trancon);
    //译者提交任务
    void t_userSubmittaskpart(SubTaskInfo *task, QString &finaltran);
    //译者查看评论并反馈
    bool t_usercheckComment(SubTaskInfo *task);
    //译者查看其他译者任务并评论
    void t_userCommentother(SubTaskInfo *task,QString &comment);
    //译者领取酬金
    virtual bool t_userGetPaid(TaskInfo *task);

};

#endif // USERINFO_H

