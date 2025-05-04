#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void startTimer();
    void pauseTimer();
    void resetTimer();
    void updateTime();

private:
    void switchState();
    void loadSettings();
    void saveSettings();

    //void setBackGround();哈哈没写对写你妈

    QTimer *timer;
    QLabel *timeLabel;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *resetButton;
    QSpinBox *workSpinBox;
    QSpinBox *restSpinBox;

    int remainingSeconds;
    bool isWorking;
    bool isPaused;
};

#endif // MAINWINDOW_H
