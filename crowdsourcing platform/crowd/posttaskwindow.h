#ifndef POSTTASKWINDOW_H
#define POSTTASKWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include "userinfo.h"

namespace Ui {
class postTaskWindow;
}

class postTaskWindow : public QMainWindow
{
    Q_OBJECT

public:
    friend bool UserInfo::userPostTask(postTaskWindow *a);
    explicit postTaskWindow(QWidget *parent = 0);
    ~postTaskWindow();
    void postWinRefresh();

private slots:
    void on_confirmposttask_released();

    void on_taskpaymentedit_editingFinished();

    void on_choosecoverbutton_released();

    void on_choosebookbutton_released();

private:
    Ui::postTaskWindow *ui;
};

#endif // POSTTASKWINDOW_H
