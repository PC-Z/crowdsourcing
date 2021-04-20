#include "messagewindow.h"
#include "ui_messagewindow.h"
#include "platform.h"
#include <QMessageBox>

MessageWindow::MessageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MessageWindow)
{
    ui->setupUi(this);
    ui->messagestacked->setCurrentIndex(0);
    //接收信息页面
    receiveModel = new QStandardItemModel;
    receiveModel->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("Sender")));
    receiveModel->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("Read")));
    receiveModel->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("Content")));
    receiveModel->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("Check")));
    ui->myreceive_tableview->setModel(receiveModel);
    ui->myreceive_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->myreceive_tableview->setColumnWidth(0,100);
    ui->myreceive_tableview->setColumnWidth(1,80);
    ui->myreceive_tableview->setColumnWidth(2,100);
    ui->myreceive_tableview->setColumnWidth(3,100);
    showRecmessage();

    //发送信息页面
    sendModel = new QStandardItemModel;
    sendModel->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("Receiver")));
    sendModel->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("SendTime")));
    sendModel->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("Content")));
    sendModel->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("Check")));
    ui->mysend_tableview->setModel(sendModel);
    ui->mysend_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->mysend_tableview->setColumnWidth(0,100);
    ui->mysend_tableview->setColumnWidth(1,100);
    ui->mysend_tableview->setColumnWidth(2,100);
    ui->mysend_tableview->setColumnWidth(3,100);
}

MessageWindow::~MessageWindow()
{
    delete ui;
}

void MessageWindow::mesWinset()
{
    ui->messagestacked->setCurrentIndex(0);
    receiveModel->removeRows(0,receiveModel->rowCount());
    sendModel->removeRows(0,sendModel->rowCount());
}

void MessageWindow::setReceivemode(int row, int isread, QString &content, QString &sender)
{
    QString isreadstr ;
    if(isread)
    {
        isreadstr = "Read";
    }
    else
    {
        isreadstr = "New";
    }
    receiveModel->setItem(row,0,new QStandardItem(sender));
    receiveModel->setItem(row,1,new QStandardItem(isreadstr));
    receiveModel->setItem(row,2,new QStandardItem(content));
    //设置查看按钮
    QPushButton *button = new QPushButton("Check");
    button->setProperty("sender",sender);
    button->setProperty("read",isread);
    button->setProperty("content",content);
    button->setProperty("row",row);
    connect(button, &QPushButton::released, this, &MessageWindow::checkMyReceiveMes);
    ui->myreceive_tableview->setIndexWidget
            (receiveModel->index(receiveModel->rowCount()-1,3), button);
    //设置查看按钮
}


//查看发送的信息（代码冗杂了）
void MessageWindow::setSendmode(int row, QDateTime &sendtime,
                                  QString &content, QString &receiver)
{
    sendModel->setItem(row,0,new QStandardItem(receiver));
    sendModel->setItem(row,1,new QStandardItem(sendtime.toString("yyyy-MM-dd")));
    sendModel->setItem(row,2,new QStandardItem(content));
    //设置查看按钮
    QPushButton *button = new QPushButton("Check");
    button->setProperty("receiver",receiver);
    button->setProperty("sendtime",sendtime);
    button->setProperty("content",content);
    button->setProperty("row",row);
    connect(button, &QPushButton::released, this, &MessageWindow::checkMySendMes);
    ui->mysend_tableview->setIndexWidget
            (sendModel->index(sendModel->rowCount()-1,3), button);
    //设置查看按钮
}
void MessageWindow::showRecmessage()
{
    receiveModel->removeRows(0,receiveModel->rowCount());
    for(int i=0;i<NOW_USER->userRecmes.size();i++)
    {
        int isread = NOW_USER->userRecmes[i]->mesisRead;
        QString content = NOW_USER->userRecmes[i]->mesContent;
        UserInfo *usender = NOW_USER->userRecmes[i]->mesSender;
        QString sender;
        if(usender == 0x0)
        {
            sender = "System";
        }
        else
        {
            sender = usender->userIdentity;
        }
        //qDebug()<<"this message is from "<<sender;
        setReceivemode(i, isread, content, sender);
    }
}

void MessageWindow::showSendmessage()
{
    sendModel->removeRows(0,receiveModel->rowCount());
    for(int i=0;i<NOW_USER->userSendmes.size();i++)
    {
        QDateTime sendtime = NOW_USER->userSendmes[i]->messendtime;
        QString content = NOW_USER->userSendmes[i]->mesContent;
        UserInfo *ureceiver = NOW_USER->userSendmes[i]->mesReceiver;
        QString receiver;
        if(ureceiver == 0x0)
        {
            receiver = "System";
        }
        else
        {
            receiver = ureceiver->userIdentity;
        }
        //qDebug()<<"this message is sent to "<<receiver;
        setSendmode(i, sendtime, content, receiver);

    }
}

void MessageWindow::on_myreceivebutton_released()
{
    ui->messagestacked->setCurrentIndex(0);
    showRecmessage();
}

void MessageWindow::on_backbutton_released()
{
    ui->messagestacked->setCurrentIndex(0);
    showRecmessage();
}

void MessageWindow::checkMyReceiveMes()
{
    ui->messagestacked->setCurrentIndex(1);
    QPushButton *button = (QPushButton *)sender();
    int num = button->property("row").toInt();
    NOW_USER->userRecmes[num]->mesRead();
    NOW_USER->userRecmes[num]->mesUPDATE(&CHANGE_MES);
    UserInfo *usender = UserInfo::userFind(ALL_USERS,button->property("sender").toString());
    if(usender)
    {
        ui->senderpixlabel->setPixmap(QPixmap(usender->userpixadd));
        ui->senderpixlabel->setScaledContents(true);
    }
    else
    {
        ui->senderpixlabel->setPixmap(QPixmap("://photouserini.jpg"));
        ui->senderpixlabel->setScaledContents(true);
    }
    ui->senderidentitylabel->setText(button->property("sender").toString());
    QString message = button->property("sendtime").toString()+
            button->property("sender").toString()+":\n"
            +button->property("content").toString();
    ui->message->setText(message);

}

void MessageWindow::on_mysendbutton_released()
{
    ui->messagestacked->setCurrentIndex(2);
    showSendmessage();
}

void MessageWindow::checkMySendMes()
{
    ui->messagestacked->setCurrentIndex(1);
    QPushButton *button = (QPushButton *)sender();
    ui->senderidentitylabel->setText(button->property("receiver").toString());
    UserInfo *usender = UserInfo::userFind(ALL_USERS,button->property("receiver").toString());
    if(usender)
    {
        ui->senderpixlabel->setPixmap(QPixmap(usender->userpixadd));
        ui->senderpixlabel->setScaledContents(true);
    }
    //系统头像

    QString message = button->property("receiver").toString()+":\n"+
            button->property("sendtime").toString()+":\n"
            +button->property("content").toString();
    ui->message->setText(message);
    ui->message->setEnabled(false);

}

void MessageWindow::on_sendmessageButton_released()
{
    ui->messagestacked->setCurrentIndex(3);
    ui->sendmessage->clear();
    ui->receiveredit->clear();
}

void MessageWindow::on_replyButton_released()
{
    ui->messagestacked->setCurrentIndex(3);
    ui->sendmessage->clear();
    ui->receiveredit->setText(ui->senderidentitylabel->text());
}

void MessageWindow::on_sendButton_released()
{
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this,"question","Send now?",
                          QMessageBox::Yes|QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        QString content = ui->sendmessage->toPlainText();
        QString recidentity = ui->receiveredit->text();
        UserInfo *receiver = UserInfo::userFind(ALL_USERS,recidentity);
        this->close();
        NOW_USER->mesSend(content, receiver, NOW_USER);
        NOW_USER->userUPDATE(&CHANGE_USERS);
        NOW_USER->userUPDATE(&ALL_USERS);
        receiver->userUPDATE(&CHANGE_USERS);
        receiver->userUPDATE(&ALL_USERS);
    }
}

void MessageWindow::on_receiveredit_editingFinished()
{
    QString inputidentity = ui->receiveredit->text();
    UserInfo *receiver = UserInfo::userFind(ALL_USERS,inputidentity);
    if(!receiver)
    {
        QMessageBox::critical(this,"error","User not Exists!");
        ui->receiveredit->clear();
        ui->receiveredit->setFocus();
    }
}

void MessageWindow::messendfromCheckWin(QString &receiver)
{
    on_sendmessageButton_released();
    ui->receiveredit->setText(receiver);
}


