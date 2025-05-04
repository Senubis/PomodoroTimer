#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSettings>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    isWorking(true),
    isPaused(true)
{
    workSpinBox = new QSpinBox(this);
    workSpinBox->setRange(1, 120);
    workSpinBox->setSuffix(" 分钟");

    restSpinBox = new QSpinBox(this);
    restSpinBox->setRange(1, 60);
    restSpinBox->setSuffix(" 分钟");

    timeLabel = new QLabel(this);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("font-size: 48px; color: #00ffcc;");

    statusLabel = new QLabel("准备就绪", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 24px; color: white;");

    startButton = new QPushButton("开始", this);
    pauseButton = new QPushButton("暂停", this);
    resetButton = new QPushButton("重置", this);
    pauseButton->setEnabled(false);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("专注时长:", workSpinBox);
    formLayout->addRow("休息时长:", restSpinBox);

    mainLayout->addLayout(formLayout);mainLayout->addWidget(statusLabel);mainLayout->addWidget(timeLabel, 1);
QHBoxLayout *buttonLayout = new QHBoxLayout();buttonLayout->addWidget(startButton);buttonLayout->addWidget(pauseButton);buttonLayout->addWidget(resetButton);mainLayout->addLayout(buttonLayout);setCentralWidget(centralWidget);resize(350, 250);timer = new QTimer(this);connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);connect(startButton, &QPushButton::clicked, this, &MainWindow::startTimer);connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseTimer);connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetTimer);connect(workSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),this, &MainWindow::resetTimer);connect(restSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),this, &MainWindow::resetTimer);loadSettings();resetTimer();}MainWindow::~MainWindow() {}void MainWindow::closeEvent(QCloseEvent *event){saveSettings();QMainWindow::closeEvent(event);}void MainWindow::startTimer(){if (isPaused) {timer->start(1000);isPaused = false;startButton->setText("继续");pauseButton->setEnabled(true);workSpinBox->setEnabled(false);restSpinBox->setEnabled(false);}}void MainWindow::pauseTimer(){if (!isPaused) {timer->stop();isPaused = true;startButton->setText("开始");pauseButton->setEnabled(false);workSpinBox->setEnabled(true);restSpinBox->setEnabled(true);}}void MainWindow::resetTimer(){timer->stop();isPaused = true;isWorking = true;remainingSeconds = workSpinBox->value() * 60;statusLabel->setText("专注中");updateTime();startButton->setText("开始");pauseButton->setEnabled(false);workSpinBox->setEnabled(true);restSpinBox->setEnabled(true);}void MainWindow::updateTime(){remainingSeconds--;if (remainingSeconds <= 0) {switchState();return;}int minutes = remainingSeconds / 60;int seconds = remainingSeconds % 60;timeLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0')));}void MainWindow::switchState(){timer->stop();isPaused = true;isWorking = !isWorking;if (isWorking) {remainingSeconds = workSpinBox->value() * 60;statusLabel->setText("专注中");} else {remainingSeconds = restSpinBox->value() * 60;statusLabel->setText("休息中");}updateTime();startButton->setText("开始");pauseButton->setEnabled(false);workSpinBox->setEnabled(true);restSpinBox->setEnabled(true);}void MainWindow::loadSettings(){QSettings settings("PomodoroApp", "Timer");workSpinBox->setValue(settings.value("workTime", 25).toInt());restSpinBox->setValue(settings.value("restTime", 5).toInt());}
void MainWindow::saveSettings(){QSettings settings("PomodoroApp", "Timer");settings.setValue("workTime", workSpinBox->value());settings.setValue("restTime", restSpinBox->value());}
/*等下bro为什么你真的在看这个代码？真是辛苦你了看半天构式东西。其实你也用不着费心分析了因为我写到这里根本就决定要退学（不是）了；以及我一怒之下删除了所有的注释和一些分段（好吧在版本2里面还是比较清楚的）就为了让我的代码成为依托。我不想写了。我的人生就像这个程序一样，我搞不明白一切；我搞砸了一切。*/
