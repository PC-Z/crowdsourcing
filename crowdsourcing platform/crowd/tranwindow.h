#ifndef TRANWINDOW_H
#define TRANWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTextBrowser>
#include "checkuserwindow.h"

namespace Ui {
class TranWindow;
}

class TranWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TranWindow(QWidget *parent = 0);
    ~TranWindow();
    //刷新
    void tranwinRefresh();
    //获取图书界面
    void setgetbookview();
    //译者
    void settranview();
    //译者互译
    void setotherTranview();
    //发布者查看已经完成的任务
    void setposterview();
    //负责人分割任务
    void setleaderDivideview(int num);
    //负责人检察任务
    void setLeaderCheckview();
    //负责人提交任务
    void setLeaderSubmitview(int num);

signals:
    void SubmitbackTOtask();

private slots:
    void on_t_commentbutton_released();

    void on_t_commentclosetool_released();

    void on_p_closebutton_released();

    void on_vd_confirmsubtaskbutton_released();

    void on_t_savebutton_released();

    void on_t_submitbutton_released();

    void on_t_lastsubmitbutton_released();

    void on_t_lastsavebutton_released();

    void on_l_commentbutton_released();

    void on_l_translaterbutton_released();

    void on_l_commentclosetool_released();

    void on_pushButton_2_released();

    void on_pushButton_4_released();

    void on_to_commentclosetool_released();

    void on_l_commentconfirm_released();

    void on_l_commentbefore_released();

    void on_to_commentbefore_released();

    void on_to_commentconfirm_released();

    void on_ls_submitconfirmbutton_released();

    void on_ls_savebutton_released();

    void on_ls_lastsavebutton_released();

    void on_refreshbutton_released();

private:
    Ui::TranWindow *ui;
    QVector<QTextEdit *> SubEdit;
    QVector<QTextBrowser *> SubRead;
    CheckUserWindow CheckUWin;
};

#endif // TRANWINDOW_H
