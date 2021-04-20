#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include "loginwindow.h"
namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = 0);
    ~RegisterWindow();

    void regisWinRefresh();

signals:
    //返回主界面信号
    void RegbackToMain();

private slots:
    void on_checkuni_clicked();

    void on_psdreread_editingFinished();

    void on_psdedit_editingFinished();

    void on_psdsure_clicked();

    void on_pixchoose_clicked();

    void on_perinfoconfirm_clicked();

    void on_backButton01_clicked();

    void on_backButton02_clicked();

    void on_nextButton_clicked();

    void on_birthEdit_editingFinished();

    void on_backbutton_released();

private:

    QIcon iconmale,iconfemale;
    Ui::RegisterWindow *ui;
    loginWindow LogWin;
};

#endif // REGISTERWINDOW_H
