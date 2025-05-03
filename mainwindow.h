#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startTimer();     // 开始/继续计时
    void pauseTimer();     // 暂停计时
    void resetTimer();     // 重置计时
    void updateTime();     // 更新倒计时

private:
    void switchState();    // 切换工作/休息状态

    QTimer *timer;        // 定时器
    QLabel *timeLabel;     // 显示剩余时间
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *resetButton;

    int remainingSeconds; // 剩余秒数
    bool isWorking;       // 当前是否为工作时间
    bool isPaused;        // 是否暂停
};

#endif // MAINWINDOW_H
