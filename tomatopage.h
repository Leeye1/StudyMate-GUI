#ifndef TOMATOPAGE_H
#define TOMATOPAGE_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QProcess>


namespace Ui {
class TomatoPage;
}

class TomatoPage : public QWidget
{
    Q_OBJECT

public:
    explicit TomatoPage(QWidget *parent = nullptr);
    ~TomatoPage();
public:
    void timeConvert(int remainTime);
    void showTomato(int tomatoNum);
    void createPythonProcess();
    void killPythonProcess();

signals:
    void back();
    void tomatoDurationChanged(int duration);
    void updateData();

private slots:
    void startPomodoro();    // 开始番茄钟
    void updateTime();       // 每秒更新一次剩余时间
    void endPomodoro();    // 结束番茄钟
    void returnMainwindow();//回到主界面
    void setTime();


private:
    Ui::TomatoPage *ui;
    QTimer* timer;
    int remainWorkSecond;
    int remainBreakSecond;
    int currentDuration;

    bool isUpdate;
    bool isRunning;//检查番茄钟是否在运行的函数
    bool isPythonStarted;

    QString timeString;
    QString tomatoString="番茄数：0";

    int tomatoNum=0;

    QProcess *pythonProcess;
    QString filepath="/home/elf/yolov5n8/yolov8.py";


};

#endif // TOMATOPAGE_H
