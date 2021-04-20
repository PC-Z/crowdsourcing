#ifndef MYTASKWINDOW_H
#define MYTASKWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include "platform.h"
#include "taskinfo.h"
#include "tranwindow.h"
#include "checkuserwindow.h"

namespace Ui {
class mytaskWindow;
}

class mytaskWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mytaskWindow(QWidget *parent = 0);
    ~mytaskWindow();
    //根据输入的task参数在tableview输出任务
    void showMYTaskmodel(QVector<TaskInfo *> userchecktask);
    void showMYTaskmodel(QVector<SubTaskInfo *> userchecktask);
    //译者查看其他译者
    void showMYTaskmodel(QVector<SubTaskInfo *> userchecktask,int);
    //无操作视图的界面设计
    void setTASKview();
    //发布者视图的界面设计
    void setPOSTview();
    //负责人视图的界面设计
    void setLEADview();
    //翻译者视图的界面设计
    void setTRANview();
    //visitor视图的界面设计
    void setVISITORview();
    void setL_VISITORview();
    void setT_VISITORview();
    void setA_VISITORview();
    //tableview表头函数
    void checkTaskset();
    //辅助函数
    void setTaskModel(int row, QString &tname, QString &tposter, int tstate, int myrole);

private slots:
    void checkMYPOSTTask();

    void checkMYLEADTask();

    void checkMYTRANTask();

    void checkVISITORTask();

    void checkOTHERTRANTask();

    void LeadcheckMYSubTask();

    void setSubmitFalse();

    void on_checktranbutton_released();

    void on_checktaskstatebutton_released();

    void on_finishtaskbutton_released();

    void on_paytaskbutton_released();

    void on_backbutton_released();

    void on_l_backbutton_released();

    void on_getbookbutton_released();

    void on_leadersignupbutton_released();

    void on_transignupbutton_released();

    void on_l_transignupbutton_released();

    void on_l_checkposterbutton_released();

    void on_l_gettaskbutton_released();

    void on_l_setddlbutton_released();

    void on_v_checkposterbutton_released();

    void on_v_checkleaderbutton_released();

    void on_v_checktranbutton_released();

    void on_vl_checkposterbutton_released();

    void on_vl_signupleader_released();

    void on_vl_signuptranbutton_released();

    void on_vt_checkposterbutton_released();

    void on_vt_checkleader_released();

    void on_vt_signuptranbutton_released();

    void on_p_checkleaderbutton_released();

    void on_l_checktranbutton_released();

    void on_l_postsubtaskbutton_released();

    void on_t_checkposterbutton_released();

    void on_t_checkleaderbutton_released();

    void on_t_checktranbutton_released();

    void on_t_backbutton_released();

    void on_t_gettaskbutton_released();

    void on_t_entermytaskbutton_released();

    void on_l_checksubbutton_released();

    void on_t_othertaskbutton_released();

    void on_l_submitbutton_released();


private:
    Ui::mytaskWindow *ui;  
    QStandardItemModel *taskModel;
    TranWindow TranWin;
    CheckUserWindow CheckUWin;
};

#endif // MYTASKWINDOW_H
