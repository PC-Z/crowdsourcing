#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "userinfo.h"
#include "platform.h"
#include <QMap>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QMapIterator>
#include <QFileDialog>
#include <QDate>
#include <QRadioButton>
#include <QDialog>

//extern QVector<UserInfo*> ALL_USERS;

RegisterWindow::RegisterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    regisWinRefresh();
    ui->regstacked->setCurrentIndex(0);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

//注册页面刷新
void RegisterWindow::regisWinRefresh()
{
    ui->regstacked->setCurrentIndex(0);
    ui->IdentityEdit->setEnabled(true);
    ui->IdentityEdit->clear();
    ui->identityread->clear();
    ui->accountread->clear();
    ui->psdedit->clear();
    ui->psdreread->clear();
    ui->qualiedit->clear();
    ui->emailEdit->clear();
    ui->birthEdit->calendarPopup();

    ui->pixlabel->setScaledContents(true);
    ui->pixlabel->setPixmap(QPixmap("://photouserini.jpg"));
    ui->pixroad->setText("://photouserini.jpg");
    ui->identityread->setFocusPolicy(Qt::NoFocus);
    ui->IdentityEdit->setText("请输入用户名、身份证号或手机号。");
    ui->accountread->setFocusPolicy(Qt::NoFocus);
    ui->birthEdit->setDate(QDate::currentDate());
    ui->nextButton->setEnabled(false);
    ui->psdedit->setEchoMode(QLineEdit::Password);
    ui->psdreread->setEchoMode(QLineEdit::Password);
    ui->pixroad->setEnabled(false);
    ui->birthEdit->setCalendarPopup(true);
}

//检查identity是否唯一
void RegisterWindow::on_checkuni_clicked()
{
    bool flag = true;
    bool noFault = true;
    QString myinput = ui->IdentityEdit->text();
    //用户选择用手机号码注册
    if((ui->Teleradio->isChecked())&&(ui->IdentityEdit->text()!=NULL))
    {
        if(ui->IdentityEdit->text().size()!=11)
        {
            QMessageBox::critical(this,"error","Please Input Correct TelephoneNumber!");
            noFault = false;
            ui->IdentityEdit->clear();
            ui->IdentityEdit->setFocus();
        }
        else if(noFault)
        {
            for(int i=0;i<11;i++)
            {
                if((ui->IdentityEdit->text()[i]>'9')||(ui->IdentityEdit->text()[i]<'0'))
                {
                    noFault = false;
                    QMessageBox::critical(this,"error","Please Input Correct TelephoneNumber!");
                    ui->IdentityEdit->clear();
                    ui->IdentityEdit->setFocus();
                    break;
                }
            }
        }
    }
    //用户选择用身份证号注册
    else if((ui->IDradio->isChecked())&&(ui->IdentityEdit->text()!=NULL)&&(noFault == true))
    {
        if((ui->IdentityEdit->text().size()!=16)&&(ui->IdentityEdit->text().size()!=18))
        {
            noFault = false;
            QMessageBox::critical(this,"error","Please Input Correct ID CardNumber!");
            ui->IdentityEdit->clear();
            ui->IdentityEdit->setFocus();
        }
        else if((ui->IdentityEdit->text().size()==18)&&(noFault == true))
        {
            for(int i=0;i<17;i++)
            {
                if((ui->IdentityEdit->text()[i]>'9')||(ui->IdentityEdit->text()[i]<'0'))
                {
                    noFault = false;
                    QMessageBox::critical(this,"error","Please Input Correct ID CardNumber!");
                    ui->IdentityEdit->clear();
                    ui->IdentityEdit->setFocus();
                    break;
                }
                //身份证生日检查？
            }
        }
    }

    if((noFault) &&( (ui->IDradio->isChecked())||(ui->Nickradio->isChecked()) ||(ui->Teleradio->isChecked()) ))
    {
        UserInfo *test = UserInfo::userFind(ALL_USERS,myinput);
        if(test)
        {
            QMessageBox::warning(this,"error","This Identity Already Existed!");

            qDebug() <<"标识符重复！";
            ui->IdentityEdit->clear();
            ui->IdentityEdit->setFocus();
            flag = false;
        }

        if(flag == true)
        {
            QMessageBox::information(this, "提示", "验证通过！");
            qDebug()<<"验证通过！";
            ui->IdentityEdit->setEnabled(false);
            //创建一个用户
            UserInfo *newuser = new UserInfo;
            qDebug()<<"创建了一个用户";
            ALL_USERS.push_back(newuser);
            QString str = "Welcome To TranHouse!";
            newuser->mesSend(str,newuser);
            qDebug()<<"yes?";
            ui->regstacked->setCurrentIndex(1);
            ui->identityread->setPlaceholderText(myinput);
            int account =ALL_USERS.size();

            //用户标识、账号确认
            newuser->userAccount = account;
            newuser->userIdentity = myinput;
            //用户标识、账号确认
            newuser->userUPDATE(&CHANGE_USERS);
            qDebug()<<ALL_USERS.size()<< CHANGE_USERS.size();
            //qDebug()<<"create account:"<<ALL_USERS[0]->userIdentity;
            ui->accountread->setPlaceholderText(QString("%1").arg(account, 6, 10, QChar('0')));
        }
    }

}

void RegisterWindow::on_psdreread_editingFinished()
{
    //extern QVector<UserInfo *> ALL_USERS;
    if(ui->psdedit->text() == NULL)
    {
        qDebug()<<"没有输入密码！";

        ui->psdedit->clear();
        ui->psdreread->clear();
        ui->psdedit->setFocus();
        QMessageBox::critical(this,"prompt","Password Cannot Be NULL!");

    }
    else
    {
        QString psdold = ui->psdedit->text();
        QString psdre = ui->psdreread->text();
        if(psdold != psdre)
        {
            qDebug()<<"输入密码不一致！";
            QMessageBox::critical(this,"prompt","The Password isnot the Same!");
            ui->psdedit->clear();
            ui->psdreread->clear();
            ui->psdreread->setFocus();
        }
    }
}

void RegisterWindow::on_psdedit_editingFinished()
{

}

void RegisterWindow::on_psdsure_clicked()
{
    //extern QVector<UserInfo *> ALL_USERS;
    bool flag = true;
    if(ui->psdedit->text()==NULL)
    {
        QMessageBox::critical(this,"error","Password Is Null！");
        ui->psdedit->setFocus();
    }
    else if(ui->psdreread->text()==NULL)
    {
        QMessageBox::critical(this,"error","Please Confirm Your Password!！");
        ui->psdreread->setFocus();
    }
    else if(flag)
    {
        QString useridentity = ui->IdentityEdit->text();
        QString userpsd = ui->psdreread->text();
        //查找到用户
        for(int i=0;i<ALL_USERS.size();i++)
        {
            if(ALL_USERS[i]->userIdentity == useridentity)
            {
                //用户密码确认
                ALL_USERS[i]->setuserPassword(userpsd);

                qDebug()<<"用户密码确认！";
                QMessageBox::information(this,"prompt","Your Password Is Confirm！");
                flag = false;
                ui->regstacked->setCurrentIndex(2);
            }
        }
    }

}

void RegisterWindow::on_pixchoose_clicked()
{
    //extern QVector<UserInfo *> ALL_USERS;
    QFileDialog *pixfile = new QFileDialog(this);
   pixfile->setWindowTitle(tr("Open File"));
   //设置文件默认路径
   pixfile->setDirectory(".");
   //设置文件过滤器
   pixfile->setNameFilter("Images(*.png *.jpg *.jpeg *.bmp)");

   //设置视图
   pixfile->setViewMode(QFileDialog::Detail);
   //设置可以选择多个文件，默认只能选择一个文件QFileDialog::ExistingFiles
   //pixfile->setFileMode(QFileDialog::ExistingFiles);
   //打印所有选择的文件路径
   QStringList filenames;
   if(pixfile->exec())
   {
       filenames = pixfile->selectedFiles();
   }
   for(auto tmp : filenames)
   {
       ui->pixroad->setText(tmp);
       ui->pixlabel->setPixmap(tmp);
       ui->pixlabel->setScaledContents(true);
   }


}


void RegisterWindow::on_perinfoconfirm_clicked()
{
    //extern QVector<UserInfo *> ALL_USERS;
    //bool flag = true;
    QString useridentity = ui->IdentityEdit->text();
        NOW_USER = UserInfo::userFind(ALL_USERS,useridentity);
        QString str = ui->sexedit->currentText();
        NOW_USER->userSex = str;
        NOW_USER->userBirthday = ui->birthEdit->date();
        NOW_USER->userQualification = ui->qualiedit->toPlainText();
        NOW_USER->userEmail = ui->emailEdit->text();
        NOW_USER->userpixadd = ui->pixroad->text();
        QString qualification = ui->qualiedit->toPlainText();
        NOW_USER->setuserCreditIni(qualification);
        NOW_USER->userUPDATE(&CHANGE_USERS);

        QMessageBox::information(this,"prompt","Successfully registered!");
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this,"question","Login now?",
                              QMessageBox::Yes|QMessageBox::No);
        if(result == QMessageBox::Yes)
        {
            this->hide();
            LogWin.show();
        }
        else if(result == QMessageBox::No)
        {
            this->hide();
            emit RegbackToMain();
        }

}

/*原本用于login与register切换
void RegisterWindow::SendbackToLogin()
{
    emit backToLogin();
}
*/

void RegisterWindow::on_backButton01_clicked()
{
    ui->regstacked->setCurrentIndex(1);
}

void RegisterWindow::on_backButton02_clicked()
{
    ui->regstacked->setCurrentIndex(0);
    ui->nextButton->setEnabled(true);
    ui->checkuni->setEnabled(false);

}

void RegisterWindow::on_nextButton_clicked()
{
    ui->regstacked->setCurrentIndex(1);
}

void RegisterWindow::on_birthEdit_editingFinished()
{
    QDate userbir = ui->birthEdit->date();
    if(userbir > QDateTime::currentDateTime().date())
    {
        QMessageBox::critical(this,"error","Wrong Birthday!");
    }
}

void RegisterWindow::on_backbutton_released()
{
    this->hide();
    emit RegbackToMain();
}
