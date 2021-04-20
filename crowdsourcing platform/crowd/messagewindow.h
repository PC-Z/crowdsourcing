#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include "message.h"
class UserInfo;
namespace Ui {
class MessageWindow;
}

class UserInfo;
class MessageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MessageWindow(QWidget *parent = 0);
    ~MessageWindow();
    void setReceivemode(int row,int isread,
         QString &content,QString &sender);
    void setSendmode(int row,QDateTime &sendtime,
         QString &content,QString &reciver);
    void showRecmessage();
    void showSendmessage();
    void messendfromCheckWin(QString &receiver);
    //点击我的消息，切换到空白页面
    void mesWinset();

public slots:

    void on_sendmessageButton_released();

private slots:
    void on_myreceivebutton_released();

    void checkMyReceiveMes();

    void checkMySendMes();

    void on_mysendbutton_released();


    void on_replyButton_released();

    void on_sendButton_released();

    void on_receiveredit_editingFinished();

    void on_backbutton_released();

private:
    Ui::MessageWindow *ui;
    QStandardItemModel *receiveModel;
    QStandardItemModel *sendModel;

};

#endif // MESSAGEWINDOW_H
