#include "posttaskwindow.h"
#include "ui_posttaskwindow.h"
#include "platform.h"
#include <QMessageBox>
#include <QFileDialog>


postTaskWindow::postTaskWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::postTaskWindow)
{
    ui->setupUi(this);

}

postTaskWindow::~postTaskWindow()
{
    delete ui;
}

void postTaskWindow::postWinRefresh()
{
    ui->bookbrowser->clear();
    ui->bookcoverlabel->setPixmap(QPixmap("://bookcover.jpg"));
    ui->bookcoverroad->setText("://bookcover.jpg");
    ui->booknameedit->clear();
    ui->circletimeedit->clear();
    ui->signupdeadlineedit->setDate(QDate::currentDate());
    ui->circletimeedit->setValidator(new QIntValidator(1,1000,this));
    ui->signupdeadlineedit->setDate(QDate::currentDate());
    ui->taskpaymentedit->clear();
    ui->taskintroedit->clear();
}

bool UserInfo::userPostTask(postTaskWindow *a)
{
    bool issuccess = false;
    bool isPost = true;
    u_POST *userp = new u_POST(*this);
    userp->userUPDATE(&CHANGE_USERS);
    userp->userUPDATE(&ALL_USERS);
    NOW_USER = UserInfo::userFind(ALL_USERS,userp->userIdentity);
    QString taskbookname = a->ui->booknameedit->text();
    if(taskbookname.isEmpty())
    {
        QMessageBox::information(a,"prompt","Please Input BookName!");
        isPost = false;
    }
    //从文件读取内容
    QString bookcontent = a->ui->bookbrowser->toPlainText();
    if(bookcontent.isEmpty())
    {
        QMessageBox::information(a,"prompt","Please Load Your Book!");
        isPost = false;
    }
    QString bookoriLan = a->ui->bookorilanedit->currentText();
    //截取前几句？
    int prelength = bookcontent.length();
    QString bookpreread = bookcontent.left(prelength/5);
    QString taskname = a->ui->booknameedit->text()+userIdentity; //任务名字，唯一标识
    QString tasktargetLan = a->ui->targetlanedit->currentText(); //任务目标语言
    if(bookoriLan == tasktargetLan)
    {
        QMessageBox::information(a,"prompt","Please Choose Target Language!");
        isPost = false;
    }
    QString taskintro = a->ui->taskintroedit->toPlainText();    //任务描述
    if(taskintro.isEmpty())
    {
        QMessageBox::information(a,"prompt","Please Input Introduction!");
        isPost = false;
    }
    TASKSTATE taskstate = WaitingLeader;                        //任务状态
    UserInfo *taskposter = this;                                //任务发布者
    float taskpay = a->ui->taskpaymentedit->text().toFloat();   //任务酬金
    QDate taskpostDay = QDateTime::currentDateTime().date();    //任务发起时间
    QDate taskleadDay = a->ui->signupdeadlineedit->date();      //负责人报名截止时间
    if(taskleadDay < QDate::currentDate())
    {
        QMessageBox::information(a,"prompt","Wrong Deadline!");
        isPost = false;
    }
    QString taskcirclestr = a->ui->circletimeedit->text();
    int taskcircleDay = a->ui->circletimeedit->text().toInt();  //任务周期
    if((taskcircleDay <= 0)||(taskcirclestr.isEmpty()))
    {
        QMessageBox::information(a,"prompt","Please Input Correct Circle Day!");
        isPost = false;
    }
    QString taskcover = a->ui->bookcoverroad->text();
    if(isPost)
    {
        TaskInfo* newtask = new TaskInfo;
        Book *newbook = new Book;
        newbook->bookName = taskbookname;
        newbook->bookContent = bookcontent;
        newbook->bookoriLan = bookoriLan;
        newbook->bookPreread = bookpreread;
        newtask->taskBook = newbook;

        newtask->taskPoster = userp;
        newtask->taskLeader = 0x0;
        newtask->taskName = taskname;
        newtask->tasktargetLan = tasktargetLan;
        newtask->taskIntro = taskintro;
        newtask->setTaskstate(taskstate);
        newtask->taskPoster = taskposter;
        newtask->setTaskpay(taskpay);
        newtask->taskPostDay = taskpostDay;
        newtask->taskLeadDay = taskleadDay;
        newtask->taskTranDay = taskleadDay;
        newtask->setTaskcircle(taskcircleDay);
        newtask->taskCover = taskcover;
        newtask->setTaskpaystate(0);
        newtask->setsubTasknum(0);
        //数据更新，change更改部分，用户user部分
        bool isFind = false;
        for(int i=0;i<ALL_TASKS.size();i++)
        {
            if(ALL_TASKS[i]->taskName == taskname)
            {
                isFind = true;
                break;
            }
        }
        if(!isFind)
        {
            userp->p_userChangeState(newtask,WaitingLeader);
            newtask->taskUPDATE(&ALL_TASKS);
            newtask->taskUPDATE(&CHANGE_TASKS);

            userp->setuserMoney(-(newtask->getTaskpay()));
            NOW_USER->setuserMoney(-(newtask->getTaskpay()));
            NOW_USER->userUPDATE(&ALL_USERS);
            NOW_USER->userUPDATE(&CHANGE_USERS);

            newtask->taskUPDATE(&(NOW_USER->userTasks));
            newtask->taskUPDATE(&(NOW_USER->userPost));
            issuccess = true;
        }
        else
        {
            QMessageBox::information(a,"prompt","You've Post Similar Task Before!");
        }
    }
    return issuccess;
}

void postTaskWindow::on_confirmposttask_released()
{
    bool success = false;
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this,"question","Post Now?",
                          QMessageBox::Yes|QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        success = NOW_USER->userPostTask(this);
        if(success)
        {
            this->close();
        }
        else
        {
            qDebug()<<"任务发布失败。";
        }
    }
}



void postTaskWindow::on_taskpaymentedit_editingFinished()
{
    float tpay = ui->taskpaymentedit->text().toFloat();
    if(NOW_USER->getuserMoney() < tpay)
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this,"question","Not Enough Money.Transfer In Now?",
                              QMessageBox::Yes|QMessageBox::No);
        if(result == QMessageBox::Yes)
        {
            QString dlgTitle="Transfer";
            QString txtLabel="Please Input Money";
            float defaultValue=20.00;

            float minValue=0, maxValue=10000;  //范围
            int decimals=2;//小数点位数

            bool ok=false;
            float inputValue = QInputDialog::getDouble(this, dlgTitle,txtLabel,
                                    defaultValue, minValue,maxValue,decimals,&ok);
            if (ok) //确认选择
            {
                NOW_USER->setuserMoney(inputValue);
            }
        }
        else if(result == QMessageBox::No)
        {
            ui->taskpaymentedit->clear();
        }
    }
    else if(tpay<=0)
    {
        QMessageBox::information(this,"prompt","Please Input Correct Money!");
        ui->taskpaymentedit->clear();
    }
    NOW_USER->userUPDATE(&CHANGE_USERS);
}

void postTaskWindow::on_choosecoverbutton_released()
{
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
        ui->bookcoverroad->setText(tmp);
        ui->bookcoverlabel->setPixmap(tmp);
        ui->bookcoverlabel->setScaledContents(true);
    }
}

void postTaskWindow::on_choosebookbutton_released()
{
    QString path = QFileDialog::getOpenFileName(this,
                "Open File",".","TXT(*.txt)");
    if(path.isEmpty() == false)
    {
        //文件对象
        QFile file(path);
        //打开文件
        bool isOK = file.open(QIODevice::ReadOnly);
        if(isOK)
        {
            //读文件
            QByteArray array;
            while( file.atEnd() == false )
            {
                array += file.readLine();
            }
            ui->bookbrowser->setText(array);
        }
        file.close();
    }

}

