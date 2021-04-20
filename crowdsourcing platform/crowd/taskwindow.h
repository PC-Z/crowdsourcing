#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QMainWindow>

namespace Ui {
class TaskWindow;
}

class TaskWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TaskWindow(QWidget *parent = 0);
    ~TaskWindow();

private:
    Ui::TaskWindow *ui;
};

#endif // TASKWINDOW_H
