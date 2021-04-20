#ifndef MESSAGE
#define MESSAGE
#include "userinfo.h"
#include "platform.h"
#include <QString>
#include <QVector>
#include <QDateTime>

/*Message类，用于用户消息盒子模块*/
class UserInfo;
class Message
{
public:
    QString mesContent;//消息内容
    UserInfo *mesSender;//消息发送者
    UserInfo *mesReceiver;//消息接收者
    QDateTime messendtime;//消息发送时间
    bool mesisRead;//消息是否阅读

    Message(){}//不含参数的构造函数
    Message(QString &content, UserInfo *receiver,UserInfo *sender);//含参构造函数
    ~Message(){}//析构函数
    void mesRead();//阅读消息，将消息是否阅读标记改变为true
    void mesUPDATE(QVector<Message *> *meslist);//消息更新

};


#endif // MESSAGE

