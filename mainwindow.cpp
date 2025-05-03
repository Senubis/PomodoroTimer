#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 初始化变量
    isWorking = true;
    isPaused = true;
    remainingSeconds = 25 * 60; // 初始为25分钟

    // 创建控件
    timeLabel = new QLabel("25:00", this);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("font-size: 48px;");

    startButton = new QPushButton("开始", this);
    pauseButton = new QPushButton("暂停", this);
    resetButton = new QPushButton("重置", this);

    // 布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    mainLayout->addWidget(timeLabel);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(resetButton);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);

    // 定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);

    // 按钮信号连接
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startTimer);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseTimer);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetTimer);
}

MainWindow::~MainWindow() {}

// 开始/继续计时
void MainWindow::startTimer() {
    if (isPaused) {
        timer->start(1000); // 每秒触发一次
        isPaused = false;
        startButton->setText("继续");
    }
}

// 暂停计时
void MainWindow::pauseTimer() {
    if (!isPaused) {
        timer->stop();
        isPaused = true;
        startButton->setText("开始");
    }
}

// 重置计时
void MainWindow::resetTimer() {
    timer->stop();
    isPaused = true;
    isWorking = true;
    remainingSeconds = 25 * 60;
    updateTime();
    startButton->setText("开始");
}

// 更新显示时间
void MainWindow::updateTime() {
    remainingSeconds--;

    if (remainingSeconds <= 0) {
        switchState();
        return;
    }

    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    timeLabel->setText(QString("%1:%2")
                           .arg(minutes, 2, 10, QLatin1Char('0'))
                           .arg(seconds, 2, 10, QLatin1Char('0')));
}

// 切换工作/休息状态
void MainWindow::switchState() {
    timer->stop();
    isPaused = true;
    isWorking = !isWorking;

    if (isWorking) {
        remainingSeconds = 25 * 60;
        QMessageBox::information(this, "状态切换", "工作时间开始！");
    } else {
        remainingSeconds = 5 * 60;
        QMessageBox::information(this, "状态切换", "休息时间开始！");
    }

    updateTime();
    startButton->setText("开始");
}
