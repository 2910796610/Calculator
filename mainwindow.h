#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QKeyEvent>
#include <QMap>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString operand;//操作数
    QString opcode;//操作符

    QStack<QString> operands;//操作数栈
    QStack<QString> opcodes;//操作符栈

    QMap<int, QPushButton *> digitBTNs;//数字按钮键值对
    QMap<int, QPushButton *> BOpBTNs; //双操作数操作符按钮键值对
    QMap<int, QPushButton *> UOpBTNs; //单操作数操作符按钮键值对
    QMap<int, QPushButton *> otherBTNs; //单操作数操作符按钮键值对


    QString calculation(bool *ok = NULL);//计算逻辑

private slots:

    void btnNumClicked();//数字点击事件

    void btnBinaryOperatorClicked();//双操作数操作符剪辑事件

    void btnUnaryOperatorClicked();//单操作数操作符点击事件

    void on_btnPeriod_clicked();//小数点点击事件

    void on_btnDelete_clicked();//退格点击事件

    void on_Clear_clicked();//清空点击事件

    void on_btnEqual_clicked();//等于点击事件


    void on_btnSign_clicked();//取反点击事件

    virtual void keyPressEvent(QKeyEvent *event);//键盘点击事件

    void on_ClearEntry_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
