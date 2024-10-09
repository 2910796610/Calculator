#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtMath>
#include <QDebug>
#include <math.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBTNs = {{Qt::Key_0, ui->btnNum0},//键盘按钮对应控件的哈希表
        {Qt::Key_1, ui->btnNum1},
        {Qt::Key_2, ui->btnNum2},
        {Qt::Key_3, ui->btnNum3},
        {Qt::Key_4, ui->btnNum4},
        {Qt::Key_5, ui->btnNum5},
        {Qt::Key_6, ui->btnNum6},
        {Qt::Key_7, ui->btnNum7},
        {Qt::Key_8, ui->btnNum8},
        {Qt::Key_9, ui->btnNum9}
    };

    foreach (auto btn, digitBTNs) {//auto后面跟的是digitBTNs的一个键值对，比如 {Qt::Key_1, ui->btnNum1}
        connect(btn, SIGNAL(clicked()), this,
                SLOT(btnNumClicked()));//connect函数第一个参数调用的实际上是按钮的指针，即btn.value()

    }

    BOpBTNs = {
        {Qt::Key_Asterisk, ui->btnMultiple },
        {Qt::Key_Slash, ui->btnDivide},
        {Qt::Key_Plus, ui->btnPlus },
        {Qt::Key_Minus, ui->btnMinus }
    };

    foreach (auto btn, BOpBTNs) {
        connect(btn, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    }

    UOpBTNs = {
        { Qt::Key_Percent, ui->btnPercentage},
        {Qt::Key_I, ui->btnInverse },
        {Qt::Key_S, ui->btnSquare },
        {Qt::Key_R, ui->btnSqrt }
    };


    foreach (auto btn, UOpBTNs) {
        connect(btn, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    }

    otherBTNs = {
        {Qt::Key_Equal, ui->btnEqual},
        {Qt::Key_Period, ui->btnPeriod},
        {Qt::Key_Delete, ui->btnDelete},
        {Qt::Key_C, ui->Clear}
    };
}

void MainWindow::keyPressEvent(QKeyEvent *event)//键盘点击事件
{
    foreach (auto btnKey, digitBTNs.keys()) {//auto后面跟的是digitBTNs.keys()的其中一个，比如：Qt::Key_1
        if (event->key() == btnKey)//event->key()返回键key_?而不是键对应的值
            digitBTNs[btnKey]->animateClick();//digitBTNs[key_0]==ui->btnNum0
    }

    // 处理二元操作符按钮
    foreach (auto btnKey, BOpBTNs.keys()) {
        if (event->key() == btnKey) {
            BOpBTNs[btnKey]->animateClick();
            return; // 找到匹配的按钮后退出
        }
    }

    // 处理一元操作符按钮
    foreach (auto btnKey, UOpBTNs.keys()) {
        if (event->key() == btnKey) {
            UOpBTNs[btnKey]->animateClick();
            return; // 找到匹配的按钮后退出
        }
    }

    //处理其他按钮
    foreach (auto btnKey, otherBTNs.keys()) {
        if (event->key() == btnKey) {
            otherBTNs[btnKey]->animateClick();
            return; // 找到匹配的按钮后退出
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)//双操作数计算逻辑
{
    double result = 0;
    if (operands.size() == 2 && opcodes.size() > 0) {

        //取操作数
        double operand1 = operands.front().toDouble();
        operands.pop_front();

        double operand2 = operands.front().toDouble();
        operands.pop_front();

        //取操作符
        QString op = opcodes.front();
        opcodes.pop_front();

        if (op == "+") {
            result = operand1 + operand2;
        } else if (op == "-") {
            result = operand1 - operand2;
        } else if (op == "×") {
            result = operand1 * operand2;
        } else if (op == "/") {
            if (operand2 == 0) {
                return "除数不能为零！";
            } else
                result = operand1 / operand2;
        }

        operands.push_back(QString::number(result));

        ui->statusbar->showMessage(QString("Calculation正在执行；operand:%1,opcode:%2").arg(operands.size()).arg(
                                       opcodes.size()));
        return QString::number(result);
    } else {
        ui->statusbar->showMessage(QString("operand:%1,opcode:%2").arg(operands.size()).arg(opcodes.size()));
        return QString::number(operands.front().toDouble());
    }
}

void MainWindow::btnNumClicked() // 数字点击处理
{
    QString digit = qobject_cast<QPushButton *>(sender())->text();

    if (digit == "0" && operand == "0") {
        digit = ""; // 避免两个零开头
    }

    if (operand == "0" && digit != "0") {
        operand = ""; // 避免类似“01”出现
    }

    operand += digit; // 添加数字

    ui->display->setText(operand); // 更新显示
}



void MainWindow::on_btnPeriod_clicked()//小数点点击事件
{
    if (operand == "")//操作数为空自动补零
        operand = "0" + qobject_cast<QPushButton *>(sender())->text();

    if (!operand.contains("."))
        operand += qobject_cast<QPushButton *>(sender())->text();

    ui->display->setText(operand);
    // ui->statusbar->showMessage(qobject_cast<QPushButton *>(sender())->text() + "btn clicked");
}


void MainWindow::on_btnDelete_clicked()//DEL
{
    operand = operand.left(operand.length() - 1);
    ui->display->setText(operand);
}


void MainWindow::on_Clear_clicked()//C
{
    operand.clear();
    opcode.clear();
    operands.clear();
    opcodes.clear();
    ui->history->setText("");
    ui->display->setText(operand);
}

void MainWindow::btnBinaryOperatorClicked()//双操作数操作符
{
    QString opcode = qobject_cast<QPushButton *>(sender())->text();//获取操作符文本

    if (operand == "") {//操作数为空时，除了减操作符，其他不做任何操作直接返回
        if (opcode == "-") {
            operand = "-"; // 将operand设为负号(此时才能向btnNumClicked函数传递负号，让后续输入的数字变成负数)
            if (operands.size() == 0)
                ui->history->setText("0" + opcode);
            else
                ui->history->setText(operands.front() + opcode);
            ui->display->setText(opcode);
        }
        return;
    }  else { //操作数不为空，则入栈，操作符也入栈
        if (operand != "-") {//不是单纯负号则进入计算
            operands.push_back(operand);
            ui->history->setText(operand + opcode);
            operand = "";
            opcodes.push_back(opcode);
        } else
            return;
    }

    QString result = calculation();

    ui->history->setText(result + opcode);
    ui->display->setText(result);
}

void MainWindow::btnUnaryOperatorClicked()//单操作数操作符
{
    if (operand != "") {
        double result = operand.toDouble();
        operand = "";

        QString op = qobject_cast<QPushButton *>(sender())->text();

        if (op == "%")
            result /= 100.0;
        else if (op == "1/x")
            result = 1 / result;
        else if (op == "x^2")
            result = result * result;
        else if (op == "√")
            result = sqrt(result);

        operand = QString::number(result); //结果存入操作数
        ui->display->setText(QString::number(result));
    }
}

void MainWindow::on_btnEqual_clicked()//=
{
    if (operand == "")
        return;
    else {
        operands.push_back(operand);
        operand = "";
    }
    QString result = calculation();

    operand = result; //结果存入操作数
    ui->history->setText(result);
    ui->display->setText(result);

    operands.clear();
    opcodes.clear();
}

void MainWindow::on_btnSign_clicked()
{
    if (operand != "") {
        double result = 0 - operand.toDouble();

        operand = QString::number(result);
        ui->display->setText(QString::number(result));
    }
}




void MainWindow::on_ClearEntry_clicked()
{
    operand = "";
    ui->display->setText(operand);
}

