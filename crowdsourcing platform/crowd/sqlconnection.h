/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2010-08-03
Description:数据库操作
**************************************************/
#ifndef SQLCONNECTION
#define SQLCONNECTION

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QVector>

#include "userinfo.h"
#include "taskinfo.h"
#include "platform.h"

/*************************************************
Function:       createSQLConnection
Description:    建立与数据库的联系
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据表建立成功，将输出成功信息，否则输出错误信息。
Return:         //
Others:         默认连接本地数据库，并且建立需要的表
*************************************************/
static bool createSQLConnection()
{
    QSqlDatabase dbu = QSqlDatabase::addDatabase("QMYSQL");
    dbu.setHostName("127.0.0.1");
    dbu.setUserName("root");
    dbu.setPassword("1040674388pc");
    dbu.setDatabaseName("crowd");
    if( !dbu.open() )
    {
        qDebug()<<"数据库连接失败！";
        return false;
    }
    else
    {
        qDebug()<<"数据库连接成功！";
        QSqlQuery dbuquery(dbu);
        dbuquery.exec("SET NAMES 'UTF8'");

        bool isquery = dbuquery.exec("create table if not exists t_user(identity varchar(255),"
                                     "account int AUTO_INCREMENT,password varchar(255),"
                                     "sex varchar(20),"
                                     "birthday varchar(255),email varchar(255),pix varchar(255),"
                                     "credit int,money DECIMAL(5,2),qualification varchar(255),"
                                     "leadnum int,trannum int,"
                                     "primary key(identity),key(account));");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_user！";
        }
        else
        {
            qDebug()<<"建立数据表t_user失败！"<<dbuquery.lastError();
        }
/*
        13 float taskPay;                        //任务酬金
        11 QDate taskPostDay;                    //任务发起时间
        12 QDate taskLeadDay;                    //负责人报名截止时间
        10 int taskCircleDay;                    //任务周期
        14 PAYSTATE taskPayState;                //任务支付状态
        15 TASKSTATE taskState;                  //任务状态
        8 UserInfo *taskPoster;                 //任务发布者
        9 UserInfo *taskLeader;                 //任务负责人
        1 QString taskName;                //任务名字，唯一标识
        2345 Book *taskBook;                  //任务书籍
        6 QString tasktargetLan;           //任务目标语言
        7 QString taskIntro;               //任务描述
*/
        isquery = dbuquery.exec("create table if not exists t_task(taskname varchar(255),"
                                "bookname varchar(255),bookcontent TEXT,"
                                "bookori varchar(20),bookpre TEXT,"
                                "tasktargetlan varchar(20),taskintro varchar(255),"
                                "taskposter varchar(255),taskleader varchar(255),"
                                "taskcircle int,taskpostday varchar(255),taskleadday varchar(255),"
                                "taskpay DECIMAL(5,2),taskpaystate int,taskstate int,subtasknum int,"
                                "taskcover varchar(255),trancontent TEXT,trancontenttemp TEXT,"
                                "tasktranday varchar(255),isdivide int,tranddl int,"
                                "primary key(taskname));");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_task！";
        }
        else
        {
            qDebug()<<"建立数据表t_task失败！"<<dbuquery.lastError();
        }

        //译者与任务的中间表
        isquery = dbuquery.exec("create table if not exists t_taskandtran(taskname varchar(255),"
                                "translater varchar(255),"
                                "primary key(taskname,translater),"
                                "foreign key(taskname) references t_task(taskname),"
                                "foreign key(translater) references t_user(identity));");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_taskandtran！";
        }
        else
        {
            qDebug()<<"建立数据表t_taskandtran失败！"<<dbuquery.lastError();
        }

        //译者报名人与任务的中间表
        isquery = dbuquery.exec("create table if not exists t_taskandtransign"
                                "(taskname varchar(255),"
                                "translater varchar(255),"
                                "primary key(taskname,translater),"
                                "foreign key(taskname) references t_task(taskname),"
                                "foreign key(translater) references t_user(identity));");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_taskandtransign！";
        }
        else
        {
            qDebug()<<"建立数据表t_taskandtransign失败！"<<dbuquery.lastError();
        }


        //负责人报名人与任务的中间表
        isquery = dbuquery.exec("create table if not exists t_taskandleadsign"
                                "(taskname varchar(255),"
                                "leader varchar(255),"
                                "primary key(taskname,leader),"
                                "foreign key(taskname) references t_task(taskname),"
                                "foreign key(leader) references t_user(identity));");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_taskandleadsign！";
        }
        else
        {
            qDebug()<<"建立数据表t_taskandleadsign失败！"<<dbuquery.lastError();
        }
/*
        QString mesContent;
        UserInfo *mesSender;
        UserInfo *mesReceiver;
        QDateTime messendtime;
        bool mesisRead;
*/
        //消息表
        isquery = dbuquery.exec("create table if not exists t_message("
                                "mesid varchar(255),"
                                "mescontent varchar(255),"
                                "messender varchar(255),"
                                "mesreceiver varchar(255),"
                                "messendtime varchar(255),"
                                "mesisread int,primary key(mesid),"
                                "foreign key(messender) references t_user(identity),"
                                "foreign key(mesreceiver) references t_user(identity));");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_message！";
        }
        else
        {
            qDebug()<<"建立数据表t_message失败！"<<dbuquery.lastError();
        }

//QString subtaskName;                //子任务唯一标识
//UserInfo *subTranslater;            //子任务译者
//QString subtaskContent;             //子任务内容
//QString subTrantemp;                //子任务暂存
//QString subTran;                    //子任务提交
//QVector<QString *> taskFeedback;      //任务反馈
        //子任务补充表
        isquery = dbuquery.exec("create table if not exists t_subtask"
                                "("
                                "subtaskname varchar(255),"
                                "subtranslater varchar(255),"
                                "subcontent TEXT,"
                                "subtemp TEXT,"
                                "subsubmit TEXT,"
                                "taskname varchar(255),"
                                "primary key(subtaskname)"
                                ");");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_subtask！";
        }
        else
        {
            qDebug()<<"建立数据表t_subtask失败！"<<dbuquery.lastError();
        }

        //负责人反馈信息表
//UserInfo *commentPerson;
//QString commentContent;
//QDateTime commentTime;
//QString commentTask;
        isquery = dbuquery.exec("create table if not exists t_comment"
                                "("
                                "count int AUTO_INCREMENT,"
                                "subtaskname varchar(255),"
                                "person varchar(255),"
                                "content varchar(255),"
                                "time varchar(255),"
                                "taskcontent TEXT,"
                                "key(count),"
                                "primary key(subtaskname,count),"
                                "foreign key(subtaskname) references t_subtask(subtaskname)"
                                ");");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_comment！";
        }
        else
        {
            qDebug()<<"建立数据表t_comment失败！"<<dbuquery.lastError();
        }

//用户评价表
//static int count;
//UserInfo *targetUser;       //被评论的用户
//UserInfo *postUser;         //发表评价的用户
//QString commentContent;     //评论内容
        isquery = dbuquery.exec("create table if not exists t_usercomment"
                                "("
                                "count int AUTO_INCREMENT,"
                                "targetuser varchar(255),"
                                "postuser varchar(255),"
                                "content varchar(255),"
                                "primary key(count),"
                                "foreign key(targetuser) references t_user(identity),"
                                "foreign key(postuser) references t_user(identity)"
                                ");");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_comment！";
        }
        else
        {
            qDebug()<<"建立数据表t_comment失败！"<<dbuquery.lastError();
        }

        //朋友中间表（已经成为朋友）
        isquery = dbuquery.exec("create table if not exists t_friend"
                                "("
                                "user1 varchar(255),"
                                "user2 varchar(255),"
                                "primary key(user1,user2),"
                                "foreign key(user1) references t_user(identity),"
                                "foreign key(user2) references t_user(identity));");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_friend！";
        }
        else
        {
            qDebug()<<"建立数据表t_friend失败！"<<dbuquery.lastError();
        }

        //朋友中间表（user1发出好友申请）
        isquery = dbuquery.exec("create table if not exists t_addfriend"
                                "("
                                "user1add varchar(255),"
                                "user2 varchar(255),"
                                "primary key(user1add,user2),"
                                "foreign key(user1add) references t_user(identity),"
                                "foreign key(user2) references t_user(identity));");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_addfriend！";
        }
        else
        {
            qDebug()<<"建立数据表t_addfriend失败！"<<dbuquery.lastError();
        }

        //server表
        isquery = dbuquery.exec("create table if not exists t_server"
                                "("
                                "account varchar(255),"
                                "password varchar(255),"
                                "primary key(account)"
                                ");");
        if(isquery)
        {
            qDebug()<<"成功建立数据表t_server！";
        }
        else
        {
            qDebug()<<"建立数据表t_server失败！"<<dbuquery.lastError();
        }
        return true;
    }

}



/*************************************************
Function:       putInsql_User
Description:    用户数据存入数据库
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存储成功，输出成功信息，否则输出错误信息
Return:         //
Others:         //
*************************************************/
static void putInsql_User()
{
    //QVector<UserInfo *>::Iterator it;
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");

    for(int i=0;i<CHANGE_USERS.size();i++)
    {
        //转化为char*
        UserInfo *nowptr = CHANGE_USERS[i];

        if(nowptr)
        {
                        std::string identity = nowptr->userIdentity.toStdString();
                        const char *cidentity = identity.c_str();
                        std::string password = nowptr->getuserPsd().toStdString();
                        const char *cpassword = password.c_str();
                        std::string sex = nowptr->userSex.toStdString();
                        const char *csex = sex.c_str();
                        std::string birthday = nowptr->userBirthday.toString("yyyy-MM-dd").toStdString();
                        const char *cbirthday = birthday.c_str();
                        std::string email = nowptr->userEmail.toStdString();
                        const char *cemail = email.c_str();
                        std::string pix = nowptr->userpixadd.toStdString();
                        const char *cpix = pix.c_str();
                        int credit = nowptr->userCredit;
                        float money = nowptr->getuserMoney();
                        std::string qualification = nowptr->userQualification.toStdString();
                        const char *cqualification = qualification.c_str();
                        int leadnum = nowptr->userleadnum;
                        int trannum = nowptr->usertrannum;
                        dbuquery.exec("select * from t_user");
                        bool isuser = false;

                        while(dbuquery.next())
                        {
                            if(dbuquery.value(0).toString() == nowptr->userIdentity)
                            {
                                isuser = true;
                            }
                        }

                        if(isuser)
                            //该用户已经存在，更改数据；
                        {
                            //qDebug()<<usercount;
                            dbuquery.prepare("update t_user set password = ? where identity = ?");
                            dbuquery.addBindValue(cpassword);
                            dbuquery.addBindValue(cidentity);
                            dbuquery.exec();
                            dbuquery.prepare("update t_user set sex = ? where identity = ?");
                            dbuquery.addBindValue(csex);
                            dbuquery.addBindValue(cidentity);
                            dbuquery.exec();
                            dbuquery.prepare("update t_user set birthday = ? where identity = ?");
                            dbuquery.addBindValue(cbirthday);
                            dbuquery.addBindValue(cidentity);
                            dbuquery.exec();
                            dbuquery.prepare("update t_user set email = ? where identity = ?");
                            dbuquery.addBindValue(cemail);
                            dbuquery.addBindValue(cidentity);
                            dbuquery.exec();
                            dbuquery.prepare("update t_user set pix = ? where identity = ?");
                            dbuquery.addBindValue(cpix);
                            dbuquery.addBindValue(cidentity);
                            dbuquery.exec();
                            dbuquery.prepare("update t_user set credit = ? where identity = ?");
                            dbuquery.addBindValue(credit);
                            dbuquery.addBindValue(cidentity);
                            dbuquery.exec();
                            dbuquery.prepare("update t_user set money = ? where identity = ?");
                            dbuquery.addBindValue(money);
                            dbuquery.addBindValue(cidentity);
                            dbuquery.exec();
                            dbuquery.prepare("update t_user set leadnum = ? where identity = ?");
                            dbuquery.addBindValue(leadnum);
                            dbuquery.addBindValue(cidentity);
                            dbuquery.exec();
                            dbuquery.prepare("update t_user set trannum = ? where identity = ?");
                            dbuquery.addBindValue(trannum);
                            dbuquery.addBindValue(cidentity);
                            dbuquery.exec();
                        }
                        else
                            //该用户不存在，新建用户
                        {
                            dbuquery.prepare("INSERT INTO t_user(identity,password,sex,"
                                             "birthday,email,pix,credit,money,"
                                             "qualification,leadnum,trannum)"
                                             "VALUES(?,?,?,?,?,?,?,?,?,?,?)");
                            dbuquery.addBindValue(cidentity);
                            //dbuquery.addBindValue(account);
                            dbuquery.addBindValue(cpassword);
                            dbuquery.addBindValue(csex);
                            dbuquery.addBindValue(cbirthday);
                            dbuquery.addBindValue(cemail);
                            dbuquery.addBindValue(cpix);
                            dbuquery.addBindValue(credit);
                            dbuquery.addBindValue(money);
                            dbuquery.addBindValue(cqualification);
                            dbuquery.addBindValue(leadnum);
                            dbuquery.addBindValue(trannum);
                            bool isquery = dbuquery.exec();
                            if(isquery)
                            {
                                qDebug()<<"user数据存入数据库！";
                            }
                            else
                            {
                               qDebug()<<"user数据储存失败！"<<dbuquery.lastError();
                            }
                        }
        }
    }

}


/*************************************************
Function:       putInQt_User();
Description:    用户数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_User()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("select * from t_user");
    while(dbuquery.next())
    {
//        qDebug()<<dbuquery.value(0).toString()<<dbuquery.value(1).toString()
//               <<dbuquery.value(2).toString()<<dbuquery.value(3).toString()
//              <<dbuquery.value(4).toString()<<dbuquery.value(5).toString()
//             <<dbuquery.value(6).toString()<<dbuquery.value(7).toString()
//            <<dbuquery.value(8).toString()<<dbuquery.value(9).toString()
//                 <<dbuquery.value(10).toString()<<dbuquery.value(11).toString();
        UserInfo *newuser = new UserInfo;
        newuser->userIdentity = dbuquery.value(0).toString();
        newuser->userAccount = dbuquery.value(1).toInt();
        newuser->setuserPassword(dbuquery.value(2).toString());
        newuser->userSex = dbuquery.value(3).toString();
        newuser->userBirthday = dbuquery.value(4).toDate();
        newuser->userEmail = dbuquery.value(5).toString();
        newuser->userpixadd = dbuquery.value(6).toString();
        newuser->userCredit = dbuquery.value(7).toInt();
        newuser->setuserMoney(dbuquery.value(8).toFloat());
        newuser->userQualification = dbuquery.value(9).toString();
        newuser->userleadnum = dbuquery.value(10).toInt();
        newuser->usertrannum = dbuquery.value(11).toInt();
        //ALL_USERS.push_back(newuser);
        newuser->userUPDATE(&(ALL_USERS));
    }
}


/*************************************************
Function:       putInsql_Task
Description:    任务数据存入数据库
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_Task()
{
    //QVector<UserInfo *>::Iterator it;
    QSqlDatabase dbt = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbt);
    dbuquery.exec("SET NAMES 'UTF8'");
    for(int i=0;i<CHANGE_TASKS.size();i++)
    {
        //转化为char*
        /*
         *  QVector<UserInfo *> leadSignup;       //负责人报名人
            QVector<UserInfo *> tranSignup;       //译者报名人
            QVector<UserInfo *> taskTranslater;   //任务译者
            QVector<QString *> taskFeedback;      //任务反馈

               13 float taskPay;                        //任务酬金
               11 QDate taskPostDay;                    //任务发起时间
               12 QDate taskLeadDay;                    //负责人报名截止时间
               10 int taskCircleDay;                    //任务周期
               14 PAYSTATE taskPayState;                //任务支付状态
               15 TASKSTATE taskState;                  //任务状态
               8 UserInfo *taskPoster;                 //任务发布者
               9 UserInfo *taskLeader;                 //任务负责人
               1 QString taskName;                //任务名字，唯一标识
               2345 Book *taskBook;                  //任务书籍
               6 QString tasktargetLan;           //任务目标语言
               7 QString taskIntro;               //任务描述
        */
        TaskInfo *nowptr = CHANGE_TASKS[i];
        std::string tname = nowptr->taskName.toStdString();
        const char *ctname = tname.c_str();
        //关于Book***************************************************************************

        std::string bookname = nowptr->taskBook->bookName.toStdString();
        const char *cbookname = bookname.c_str();

        std::string bookcontent = nowptr->taskBook->bookContent.toStdString();
        const char *cbookcontent = bookcontent.c_str();
        std::string bookori = nowptr->taskBook->bookoriLan.toStdString();
        const char *cbookori = bookori.c_str();
        std::string bookpre = nowptr->taskBook->bookPreread.toStdString();
        const char *cbookpre = bookpre.c_str();
        //Book*******************************************************************************
        std::string targetlan = nowptr->tasktargetLan.toStdString();
        const char *ctargetlan = targetlan.c_str();
        std::string tintro = nowptr->taskIntro.toStdString();
        const char *ctintro = tintro.c_str();

        //发布者、负责人,译者组另外储存**********************************************************
        //QString temp = nowptr->taskPoster->userIdentity;
        std::string tposter = nowptr->taskPoster->userIdentity.toStdString();
        const char *ctposter = tposter.c_str();
        char *ctleader = 0x0;
        QByteArray tleader;
        if(nowptr->taskLeader)
        {
            tleader = nowptr->taskLeader->userIdentity.toUtf8();
            ctleader = tleader.data();
        }

        //发布者、负责人*************************************************************************
        //发布时间、截止时间、任务周期*************************************************************
        int tcircleday = nowptr->getTaskcircleday();
        std::string tpostday = nowptr->getTaskpostday().toString("yyyy-MM-dd").toStdString();
        const char *ctpostday = tpostday.c_str();
        std::string tleadday = nowptr->getTaskleadday().toString("yyyy-MM-dd").toStdString();
        const char *ctleadday = tleadday.c_str();
        std::string ttranday = nowptr->taskTranDay.toString("yyyy-MM-dd").toStdString();
        const char *cttranday = ttranday.c_str();
        //时间**********************************************************************************
        //任务酬金******************************************************************************
        int tpaystate = nowptr->getTaskpaystate();
        float tpay = nowptr->getTaskpay();
        //*************************************************************************************
        int tstate = nowptr->getTaskstate();
        int tsubtask = nowptr->getsubTasknum();
        std::string tcover = nowptr->taskCover.toStdString();
        const char *ctcover = tcover.c_str();
        std::string ttrancon = nowptr->taskTran.toStdString();
        const char *ctrancon = ttrancon.c_str();
        std::string ttrancontemp = nowptr->taskTrantemp.toStdString();
        const char *ctrantemp = ttrancontemp.c_str();
        //是否设定译者报名截止时间、分发子任务
        int tdivide = nowptr->isDivide;
        int ttranddl = nowptr->isTranSignday;
        dbuquery.exec("select * from t_task;");
        bool istask = false;
        while(dbuquery.next())
        {
            if(dbuquery.value(0).toString() == nowptr->taskName)
            {
                istask = true;
            }
        }

        if(istask)
        //该任务已经存在，更改数据；
        {

            dbuquery.prepare("update t_task set bookname = ? where taskname = ?");
            dbuquery.addBindValue(cbookname);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set bookcontent = ? where taskname = ?");
            dbuquery.addBindValue(cbookcontent);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set bookori = ? where taskname = ?");
            dbuquery.addBindValue(cbookori);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set bookpre = ? where taskname = ?");
            dbuquery.addBindValue(cbookpre);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set tasktargetlan = ? where taskname = ?");
            dbuquery.addBindValue(ctargetlan);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set taskintro = ? where taskname = ?");
            dbuquery.addBindValue(ctintro);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set taskposter = ? where taskname = ?");
            dbuquery.addBindValue(ctposter);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set taskleader = ? where taskname = ?");
            dbuquery.addBindValue(ctleader);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set taskcircle = ? where taskname = ?");
            dbuquery.addBindValue(tcircleday);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set taskpostday = ? where taskname = ?");
            dbuquery.addBindValue(ctpostday);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();

            dbuquery.prepare("update t_task set taskleadday = ? where taskname = ?");
            dbuquery.addBindValue(ctleadday);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set taskpay = ? where taskname = ?");
            dbuquery.addBindValue(tpay);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set taskpaystate = ? where taskname = ?");
            dbuquery.addBindValue(tpaystate);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set taskstate = ? where taskname = ?");
            dbuquery.addBindValue(tstate);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set subtasknum = ? where taskname = ?");
            dbuquery.addBindValue(tsubtask);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set taskcover = ? where taskname = ?");
            dbuquery.addBindValue(ctcover);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set trancontent = ? where taskname = ?");
            dbuquery.addBindValue(ctrancon);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set trancontenttemp = ? where taskname = ?");
            dbuquery.addBindValue(ctrantemp);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();

            dbuquery.prepare("update t_task set tasktranday = ? where taskname = ?");
            dbuquery.addBindValue(cttranday);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set isdivide = ? where taskname = ?");
            dbuquery.addBindValue(tdivide);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
            dbuquery.prepare("update t_task set tranddl = ? where taskname = ?");
            dbuquery.addBindValue(ttranddl);
            dbuquery.addBindValue(ctname);
            dbuquery.exec();
        }
        else
            //该任务不存在，新建任务
        {

            dbuquery.prepare("INSERT INTO t_task(taskname,bookname,bookcontent,"
                             "bookori,bookpre,tasktargetlan,taskintro,"
                             "taskposter,taskleader,taskcircle,taskpostday,"
                             "taskleadday,taskpay,taskpaystate,taskstate,subtasknum,"
                             "taskcover,trancontent,trancontenttemp,tasktranday,"
                             "isdivide,tranddl)"
                             "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
            dbuquery.addBindValue(ctname);
            dbuquery.addBindValue(cbookname);
            dbuquery.addBindValue(cbookcontent);
            dbuquery.addBindValue(cbookori);
            dbuquery.addBindValue(cbookpre);
            dbuquery.addBindValue(ctargetlan);
            dbuquery.addBindValue(ctintro);
            dbuquery.addBindValue(ctposter);
            dbuquery.addBindValue(ctleader);
            dbuquery.addBindValue(tcircleday);
            dbuquery.addBindValue(ctpostday);
            dbuquery.addBindValue(ctleadday);
            dbuquery.addBindValue(tpay);
            dbuquery.addBindValue(tpaystate);
            dbuquery.addBindValue(tstate);
            dbuquery.addBindValue(tsubtask);
            dbuquery.addBindValue(ctcover);
            dbuquery.addBindValue(ctrancon);
            dbuquery.addBindValue(ctrantemp);
            dbuquery.addBindValue(cttranday);
            dbuquery.addBindValue(tdivide);
            dbuquery.addBindValue(ttranddl);
            bool isquery = dbuquery.exec();
            if(isquery)
            {
                qDebug()<<"task数据存入数据库！";
            }
            else
            {
               qDebug()<<"task数据储存失败！"<<dbuquery.lastError();
            }
        }
    }

}


/*************************************************
Function:       putInQt_Task
Description:    任务数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_Task()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    bool isOK = dbuquery.exec("select * from t_task ORDER BY taskpostday;");
    while(dbuquery.next())
    {
        TaskInfo *newtask = new TaskInfo;
        Book *newbook = new Book;
        newtask->taskBook = newbook;
        newtask->taskName = dbuquery.value(0).toString();
//        qDebug()<<dbuquery.value(0).toString()<<dbuquery.value(1).toString()
//               <<dbuquery.value(2).toString()<<dbuquery.value(3).toString()
//              <<dbuquery.value(4).toString()<<dbuquery.value(5).toString()
//             <<dbuquery.value(6).toString()<<dbuquery.value(7).toString()
//            <<dbuquery.value(8).toString()<<dbuquery.value(9).toString()
//                 <<dbuquery.value(10).toString()<<dbuquery.value(11).toString()
//                <<dbuquery.value(12).toString()<<dbuquery.value(13).toString()
//               <<dbuquery.value(14).toString()<<dbuquery.value(15).toString()
//              <<dbuquery.value(16).toString()<<dbuquery.value(17).toString()
//             <<dbuquery.value(18).toString()<<dbuquery.value(19).toString()
//            <<dbuquery.value(20).toString()<<dbuquery.value(21).toString();
        newtask->taskBook->bookName = dbuquery.value(1).toString();
        newtask->taskBook->bookContent = dbuquery.value(2).toString();
        newtask->taskBook->bookoriLan = dbuquery.value(3).toString();
        newtask->taskBook->bookPreread = dbuquery.value(4).toString();
        newtask->tasktargetLan = dbuquery.value(5).toString();
        newtask->taskIntro = dbuquery.value(6).toString();

        QString tempidentity = dbuquery.value(7).toString();
        for(int i=0;i<ALL_USERS.size();i++)
        {
            if(ALL_USERS[i]->userIdentity == tempidentity)
            {
                newtask->taskPoster = ALL_USERS[i];
                break;
            }
        }

        tempidentity = dbuquery.value(8).toString();
        if(tempidentity.isEmpty()==false)
        {
            newtask->taskLeader = UserInfo::userFind(ALL_USERS,tempidentity);
        }
        else
        {
            newtask->taskLeader = 0x0;
        }
        newtask->setTaskcircle(dbuquery.value(9).toInt());
        newtask->taskPostDay = (dbuquery.value(10).toDate());
        newtask->taskLeadDay = (dbuquery.value(11).toDate());

        newtask->setTaskpay(dbuquery.value(12).toFloat());
        newtask->setTaskpaystate(dbuquery.value(13).toInt());
        newtask->setTaskstate(dbuquery.value(14).toInt());
        newtask->setsubTasknum(dbuquery.value(15).toInt());
        newtask->taskCover = dbuquery.value(16).toString();
        newtask->taskTran = dbuquery.value(17).toString();
        newtask->taskTrantemp = dbuquery.value(18).toString();
        newtask->taskTranDay = dbuquery.value(19).toDate();
        newtask->isDivide = dbuquery.value(20).toInt();
        newtask->isTranSignday = dbuquery.value(21).toInt();
        //ALL_TASKS.push_back(newtask);
        newtask->taskUPDATE(&(ALL_TASKS));
    }
    qDebug()<<"put task in QT"<<isOK;
}


/*************************************************
Function:       putPostInVector
Description:    用户发布任务数据存入用户数组
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
static void putPostInVector()
{
    for(int i=0;i<ALL_USERS.size();i++)
    {
        for(int j=0;j<ALL_TASKS.size();j++)
        {
            if(ALL_USERS[i]->userIdentity == ALL_TASKS[j]->taskPoster->userIdentity)
            {
                //ALL_USERS[i]->userPost.push_back(ALL_TASKS[j]);
                ALL_TASKS[j]->taskUPDATE(&(ALL_USERS[i]->userPost));
            }
        }
    }
}


/*************************************************
Function:       putLeadInVector
Description:    用户负责任务存入用户数组
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
static void putLeadInVector()
{
    for(int i=0;i<ALL_USERS.size();i++)
    {
        for(int j=0;j<ALL_TASKS.size();j++)
        {
            if(ALL_TASKS[j]->taskLeader)
            {
                if(ALL_TASKS[j]->taskLeader->userIdentity == ALL_USERS[i]->userIdentity)
                {
                    ALL_TASKS[j]->taskUPDATE(&(ALL_USERS[i]->userLead));
                    //ALL_USERS[i]->userLead.push_back(ALL_TASKS[j]);
                }
            }
        }
    }
}

/*************************************************
Function:       putTaskInVector
Description:    用户任务放入用户数组
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
static void putTaskInVector()
{
    for(int i=0;i<ALL_USERS.size();i++)
    {
        for(int j=0;j<ALL_USERS[i]->userPost.size();j++)
        {
            //ALL_USERS[i]->userTasks.push_back(ALL_USERS[i]->userPost[j]);
            ALL_USERS[i]->userPost[j]->taskUPDATE(&(ALL_USERS[i]->userTasks));
        }
        for(int j=0;j<ALL_USERS[i]->userLead.size();j++)
        {
            //ALL_USERS[i]->userTasks.push_back(ALL_USERS[i]->userLead[j]);
            ALL_USERS[i]->userLead[j]->taskUPDATE(&(ALL_USERS[i]->userTasks));
        }
        for(int j=0;j<ALL_USERS[i]->userTran.size();j++)
        {
            TaskInfo *temp = dynamic_cast<TaskInfo *>(ALL_USERS[i]->userTran[j]);
            //ALL_USERS[i]->userTasks.push_back(temp);
            temp->taskUPDATE(&(ALL_USERS[i]->userTasks));
        }
    }
}



/*************************************************
Function:       putInsql_Mes
Description:    消息数据存入数据库
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_Mes()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    for(int i=0;i<CHANGE_MES.size();i++)
    {
        //转化为char*
        Message *nowptr = CHANGE_MES[i];
        std::string mcontent = nowptr->mesContent.toStdString();
        const char *cmcontent = mcontent.c_str();
        std::string msender;
        const char *cmsender = NULL;
        if(nowptr->mesSender)
        {
            msender = nowptr->mesSender->userIdentity.toStdString();
            cmsender = msender.c_str();
        }
        std::string mreceiver;
        const char *cmreceiver = NULL;
        if(nowptr->mesReceiver)
        {
            mreceiver = nowptr->mesReceiver->userIdentity.toStdString();
            cmreceiver = mreceiver.c_str();
        }
        std::string msendtime = nowptr->messendtime.toString("yyyy-MM-dd hh:mm:ss").toStdString();
        const char *cmsendtime = msendtime.c_str();
        int misread = nowptr->mesisRead;
        //mes唯一标识 sender+sendtime+receiver
        std::string mesidstr = msender+msendtime+mreceiver;
        const char *cmesid = mesidstr.c_str();
        dbuquery.exec("select * from t_message ORDER BY messendtime");
        bool ismes = false;
        while(dbuquery.next())
        {
            //以sender 和 sendtime 作为唯一区分
            if(dbuquery.value(0).toString().toStdString() == mesidstr)
            {
                ismes = true;
            }
        }
        if(ismes)
            //该消息已经存在，更改数据；
        {
            dbuquery.prepare("update t_message set mescontent = ? where mesid = ?");
            dbuquery.addBindValue(cmcontent);
            dbuquery.addBindValue(cmesid);
            dbuquery.exec();
            dbuquery.prepare("update t_message set messender = ? where mesid = ?");
            dbuquery.addBindValue(cmsender);
            dbuquery.addBindValue(cmesid);
            dbuquery.exec();
            dbuquery.prepare("update t_message set mesreceiver = ? where mesid = ?");
            dbuquery.addBindValue(cmreceiver);
            dbuquery.addBindValue(cmesid);
            dbuquery.exec();
            dbuquery.prepare("update t_message set messendtime = ? where mesid = ?");
            dbuquery.addBindValue(cmsendtime);
            dbuquery.addBindValue(cmesid);
            dbuquery.exec();
            dbuquery.prepare("update t_message set mesisread = ? where mesid = ?");
            dbuquery.addBindValue(misread);
            dbuquery.addBindValue(cmesid);
            dbuquery.exec();

        }
        else
            //该信息不存在，新建信息
        {
            dbuquery.prepare("INSERT INTO t_message("
                             "mesid,"
                             "mescontent,"
                             "messender,"
                             "mesreceiver,"
                             "messendtime,"
                             "mesisread)"
                             "VALUES(?,?,?,?,?,?)");
            dbuquery.addBindValue(cmesid);
            dbuquery.addBindValue(cmcontent);
            dbuquery.addBindValue(cmsender);
            dbuquery.addBindValue(cmreceiver);
            dbuquery.addBindValue(cmsendtime);
            dbuquery.addBindValue(misread);
            bool isquery = dbuquery.exec();
            if(isquery)
            {
                qDebug()<<"message数据存入数据库！";
            }
            else
            {
               qDebug()<<"message数据储存失败！"<<dbuquery.lastError();
            }
        }
    }

}


/*************************************************
Function:       putInQt_Mes
Description:    消息数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_Mes()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    bool isOK = dbuquery.exec("select * from t_message ORDER BY messendtime desc;");
    while(dbuquery.next())
    {
        Message *newmes = new Message;
        newmes->mesContent = dbuquery.value(1).toString();
//        qDebug()<<dbuquery.value(0).toString()<<dbuquery.value(1).toString()
//               <<dbuquery.value(2).toString()<<dbuquery.value(3).toString()
//              <<dbuquery.value(4).toString();
        QString tempidentity = dbuquery.value(2).toString();
        if(tempidentity.isEmpty()==false)
        {
            //qDebug()<<newmes->mesContent<<"message from user";
            newmes->mesSender = UserInfo::userFind(ALL_USERS,tempidentity);
        }
        else
        {
            newmes->mesSender = 0x0;
        }
        tempidentity = dbuquery.value(3).toString();
        if(tempidentity.isEmpty()==false)
        {
            newmes->mesReceiver = UserInfo::userFind(ALL_USERS,tempidentity);
        }
        else
        {
            newmes->mesReceiver = 0x0;
        }
        newmes->messendtime = dbuquery.value(4).toDateTime();
        newmes->mesisRead = dbuquery.value(5).toInt();
        ALL_MES.push_back(newmes);
    }
    qDebug()<<"put message in QT"<<isOK;
}



/*************************************************
Function:       putMessageInVector
Description:    用户消息存入用户数组
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
static void putMessageInVector()
{
    for(int i=0;i<ALL_USERS.size();i++)
    {
        for(int j=0;j<ALL_MES.size();j++)
        {
            if((ALL_MES[j]->mesSender)!=0x0)
            {

                if(ALL_USERS[i]->userIdentity == ALL_MES[j]->mesSender->userIdentity)
                {
                    //qDebug()<<"mess"<<ALL_MES[j]->mesSender->userIdentity;
                    ALL_MES[j]->mesUPDATE(&(ALL_USERS[i]->userSendmes));
                    //ALL_USERS[i]->userSendmes.push_back(ALL_MES[j]);
                }
            }
            if((ALL_MES[j]->mesReceiver)!=0x0)
            {
                if(ALL_USERS[i]->userIdentity == ALL_MES[j]->mesReceiver->userIdentity)
                {
                    ALL_MES[j]->mesUPDATE(&(ALL_USERS[i]->userRecmes));
                    //ALL_USERS[i]->userRecmes.push_back(ALL_MES[j]);
                }
            }
        }

    }
}

/*************************************************
Function:       putInsql_TaskandTran
Description:    译者与任务的中间关联表
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_TaskandTran()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        for(int j=0;j<ALL_TASKS[i]->taskTranslater.size();j++)
        {
            //转化为char*
            TaskInfo *nowptr = ALL_TASKS[i];
            QString taskname = nowptr->taskName;
            std::string tname = nowptr->taskName.toStdString();
            const char *ctname = tname.c_str();
            QString translater = nowptr->taskTranslater[j]->userIdentity;
            std::string tran = nowptr->taskTranslater[j]->userIdentity.toStdString();
            const char *ctran = tran.c_str();

            dbuquery.exec("select * from t_taskandtran");
            bool isexist = false;
            while(dbuquery.next())
            {
                //联合主键
                if((dbuquery.value(0).toString() == taskname)&&
                        (dbuquery.value(1).toString() == translater))
                {
                    isexist = true;
                }

            }
            if(!isexist)
            {
                dbuquery.prepare("INSERT IGNORE INTO t_taskandtran("
                                     "taskname,"
                                     "translater)"
                                     "VALUES(?,?)");
                dbuquery.addBindValue(ctname);
                dbuquery.addBindValue(ctran);
                bool isquery = dbuquery.exec();
                if(isquery)
                {
                    qDebug()<<"taskandtran数据存入数据库！";
                }
                else
                {
                   qDebug()<<"taskandtran数据储存失败！"<<dbuquery.lastError();
                }
            }
        }
    }
}

/*************************************************
Function:       putInQt_Tran
Description:    译者数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_Tran()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    QString taskname ;
    bool isOK = false;
    std::string tasknamestr;
    const char *ctaskname = "null";
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        taskname = ALL_TASKS[i]->taskName;
        tasknamestr = taskname.toStdString();
        ctaskname = tasknamestr.c_str();
        dbuquery.prepare("select translater from t_taskandtran where taskname = ?");
        dbuquery.addBindValue(ctaskname);
        isOK = dbuquery.exec();
        while(dbuquery.next())
        {
            //找到译者并插入任务译者队列
            QString tranfindidentity = dbuquery.value(0).toString();
            UserInfo *tranfind = UserInfo::userFind(ALL_USERS,tranfindidentity);
            ALL_TASKS[i]->taskTranslater.push_back(tranfind);
        }
    }
    qDebug()<<"put translaters in qt"<<isOK;
}


/*************************************************
Function:       putIsql_TaskandTransign
Description:    任务与译者报名人的中间关联表
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_TaskandTransign()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        for(int j=0;j<ALL_TASKS[i]->tranSignup.size();j++)
        {
            //转化为char*
            TaskInfo *nowptr = ALL_TASKS[i];
            QString taskname = nowptr->taskName;
            std::string tname = nowptr->taskName.toStdString();
            const char *ctname = tname.c_str();
            QString signidentity = nowptr->tranSignup[j]->userIdentity;
            std::string tran = nowptr->tranSignup[j]->userIdentity.toStdString();
            const char *ctran = tran.c_str();

            dbuquery.exec("select * from t_taskandtransign");
            bool isexist = false;
            while(dbuquery.next())
            {
                //联合主键
                if((dbuquery.value(0).toString() == taskname)&&
                        (dbuquery.value(1).toString() == signidentity))
                {
                    isexist = true;
                }
            }
            if(!isexist)
            {
                dbuquery.prepare("INSERT IGNORE INTO t_taskandtransign("
                                 "taskname,"
                                 "translater)"
                                 "VALUES(?,?)");
                dbuquery.addBindValue(ctname);
                dbuquery.addBindValue(ctran);
                bool isquery = dbuquery.exec();
                if(isquery)
                {
                    qDebug()<<"taskandtransign数据存入数据库！";
                }
                else
                {
                   qDebug()<<"taskandtransign数据储存失败！"<<dbuquery.lastError();
                }
            }
        }
    }
}


/*************************************************
Function:       putInQt_Transign
Description:    译者报名数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_Transign()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    QString taskname,identity;
    bool isOK = false;
    std::string tasknamestr,identitystr;
    const char *ctaskname = "null";
    const char *cidentity = "null";
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        taskname = ALL_TASKS[i]->taskName;
        tasknamestr = taskname.toStdString();
        ctaskname = tasknamestr.c_str();
        dbuquery.prepare("select translater from t_taskandtransign where taskname = ?");
        dbuquery.addBindValue(ctaskname);
        isOK = dbuquery.exec();

        while(dbuquery.next())
        {
            //找到报名译者并插入报名译者队列
            QString tranfindidentity = dbuquery.value(0).toString();
            UserInfo *tranfind = UserInfo::userFind(ALL_USERS,tranfindidentity);
            ALL_TASKS[i]->tranSignup.push_back(tranfind);
        }
    }
    for(int i=0;i<ALL_USERS.size();i++)
    {
        identity = ALL_USERS[i]->userIdentity;
        identitystr = identity.toStdString();
        cidentity = identitystr.c_str();
        dbuquery.prepare("select taskname from t_taskandtransign where translater = ?");
        dbuquery.addBindValue(cidentity);
        isOK = dbuquery.exec();
//        qDebug()<<"Now tranign Into QT"<<isOK
//               <<"char"<<cidentity;
        while(dbuquery.next())
        {
            QString taskfindname = dbuquery.value(0).toString();
            TaskInfo *taskfind = TaskInfo::getnowTask(ALL_TASKS,taskfindname);
            ALL_USERS[i]->userSignTran.push_back(taskfind);
        }
    }
    qDebug()<<"put transignup in QT"<<isOK;
}

/*************************************************
Function:       putInsql_TaskandLeadsign
Description:    任务与负责人报名者的中间关联表
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_TaskandLeadsign()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        for(int j=0;j<ALL_TASKS[i]->leadSignup.size();j++)
        {
            //转化为char*
            TaskInfo *nowptr = ALL_TASKS[i];
            std::string tname = nowptr->taskName.toStdString();
            QString taskname = nowptr->taskName;
            const char *ctname = tname.c_str();
            std::string leader = nowptr->leadSignup[j]->userIdentity.toStdString();
            QString signidentity = nowptr->leadSignup[j]->userIdentity;
            const char *cleader = leader.c_str();
            dbuquery.exec("select * from t_taskandtransign");
            bool isexist = false;
            while(dbuquery.next())
            {
                //联合主键
//                qDebug()<<dbuquery.value(0).toString()<<"and"<<taskname
//                       <<dbuquery.value(1).toString()<<"and"<<signidentity;
                if((dbuquery.value(0).toString() == taskname)&&
                        (dbuquery.value(1).toString() == signidentity))
                {
                    isexist = true;
                    break;
                }
//                qDebug()<<signidentity<<"this signleader exist"<<isexist;
            }
            if(!isexist)
            {
                dbuquery.prepare("INSERT IGNORE INTO t_taskandleadsign("
                                 "taskname,"
                                 "leader)"
                                 "VALUES(?,?)");
                dbuquery.addBindValue(ctname);
                dbuquery.addBindValue(cleader);
                bool isquery = dbuquery.exec();
                if(isquery)
                {
                    //qDebug()<<"taskandleadsign数据存入数据库！";
                }
                else
                {
                   //qDebug()<<"taskandleadsign数据储存失败！"<<dbuquery.lastError();
                }
            }
        }
    }
}


/*************************************************
Function:       putInQt_Leadsign
Description:    用户报名负责人数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_Leadsign()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    QString taskname,identity;
    bool isOK = false;
    std::string tasknamestr,identitystr;
    const char *ctaskname = "null";
    const char *cidentity = "null";
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        taskname = ALL_TASKS[i]->taskName;
        tasknamestr = taskname.toStdString();
        ctaskname = tasknamestr.c_str();
        dbuquery.prepare("select leader from t_taskandleadsign where taskname = ?");
        dbuquery.addBindValue(ctaskname);
        isOK = dbuquery.exec();
//        qDebug()<<"Now leadsign Into QT"<<isOK
//               <<"char"<<ctaskname;
        while(dbuquery.next())
        {
            //找到报名译者并插入报名译者队列
            QString leadfindidentity = dbuquery.value(0).toString();
            UserInfo *leadfind = UserInfo::userFind(ALL_USERS,leadfindidentity);
            ALL_TASKS[i]->leadSignup.push_back(leadfind);
        }
    }
    for(int i=0;i<ALL_USERS.size();i++)
    {
        identity = ALL_USERS[i]->userIdentity;
        identitystr = identity.toStdString();
        cidentity = identitystr.c_str();
        dbuquery.prepare("select taskname from t_taskandleadsign where leader = ?");
        dbuquery.addBindValue(cidentity);
        isOK = dbuquery.exec();
//        qDebug()<<"Now leadsign Into QT"<<isOK
//               <<"char"<<cidentity;
        while(dbuquery.next())
        {
            QString taskfindname = dbuquery.value(0).toString();
            TaskInfo *taskfind = TaskInfo::getnowTask(ALL_TASKS,taskfindname);
            ALL_USERS[i]->userSignLead.push_back(taskfind);
            //qDebug()<<identity<<"signuplead"<<taskfind->taskName;
        }
    }
    qDebug()<<"put leadsignup in QT"<<isOK;
}


/*************************************************
Function:       putInsql_Subtask
Description:    子任务数据存入数据库
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_SubTask()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    for(int i=0;i<CHANGE_SUB.size();i++)
    {
            SubTaskInfo *nowptr = CHANGE_SUB[i];
            QByteArray sname = nowptr->subtaskName.toUtf8();
            char *csname = sname.data();
            QByteArray stran = nowptr->subTranslater->userIdentity.toUtf8();
            char *cstran = stran.data();
            QByteArray scontent = nowptr->subtaskContent.toUtf8();
            char *cscontent = scontent.data();
            QByteArray stemp = nowptr->subTrantemp.toUtf8();
            char *cstemp = stemp.data();
            QByteArray ssubmit = nowptr->subTran.toUtf8();
            char *cssubmit = ssubmit.data();
            QByteArray staskname = nowptr->taskName.toUtf8();
            char *cstaskname = staskname.data();
            dbuquery.exec("select * from t_subtask");
            bool isexist = false;
            while(dbuquery.next())
            {
                if((dbuquery.value(0).toString() == csname))
                {
                    isexist = true;
                }
            }
            if(!isexist)
            {
                dbuquery.prepare("INSERT IGNORE INTO t_subtask("
                                 "subtaskname,"
                                 "subtranslater,"
                                 "subcontent,"
                                 "subtemp,"
                                 "subsubmit,"
                                 "taskname"
                                 ")"
                                 "VALUES(?,?,?,?,?,?)");
                dbuquery.addBindValue(csname);
                dbuquery.addBindValue(cstran);
                dbuquery.addBindValue(cscontent);
                dbuquery.addBindValue(cstemp);
                dbuquery.addBindValue(cssubmit);
                dbuquery.addBindValue(cstaskname);
                bool isquery = dbuquery.exec();
                if(isquery)
                {
                    qDebug()<<"subtask数据存入数据库！";
                }
                else
                {
                   qDebug()<<"subtask数据储存失败！"<<dbuquery.lastError();
                }
            }
            else
            {
                dbuquery.prepare("update t_subtask set subtranslater = ? where subtaskname = ?");
                dbuquery.addBindValue(cstran);
                dbuquery.addBindValue(csname);
                dbuquery.exec();
                dbuquery.prepare("update t_subtask set subcontent = ? where subtaskname = ?");
                dbuquery.addBindValue(cscontent);
                dbuquery.addBindValue(csname);
                dbuquery.exec();
                dbuquery.prepare("update t_subtask set subtemp = ? where subtaskname = ?");
                dbuquery.addBindValue(cstemp);
                dbuquery.addBindValue(csname);
                dbuquery.exec();
                dbuquery.prepare("update t_subtask set subsubmit = ? where subtaskname = ?");
                dbuquery.addBindValue(cssubmit);
                dbuquery.addBindValue(csname);
                dbuquery.exec();
                dbuquery.prepare("update t_subtask set taskname = ? where subtaskname = ?");
                dbuquery.addBindValue(cstaskname);
                dbuquery.addBindValue(csname);
                dbuquery.exec();

            }

    }
}


/*************************************************
Function:       putInQt_Subtask
Description:    子任务数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_SubTask()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    bool isOK = dbuquery.exec("select * from t_subtask");
    while(dbuquery.next())
    {
        QString taskname = dbuquery.value(5).toString();
        TaskInfo *maintask = TaskInfo::getnowTask(ALL_TASKS,taskname);
        SubTaskInfo *newsub = new SubTaskInfo(*maintask);
//        qDebug()<<dbuquery.value(0).toString()<<dbuquery.value(1).toString()
//               <<dbuquery.value(2).toString()<<dbuquery.value(3).toString()
//              <<dbuquery.value(4).toString()<<dbuquery.value(5).toString();

        newsub->subtaskName = dbuquery.value(0).toString();
        QString tempidentity = dbuquery.value(1).toString();
        if(tempidentity.isEmpty()==false)
        {
            newsub->subTranslater = UserInfo::userFind(ALL_USERS,tempidentity);
        }
        else
        {
            newsub->subTranslater = 0x0;
        }
        newsub->subtaskContent = dbuquery.value(2).toString();
        newsub->subTrantemp = dbuquery.value(3).toString();
        newsub->subTran = dbuquery.value(4).toString();

        ALL_SUBTASK.push_back(newsub);
    }
    qDebug()<<"put subtask in QT"<<isOK;
}


/*************************************************
Function:       putSubtaskInVector
Description:    子任务存入用户与任务数组
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putSubtaskInVector()
{
    //将subtask相应放入用户和任务的subtask之中
    //用户
    for(int i=0;i<ALL_USERS.size();i++)
    {
        for(int j=0;j<ALL_SUBTASK.size();j++)
        {
            if((ALL_USERS[i]->userIdentity) == ALL_SUBTASK[j]->subTranslater->userIdentity)
            {
                //ALL_USERS[i]->userTran.push_back(ALL_SUBTASK[j]);
                ALL_SUBTASK[j]->taskUPDATE(&(ALL_USERS[i]->userTran));
            }
        }
    }
    //任务
    for(int i=0;i<ALL_TASKS.size();i++)
    {
        for(int j=0;j<ALL_SUBTASK.size();j++)
        {
            if(ALL_TASKS[i]->taskName == ALL_SUBTASK[j]->taskName)
            {
                ALL_TASKS[i]->taskSub.push_back(ALL_SUBTASK[j]);
            }
        }
    }
}



/*************************************************
Function:       putInsql_Comment
Description:    反馈数据存入数据库
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_Comment()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    for(int i=0;i<CHANGE_COMMENT.size();i++)
    {
        Comment *nowptr = CHANGE_COMMENT[i];
        int count = nowptr->count;
        QByteArray sname = nowptr->commentTaskname.toUtf8();
        char *csname = sname.data();
        QByteArray per = nowptr->commentPerson->userIdentity.toUtf8();
        char *cper = per.data();
        QByteArray content = nowptr->commentContent.toUtf8();
        char *ccontent = content.data();
        QByteArray time = nowptr->commentTime.toString("yyyy-MM-dd").toUtf8();
        char *ctime = time.data();
        QByteArray scontent = nowptr->commentTask.toUtf8();
        char *cscontent = scontent.data();
        dbuquery.exec("select * from t_comment;");
        bool isexist = false;
        while(dbuquery.next())
        {
            if((dbuquery.value(0).toInt() == count))
            {
                isexist = true;
            }
        }
        if(!isexist)
        {
            dbuquery.prepare("INSERT INTO t_comment("
                             "count,"
                             "subtaskname,"
                             "person,"
                             "content,"
                             "time,"
                             "taskcontent"
                             ")"
                             "VALUES(?,?,?,?,?,?)");
            dbuquery.addBindValue(count);
            dbuquery.addBindValue(csname);
            dbuquery.addBindValue(cper);
            dbuquery.addBindValue(ccontent);
            dbuquery.addBindValue(ctime);
            dbuquery.addBindValue(cscontent);
            bool isquery = dbuquery.exec();
            if(isquery)
            {
                //qDebug()<<count<<csname<<cper<<ccontent<<ctime<<cscontent;
                qDebug()<<"comment数据存入数据库！";
            }
            else
            {
               qDebug()<<"comment数据储存失败！"<<dbuquery.lastError();
            }
        }
        else
        {
            dbuquery.prepare("update t_comment set subtaskname = ? where count = ?");
            dbuquery.addBindValue(csname);
            dbuquery.addBindValue(count);
            dbuquery.exec();
            dbuquery.prepare("update t_comment set person = ? where count = ?");
            dbuquery.addBindValue(cper);
            dbuquery.addBindValue(count);
            dbuquery.exec();
            dbuquery.prepare("update t_comment set content = ? where count = ?");
            dbuquery.addBindValue(ccontent);
            dbuquery.addBindValue(count);
            dbuquery.exec();
            dbuquery.prepare("update t_comment set time = ? where count = ?");
            dbuquery.addBindValue(ctime);
            dbuquery.addBindValue(count);
            dbuquery.exec();
            dbuquery.prepare("update t_comment set taskcontent = ? where count = ?");
            dbuquery.addBindValue(cscontent);
            dbuquery.addBindValue(count);
            dbuquery.exec();

        }
    }
}


/*************************************************
Function:       putInQt_Comment
Description:    反馈数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_Comment()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    bool isOK = dbuquery.exec("select * from t_comment");
    while(dbuquery.next())
    {
//        qDebug()<<dbuquery.value(0).toString()<<dbuquery.value(1).toString()
//               <<dbuquery.value(2).toString()<<dbuquery.value(3).toString()
//              <<dbuquery.value(4).toString()<<dbuquery.value(5).toString();
        Comment *newcomment = new Comment;
        newcomment->count = dbuquery.value(0).toInt();
        newcomment->commentTaskname = dbuquery.value(1).toString();
        QString personidentity = dbuquery.value(2).toString();
        newcomment->commentPerson = UserInfo::userFind(ALL_USERS,personidentity);
        newcomment->commentContent = dbuquery.value(3).toString();
        newcomment->commentTime = dbuquery.value(4).toDateTime();
        newcomment->commentTask = dbuquery.value(5).toString();
        ALL_COMMENT.push_back(newcomment);
    }
    qDebug()<<"put comment in QT"<<isOK;
}

/*************************************************
Function:       putCommentInSubtaskVector
Description:    用户数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putCommentInSubtaskVector()
{
    for(int i=0;i<ALL_SUBTASK.size();i++)
    {
        for(int j=0;j<ALL_COMMENT.size();j++)
        {
//            qDebug()<<ALL_COMMENT[j]->commentTaskname;
//            qDebug()<<ALL_SUBTASK[i]->subtaskName;
            if(ALL_SUBTASK[i]->subtaskName == ALL_COMMENT[j]->commentTaskname)
            {
                //ALL_SUBTASK[i]->taskFeedback.push_back(ALL_COMMENT[j]);
                ALL_COMMENT[j]->commentUPDATE(&(ALL_SUBTASK[i]->taskFeedback));
            }
        }
    }
}



/*************************************************
Function:       putInsql_userComment
Description:    用户评论数据存入数据库
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_userComment()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    for(int i=0;i<CHANGE_USERCOMMENTS.size();i++)
    {
        userComment *nowptr = CHANGE_USERCOMMENTS[i];
        int count = nowptr->count;
        QByteArray target = nowptr->targetUser->userIdentity.toUtf8();
        char *ctarget = target.data();
        QByteArray post = nowptr->postUser->userIdentity.toUtf8();
        char *cpost = post.data();
        QByteArray content = nowptr->commentContent.toUtf8();
        char *ccontent = content.data();

        dbuquery.exec("select * from t_usercomment;");
        bool isexist = false;
        while(dbuquery.next())
        {
            if((dbuquery.value(0).toInt() == count))
            {
                isexist = true;
            }
        }
        if(!isexist)
        {
            dbuquery.prepare("INSERT INTO t_usercomment("
                             "count,"
                             "targetuser,"
                             "postuser,"
                             "content"
                             ")"
                             "VALUES(?,?,?,?)");
            dbuquery.addBindValue(count);
            dbuquery.addBindValue(ctarget);
            dbuquery.addBindValue(cpost);
            dbuquery.addBindValue(ccontent);
            bool isquery = dbuquery.exec();
            if(isquery)
            {
                //qDebug()<<count<<csname<<cper<<ccontent<<ctime<<cscontent;
                qDebug()<<"usercomment数据存入数据库！";
            }
            else
            {
               qDebug()<<"usercomment数据储存失败！"<<dbuquery.lastError();
            }
        }
        else
        {
            dbuquery.prepare("update t_usercomment set targetuser = ? where count = ?");
            dbuquery.addBindValue(ctarget);
            dbuquery.addBindValue(count);
            dbuquery.exec();
            dbuquery.prepare("update t_usercomment set postuser = ? where count = ?");
            dbuquery.addBindValue(cpost);
            dbuquery.addBindValue(count);
            dbuquery.exec();
            dbuquery.prepare("update t_usercomment set content = ? where count = ?");
            dbuquery.addBindValue(ccontent);
            dbuquery.addBindValue(count);
            dbuquery.exec();
        }
    }
}


/*************************************************
Function:       putInQt_userComment
Description:    用户评论数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_userComment()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    bool isOK = dbuquery.exec("select * from t_usercomment");
    while(dbuquery.next())
    {
//        qDebug()<<dbuquery.value(0).toString()<<dbuquery.value(1).toString()
//               <<dbuquery.value(2).toString()<<dbuquery.value(3).toString()
        userComment *newcomment = new userComment;
        newcomment->count = dbuquery.value(0).toInt();
        QString targetidentity = dbuquery.value(1).toString();
        newcomment->targetUser = UserInfo::userFind(ALL_USERS,targetidentity);
        QString postidentity = dbuquery.value(2).toString();
        newcomment->postUser = UserInfo::userFind(ALL_USERS,postidentity);
        newcomment->commentContent = dbuquery.value(3).toString();
        ALL_USERCOMMENTS.push_back(newcomment);
    }
    qDebug()<<"put usercomment in QT"<<isOK;
}


/*************************************************
Function:       putuserCommentInUserVector
Description:    用户评论数据存入用户数组
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         //
Others:         //
*************************************************/
static void putuserCommentInUserVector()
{
    for(int i=0;i<ALL_USERS.size();i++)
    {
        for(int j=0;j<ALL_USERCOMMENTS.size();j++)
        {
            //qDebug()<<ALL_USERCOMMENTS[j]->targetUser;
            //qDebug()<<ALL_USERS[i]->userIdentity;
            if(ALL_USERS[i] == ALL_USERCOMMENTS[j]->targetUser)
            {
                //ALL_USERS[i]->userComments.push_back(ALL_USERCOMMENTS[j]);
                ALL_USERCOMMENTS[j]->userCommentUPDATE(&(ALL_USERS[i]->userComments));
            }
        }
    }
}


/*************************************************
Function:       putInQt_Server
Description:    Server数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_Server()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    bool isOK = dbuquery.exec("select * from t_server");
    while(dbuquery.next())
    {
//        qDebug()<<dbuquery.value(0).toString()<<dbuquery.value(1).toString()
        QString account = dbuquery.value(0).toString();
        QString psd = dbuquery.value(1).toString();
        Server *newserver = new Server(account, psd);
        ALL_SERVERS.push_back(newserver);
    }
    qDebug()<<"put server in QT"<<isOK;
}


/*************************************************
Function:       putInsql_Friend();
Description:    好友数据存入数据库
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_Friend()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    for(int i=0;i<ALL_USERS.size();i++)
    {
        for(int j=0;j<ALL_USERS[i]->userFriends.size();j++)
        {
            QByteArray user1 = ALL_USERS[i]->userIdentity.toUtf8();
            char *cuser1 = user1.data();
            QByteArray user2 = ALL_USERS[i]->userFriends[j]->userIdentity.toUtf8();
            char *cuser2 = user2.data();
            dbuquery.exec("select * from t_friend;");
            bool isexist = false;
            while(dbuquery.next())
            {
                if((dbuquery.value(0).toString() == cuser1)&&
                        (dbuquery.value(1).toString() == cuser2))
                {
                    isexist = true;
                }
            }
            if(!isexist)
            {
                dbuquery.prepare("INSERT IGNORE INTO t_friend("
                                     "user1,"
                                     "user2)"
                                     "VALUES(?,?)");
                dbuquery.addBindValue(cuser1);
                dbuquery.addBindValue(cuser2);
                bool isquery = dbuquery.exec();
                if(isquery)
                {
                    qDebug()<<"friend数据存入数据库！";
                }
                else
                {
                   qDebug()<<"friend数据储存失败！"<<dbuquery.lastError();
                }
            }
        }
    }
}


/*************************************************
Function:       putInQt_Friend
Description:    好友数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_Friend()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    QString userid;
    bool isOK = false;
    for(int i=0;i<ALL_USERS.size();i++)
    {
        userid = ALL_USERS[i]->userIdentity;
        char *cuserid = userid.toUtf8().data();
        dbuquery.prepare("select user2 from t_friend where user1 = ?");
        dbuquery.addBindValue(cuserid);
        isOK = dbuquery.exec();
        while(dbuquery.next())
        {
            QString targetuserid = dbuquery.value(0).toString();
            UserInfo *friendfind = UserInfo::userFind(ALL_USERS,targetuserid);
            ALL_USERS[i]->userFriends.push_back(friendfind);
        }
    }
    qDebug()<<"put friend in QT"<<isOK;
}

/*************************************************
Function:       putInsql_AddFriend
Description:    好友申请数据存入数据库
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInsql_AddFriend()
{

    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    dbuquery.exec("SET NAMES 'UTF8'");
    dbuquery.exec("TRUNCATE TABLE t_addfriend;");
    for(int i=0;i<ALL_USERS.size();i++)
    {
        for(int j=0;j<ALL_USERS[i]->userSendfriend.size();j++)
        {
            QByteArray user1 = ALL_USERS[i]->userIdentity.toUtf8();
            char *cuser1 = user1.data();
            QByteArray user2 = ALL_USERS[i]->userSendfriend[j]->userIdentity.toUtf8();
            char *cuser2 = user2.data();
            dbuquery.exec("select * from t_addfriend;");
            bool isexist = false;
            while(dbuquery.next())
            {
                if((dbuquery.value(0).toString() == cuser1)&&
                        (dbuquery.value(1).toString() == cuser2))
                {
                    isexist = true;
                }
            }
            if(!isexist)
            {
                dbuquery.prepare("INSERT IGNORE INTO t_addfriend("
                                     "user1add,"
                                     "user2)"
                                     "VALUES(?,?)");
                dbuquery.addBindValue(cuser1);
                dbuquery.addBindValue(cuser2);
                bool isquery = dbuquery.exec();
                if(isquery)
                {
                    qDebug()<<"addfriend数据存入数据库！";
                }
                else
                {
                   qDebug()<<"addfriend数据储存失败！"<<dbuquery.lastError();
                }
            }
        }
    }
}


/*************************************************
Function:       putInQt_AddFriend
Description:    好友申请数据存入内存
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         若数据存入成功，输出成功信息， 否则输出错误信息。
Return:         //
Others:         //
*************************************************/
static void putInQt_AddFriend()
{
    QSqlDatabase dbu = QSqlDatabase::database("crowd");
    QSqlQuery dbuquery(dbu);
    QString userid;
    bool isOK = false;
    for(int i=0;i<ALL_USERS.size();i++)
    {
        userid = ALL_USERS[i]->userIdentity;
        char *cuserid = userid.toUtf8().data();
        dbuquery.prepare("select user2 from t_addfriend where user1add = ?");
        dbuquery.addBindValue(cuserid);
        isOK = dbuquery.exec();
        while(dbuquery.next())
        {
            QString targetuserid = dbuquery.value(0).toString();
            UserInfo *addfind = UserInfo::userFind(ALL_USERS,targetuserid);
            ALL_USERS[i]->userSendfriend.push_back(addfind);
            addfind->userRecfriend.push_back(ALL_USERS[i]);
        }
    }
    qDebug()<<"put addfriend in QT"<<isOK;
}

#endif // SQLCONNECTION

