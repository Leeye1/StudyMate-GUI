#include "tomatopage.h"
#include "ui_tomatopage.h"
#include "quotewidget.h"
//时间加框和调整各个元素的字体颜色
//提示音
//有个番茄钟
//layout 自动扩展
TomatoPage::TomatoPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TomatoPage)
{
    ui->setupUi(this);
    ui->timeLabel->setText("开始专注吧！");
    ui->tomatoLabel->setText(tomatoString);
    // 居中显示
    ui->timeLabel->setAlignment(Qt::AlignCenter);
    ui->tomatoLabel->setAlignment(Qt::AlignCenter);
    //set font
    QFont font;
    font.setPointSize(24);  // 设置字体大小，例如 24pt
    font.setBold(true);     // 可选：加粗
    ui->timeLabel->setFont(font);
    ui->tomatoLabel->setFont(font);
    //set time
    TomatoPage::setTime();

    //Place quote widget
    QuoteWidget *quoteWidget = new QuoteWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(ui->quotePlaceholder);
    layout->setContentsMargins(0, 0, 0, 0); // 可选：让布局更紧凑
    layout->addWidget(quoteWidget);



    connect(ui->startButton,&QPushButton::clicked,this,&TomatoPage::startPomodoro);
    //可以做一个选择时间的部分，这个逻辑应该在start部分检验
    this->timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&TomatoPage::updateTime);
    connect(ui->endButton,&QPushButton::clicked,this,&TomatoPage::endPomodoro);
    connect(ui->returnButton,&QPushButton::clicked,this,&TomatoPage::returnMainwindow);
}

TomatoPage::~TomatoPage()
{
    delete ui;
}

void TomatoPage::startPomodoro(){
    if(isRunning) return; //如果已经启动了番茄钟那么就不会响应
    isRunning=true;
    setTime();
    remainBreakSecond=3;
    timer->start(1000);
    ui->timeLabel->setText("剩余专注时长: "+timeString);
}
void TomatoPage::updateTime(){
    if(remainWorkSecond>=0){
        timeConvert(remainWorkSecond);
        ui->timeLabel->setText("剩余专注时长: "+timeString);
        if(remainWorkSecond==0){
            tomatoNum++;
            showTomato(tomatoNum);
        }
        remainWorkSecond--;
        //qDebug() << timeString;
    }
    else if(remainBreakSecond>=0){
        timeConvert(remainBreakSecond);
        ui->timeLabel->setText("剩余休息时长: "+timeString);
        if(remainBreakSecond==0){
            endPomodoro();
        }
        remainBreakSecond--;
        //qDebug() << timeString;

    }

}
void TomatoPage::endPomodoro(){
    timer->stop();
    ui->timeLabel->setText("开始专注吧！");
    isRunning=false;
    setTime();

}

void TomatoPage::timeConvert(int remainSecond){
    // minutes = remainSecond/60;
    // seconds = remainSecond%60;

    // QTime time(0, minutes, seconds);  // 创建一个 QTime 对象，时=0，分=minutes，秒=seconds
    // timeString = time.toString("mm:ss");  // 输出格式为 例如 "25:00"
    int hours = remainSecond / 3600;
    int minutes = (remainSecond % 3600) / 60;
    int seconds = remainSecond % 60;

    if (hours > 0)
        timeString = QTime(hours, minutes, seconds).toString("hh:mm:ss");
    else
        timeString = QTime(0, minutes, seconds).toString("mm:ss");
}


void TomatoPage::showTomato(int tomatoNum){
    tomatoString = QString("番茄数：%1").arg(tomatoNum);
    ui->tomatoLabel->setText(tomatoString);
}

void TomatoPage::returnMainwindow(){
    emit this->back();
}

void TomatoPage::setTime(){
    //获取combo box数据
    QString selectedText = ui->workTimeComboBox->currentText(); // 例如 "25 分钟"
    int workMinute = selectedText.toInt();
    remainWorkSecond= workMinute * 60;
    timeConvert(remainWorkSecond);

}
