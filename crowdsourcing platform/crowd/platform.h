#ifndef PLATFORM
#define PLATFORM
#include "userinfo.h"
#include "taskinfo.h"
#include "server.h"
//#include "message.h"
#include <QMap>
#include <QString>
#include <QVector>

class Message;
class userComment;
class Server;
/*枚举，标记用户在某个任务中的角色*/
enum TASKROLE
{
    VISITOR=0,//用户不参与任务(含报名用户
    POSTER,//发布者
    LEADER,//负责人
    TRANSLATER//译者
};
/*全局变量，存放所有的平台管理员Server*/
extern QVector<Server *> ALL_SERVERS;
/*全局变量，存放当前操作的平台管理员*/
extern Server *NOW_SERVER;
/*全局变量，存放所有已注册的用户*/
extern QVector<UserInfo*> ALL_USERS;
/*全局变量，存放所有的任务*/
extern QVector<TaskInfo*> ALL_TASKS;
/*全局变量，存放所有的子任务*/
extern QVector<SubTaskInfo *>ALL_SUBTASK;
/*全局变量，存放当前操作的用户指针*/
extern UserInfo *NOW_USER;
/*全局变量，存放当前操作的任务指针*/
extern TaskInfo *NOW_TASK;
/*全局变量，存放当前操作的子任务指针*/
extern SubTaskInfo *NOW_SUB;
/*全局变量，存放所有信息发生改变的用户*/
extern QVector<UserInfo *> CHANGE_USERS;
/*全局变量，存放所有信息发生改变的任务*/
extern QVector<TaskInfo *> CHANGE_TASKS;
/*全局变量，存放所有信息发生改变的子任务*/
extern QVector<SubTaskInfo *> CHANGE_SUB;
/*全局变量，存放所有的消息*/
extern QVector<Message *> ALL_MES;
/*全局变量，存放所有信息发生改变的消息*/
extern QVector<Message *> CHANGE_MES;
/*全局变量，存放所有的子任务反馈*/
extern QVector<Comment *> ALL_COMMENT;
/*全局变量，存放所有信息发生改变的子任务反馈*/
extern QVector<Comment *> CHANGE_COMMENT;
/*全局变量，存放所有的用户评论*/
extern QVector<userComment *> ALL_USERCOMMENTS;
/*全局变量，存放所有信息发生改变的用户评论*/
extern QVector<userComment *> CHANGE_USERCOMMENTS;




#endif // PLATFORM

