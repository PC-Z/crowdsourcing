/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2019-08-03
Description:消息类功能
**************************************************/
#include "message.h"
#include <QDebug>


Message::Message(QString &content, UserInfo *receiver,  UserInfo *sender):
  mesContent(content),mesSender(sender),mesReceiver(receiver),mesisRead(false)
{
    messendtime = QDateTime::currentDateTime();
}

//消息已读
void Message::mesRead()
{
    mesisRead = true;
    mesUPDATE(&CHANGE_MES);
}

//消息更新
void Message::mesUPDATE(QVector<Message *> *meslist)
{
    bool ismes = false;
    for(int i=0;i<meslist->size();i++)
    {
        if(((*meslist)[i]->mesSender == mesSender) &&
           ((*meslist)[i]->messendtime == messendtime) &&
           ((*meslist)[i]->mesReceiver == mesReceiver))
        {
            ismes = true;
            (*meslist)[i] = this;
            break;
        }
    }
    if(!ismes)
    {
        meslist->push_front(this);
    }
    //qDebug()<<"is message exist?"<<ismes<<CHANGE_MES.size();
}


