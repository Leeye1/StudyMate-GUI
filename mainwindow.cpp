#include "mainwindow.h"
#include "ui_mainwindow.h"

/*有一个问题就是如果吧MainWindow掩藏了，那么就会导致其他窗口弹出，
 * 且在此进入这些窗口的时候就无法再次变成全屏了。
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->tomatoPage= new TomatoPage();
    this->reportPage= new ReportPage();
    this->reportPage->hide();
    this->tomatoPage->hide();
    this->showMaximized();
    connect(tomatoPage, &TomatoPage::tomatoDurationChanged, reportPage, &ReportPage::setTomatoDuration);
    connect(tomatoPage, &TomatoPage::updateData, reportPage, &ReportPage::updateJSON);
    //界面交互逻辑
    connect(ui->tomatoButton,&QPushButton::clicked,[=](){
        //把主窗口隐藏，第二个窗口界面显示
        this->hide();
        this->reportPage->hide();
        this->tomatoPage->showMaximized();
    });
    connect(this->tomatoPage,&TomatoPage::back,[=](){
        this->tomatoPage->hide();
        this->show();
    });
    connect(ui->reportButton,&QPushButton::clicked,[=](){
        //把主窗口隐藏，第二个窗口界面显示
        this->hide();
        this->tomatoPage->hide();
        this->reportPage->showMaximized();
    });
    connect(this->reportPage,&ReportPage::back,[=](){
        this->reportPage->hide();
        this->show();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_quitButton_clicked()
{
    QApplication::quit();
}
