#include "tomatopage.h"
#include "ui_tomatopage.h"
#include "quotewidget.h"
#include <QMessageBox>


/*1. 在setTime函数里需要在调试结束以后把remainWorkTime调整为60
 *2. python脚本路径在createPythonProcess里面调整，filepath变量在tomato.h
 *3. 目前暂时取消了检查python脚本启动是否成功的检查
 *4. endPomodoro的如果结束时剩余时间大于0就不更新时间的判断可优化也可以不优化

*/
TomatoPage::TomatoPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TomatoPage)
{
    ui->setupUi(this);
    this->showMaximized();
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
    //intialize python process
    pythonProcess = new QProcess(this);
    pythonProcess->setProcessChannelMode(QProcess::MergedChannels);
//    connect(pythonProcess, &QProcess::readyReadStandardOutput, [=]() {
//        QString output = pythonProcess->readAll();
//        //qDebug() << "Python 输出:" << output;

//        if (output.contains("__PYTHON_STARTED__")) {
//            // 可以设置一个标志 isPythonStarted = true;
//            isPythonStarted=true;
//        }
//    });

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
    //initialization
    //run python script
    createPythonProcess();
    if(!isPythonStarted)
    isRunning=true;
    setTime();
    remainBreakSecond=3;
    //starting timing
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
            //时长为0时结束python脚本
            killPythonProcess();
            emit updateData();//在结束脚本后，更新数据
            isUpdate=true;

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
    if(remainWorkSecond>0){//如果剩余工作时间》0那么久不会调用文件读写功能
        return;
    }
    else{
        //1. 番茄钟的时长在开始的时候就已经返回了
        //2. 控制python脚本结束
        killPythonProcess();
        //3. 发送endPomodoro信号调用文件读写
        if(isUpdate){
            isUpdate=false;
            return;
        }
        emit updateData();
    }
}

void TomatoPage::timeConvert(int remainSecond){
    // minutes = remainSecond/60;
    // seconds = remainSecond%60;


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
    currentDuration = selectedText.toInt();
    remainWorkSecond= currentDuration * 1;//这里暂时改成1 然后ui暂时改成3s这样方便调试,之后调整为60
    timeConvert(remainWorkSecond);
    emit tomatoDurationChanged(currentDuration);//设定了时间之后我就传递

}

void TomatoPage::createPythonProcess(){

    if (pythonProcess->state() == QProcess::NotRunning) {
        pythonProcess->start("python3", QStringList() << filepath);    // 等待最多3秒启动完成

    }
}

void TomatoPage::killPythonProcess(){
    if (pythonProcess->state() != QProcess::NotRunning) {
        pythonProcess->kill();  // 或者 pythonProcess->terminate();
        pythonProcess->waitForFinished();
    }
}
