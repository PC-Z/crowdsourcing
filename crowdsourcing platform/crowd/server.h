#ifndef SERVER_H
#define SERVER_H

#include "userinfo.h"

/*Server类，包括平台管理员的属性与方法*/
class Server
{
private:
    QString serverAccount;//Server账号
    QString serverPassword;//Server密码

public:
    //Server构造函数
    Server(){}
    //Server析构函数
    ~Server(){}
    //Server含参构造函数
    Server(QString &account, QString &psd);
    //Server账号接口
    QString getserverAccount()const;
    //Server密码借口
    QString getserverPsd()const;
    //Server登录检查
    static bool serverLogin(QString &account,QString &psd);
    //Server设置用户积分
    void serverCredituser(UserInfo *user, int credit);
};
#endif // SERVER_H

