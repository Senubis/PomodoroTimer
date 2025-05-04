// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    isWorking(true),
    isPaused(true)
{
    // 初始化控件
    workSpinBox = new QSpinBox(this);
    workSpinBox->setRange(1, 120);
    workSpinBox->setSuffix(" 分钟");

    restSpinBox = new QSpinBox(this);
    restSpinBox->setRange(1, 60);
    restSpinBox->setSuffix(" 分钟");

    timeLabel = new QLabel(this);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("font-size: 48px; color: #00ffcc;");//哎私货色号

    startButton = new QPushButton("开始", this);
    pauseButton = new QPushButton("暂停", this);
    resetButton = new QPushButton("重置", this);
    pauseButton->setEnabled(false);

    // 布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("专注时长:", workSpinBox);
    formLayout->addRow("休息时长:", restSpinBox);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(resetButton);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(timeLabel, 1);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);
    resize(350, 250);

    // 初始化
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);

    // 信号槽
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startTimer);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseTimer);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetTimer);

    // 输入变化时自动重置
    connect(workSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::resetTimer);
    connect(restSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::resetTimer);

    // 加载保存的设置
    loadSettings();
    resetTimer();
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}

void MainWindow::startTimer()
{
    if (isPaused) {
        timer->start(1000);
        isPaused = false;
        startButton->setText("继续");
        pauseButton->setEnabled(true);

        // 禁用设置控件
        workSpinBox->setEnabled(false);
        restSpinBox->setEnabled(false);
    }
}

void MainWindow::pauseTimer()
{
    if (!isPaused) {
        timer->stop();
        isPaused = true;
        startButton->setText("开始");
        pauseButton->setEnabled(false);

        // 启用设置控件
        workSpinBox->setEnabled(true);
        restSpinBox->setEnabled(true);
    }
}

void MainWindow::resetTimer()
{
    timer->stop();
    isPaused = true;
    isWorking = true;
    remainingSeconds = workSpinBox->value() * 60;

    updateTime();
    startButton->setText("开始");
    pauseButton->setEnabled(false);

    // 启用设置控件
    workSpinBox->setEnabled(true);
    restSpinBox->setEnabled(true);
}





void MainWindow::updateTime()
{
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





void MainWindow::switchState()
{
    timer->stop();
    isPaused = true;
    isWorking = !isWorking;

    if (isWorking) {
        remainingSeconds = workSpinBox->value() * 60;
        QMessageBox::information(this, "状态切换", "专注时间开始！");
    } else {
        remainingSeconds = restSpinBox->value() * 60;
        QMessageBox::information(this, "状态切换", "休息时间开始！");
    }

    updateTime();
    startButton->setText("开始");
    pauseButton->setEnabled(false);

    // 切换状态后允许修改设置
    workSpinBox->setEnabled(true);
    restSpinBox->setEnabled(true);
}





void MainWindow::loadSettings()//哎操你妈为什么封装会出bug！
{
    QSettings settings("PomodoroApp", "Timer");
    workSpinBox->setValue(settings.value("workTime", 25).toInt());
    restSpinBox->setValue(settings.value("restTime", 5).toInt());
}

void MainWindow::saveSettings()
{
    QSettings settings("PomodoroApp", "Timer");
    settings.setValue("workTime", workSpinBox->value());
    settings.setValue("restTime", restSpinBox->value());
}
