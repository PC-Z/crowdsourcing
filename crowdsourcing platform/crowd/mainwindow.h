#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "registerwindow.h"
#include "loginwindow.h"
#include "personmainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_loginButton_clicked();

    void PerBacktoMain();

    void on_registerButton_released();

    void on_serverbutton_released();

private:
    Ui::MainWindow *ui;
    RegisterWindow RegWin;
    loginWindow LogWin;
    PersonMainWindow PerMainWin;
};

#endif // MAINWINDOW_H
