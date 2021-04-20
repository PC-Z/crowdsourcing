/*************************************************
Copyright:zhengpc18@mails.tsinghua.edu.cn
Author:zhengpc
Date:2010-08-03
Description:Server类（用户平台函数实现）
**************************************************/
#include "server.h"
/*全局变量，存放所有的用户*/
extern QVector<UserInfo *> ALL_USERS;
/*全局变量，存放所有的任务*/
extern QVector<UserInfo *> CHANGE_USERS;
/*全局变量，存放所有的平台管理员*/
extern QVector<Server *> ALL_SERVERS;
/*全局变量，存放当前操作的平台管理员*/
extern Server *NOW_SERVER;

/*************************************************
Function:       Server
Description:    Server类含参构造函数
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QString类型account，表示Server账号
                QString类型psd，表示Server密码
Output:         //
Return:         //
Others:         //
*************************************************/
Server::Server(QString &account, QString &psd):
    serverAccount(account),serverPassword(psd)
{}

/*************************************************
Function:       getserverAccount
Description:    获取平台管理员账号
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         返回QString类型的管理员账号
Others:         //
*************************************************/
QString Server::getserverAccount()const
{
    return serverAccount;
}


/*************************************************
Function:       getserverPsd
Description:    返回Server密码
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
Output:         //
Return:         返回QString类型的管理员密码
Others:         //
*************************************************/
QString Server::getserverPsd()const
{
    return serverPassword;
}

/*************************************************
Function:       serverCredituser
Description:    管理员设置用户初始积分
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          UserInfo *类型user，表示待修改的用户指针
                int类型credit，表示输入的积分
Output:         //
Return:         //
Others:         //
*************************************************/
void Server::serverCredituser(UserInfo *user, int credit)
{
    user->usersetCredit(credit);
    user->userUPDATE(&CHANGE_USERS);
}

/*************************************************
Function:       serverLogin
Description:    管理员登录检查
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          QString类型account，表示Server账号
                QString类型psd，表示Server密码
Output:         //
Return:         //
Others:         本函数会将管理员账号和密码进行匹配，
                若账号不存在或者账号密码不匹配，将会报错
*************************************************/
bool Server::serverLogin(QString &account, QString &psd)
{
    bool islogin = false;
    for(int i=0;i<ALL_SERVERS.size();i++)
    {
        if((ALL_SERVERS[i]->getserverAccount() == account)
         &&(ALL_SERVERS[i]->getserverPsd() == psd))
        {
            islogin = true;
            NOW_SERVER = ALL_SERVERS[i];
            break;
        }

    }
    return islogin;
}
